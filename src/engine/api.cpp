link_export b32
Bonsai_OnLibraryLoad(engine_resources *Resources)
{
  b32 Result = InitializeOpenglFunctions();
#if DEBUG_SYSTEM_API
  Global_DebugStatePointer = Resources->DebugState;
#endif

  Global_ThreadStates = Resources->ThreadStates;
  Global_EngineResources = Resources;

  // We should only ever call this from the main thread, and this sets our
  // thread index such that the game doesn't have to worry about doing it.
  if (ThreadLocal_ThreadIndex == -1) { SetThreadLocal_ThreadIndex(0); }
  else { Assert(ThreadLocal_ThreadIndex == 0); }

  return Result;
}

link_export b32
Bonsai_Init(engine_resources *Resources)
{
  TIMED_FUNCTION();
  platform *Plat = Resources->Plat;

  b32 Result = True;

  memory_arena *BonsaiInitArena = AllocateArena(Megabytes(256));;
  DEBUG_REGISTER_ARENA(BonsaiInitArena, 0);

  Resources->Memory = BonsaiInitArena;
  Resources->Heap = InitHeap(Gigabytes(4));

  Init_Global_QuadVertexBuffer();

  Resources->World = Allocate(world, BonsaiInitArena, 1);
  if (!Resources->World) { Error("Allocating World"); return False; }

  memory_arena *GraphicsMemory = AllocateArena();
  Resources->Graphics = GraphicsInit(GraphicsMemory);
  if (!Resources->Graphics) { Error("Initializing Graphics"); return False; }

  memory_arena *GraphicsMemory2D = AllocateArena();
  InitRenderer2D(&Resources->Ui, &Resources->Heap, GraphicsMemory2D, &Plat->MouseP, &Plat->MouseDP, &Plat->ScreenDim, &Plat->Input);

  Resources->EntityTable = AllocateEntityTable(BonsaiInitArena, TOTAL_ENTITY_COUNT);

  return Result;
}

link_export b32
Bonsai_FrameBegin(engine_resources *Resources)
{
  TIMED_FUNCTION();

  // Must come before we update the frame index
  CollectUnusedChunks(Resources, &Resources->MeshFreelist, Resources->World->Memory, Resources->World->VisibleRegion);

  Resources->FrameIndex += 1;

  // Must come before UNPACK_ENGINE_RESOURCES such that we unpack the correct GpuMap
  graphics *G = Resources->Graphics;
  G->GpuBufferWriteIndex = 0;
  G->GpuBufferWriteIndex = (Resources->FrameIndex) % 2;

  UNPACK_ENGINE_RESOURCES(Resources);

  if (Input->F12.Pressed) { EngineDebug->TriggerRuntimeBreak = True; }

  World->ChunkHash = CurrentWorldHashtable(Resources);

  MapGpuElementBuffer(GpuMap);

  ClearFramebuffers(Graphics);

  if (GetEngineDebug()->DrawWorldAxies)
  {
    untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL*3);
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(10000, 0, 0), RED, 0.15f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.15f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 0, 10000), BLUE, 0.15f );
  }

  UiFrameBegin(&Resources->Ui);


  Resources->MousedOverVoxel = MousePickVoxel(Resources);

#if 0 // DEBUG_SYSTEM_API
  if (GetDebugState()->UiGroup.PressedInteractionId != StringHash("GameViewport"))
  {
    GameInput = 0;
  }
#endif

  Graphics->Lighting.Lights.Count = 0;

  DoEngineDebugMenu(Resources);

  b32 Result = True;
  return Result;
}

link_export b32
Bonsai_FrameEnd(engine_resources *Resources)
{
  /* DoEngineDebugMenu(Resources); */
  UiFrameEnd(&Resources->Ui);

  b32 Result = True;
  return Result;
}

link_export b32
Bonsai_SimulateAndBufferGeometry(engine_resources *Resources)
{
  TIMED_FUNCTION();

  SignalFutex(&Resources->Plat->HighPriorityModeFutex);

  UNPACK_ENGINE_RESOURCES(Resources);

#if 0 // DEBUG_SYSTEM_API
  if (GetDebugState()->UiGroup.PressedInteractionId != StringHash("GameViewport"))
  {
    GameInput = 0;
  }
#endif

  SimulateEntities(Resources, Plat->dt, World->VisibleRegion, &GpuMap->Buffer, &Plat->HighPriority);
  /* DispatchSimulateParticleSystemJobs(&Plat->HighPriority, EntityTable, World->ChunkDim, &GpuMap->Buffer, Graphics, Plat->dt); */

  // NOTE(Jesse): This has to come after the entities simulate, and before the
  // draw; we have to update the camera target p before we do the camera update
  //
  auto CameraTargetP = Resources->CameraTarget ? Resources->CameraTarget->P : Canonical_Position(0);


  {
    input *InputForCamera = 0;
    if (UiCapturedMouseInput(Ui) == False)
    {
      InputForCamera = &Plat->Input;
    }

    v2 MouseDelta = GetMouseDelta(Plat);
    UpdateGameCamera(World, MouseDelta, InputForCamera, CameraTargetP, Camera);
    Resources->Graphics->gBuffer->ViewProjection =
      ProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
      ViewMatrix(World->ChunkDim, Camera);
  }

  if (World->Flags & WorldFlag_WorldCenterFollowsCameraTarget)
  {
    World->Center = CameraTargetP.WorldP;
  }

  BufferWorld(Plat, &GpuMap->Buffer, World, Graphics, Heap);
  BufferEntities( EntityTable, &GpuMap->Buffer, Graphics, World, Plat->dt);

  UnsignalFutex(&Resources->Plat->HighPriorityModeFutex);

#if DEBUG_SYSTEM_API
  Debug_DoWorldChunkPicking(Resources);
#endif

  b32 Result = True;
  return Result;
}

link_export b32
Bonsai_Render(engine_resources *Resources)
{
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Resources);

  ao_render_group     *AoGroup = Graphics->AoGroup;
  shadow_render_group *SG      = Graphics->SG;

  r32 MappedGameTime = Plat->GameTime / 18.0f;
  /* r32 MappedGameTime = Plat->GameTime; */
  /* r32 MappedGameTime = Plat->GameTime/2.f; */

  /* r32 tDaytime = (Cos(MappedGameTime) + 1.f) / 2.f; */
  r32 tDaytime = Cos(MappedGameTime);
  r32 tPostApex = Sin(MappedGameTime);

  f32 SunIntensity = 1.f;
  f32 MoonIntensity = 0.01f;

  f32 DawnIntensity = 0.5f;
  f32 DuskIntensity = 0.1f;

  v3 DawnColor = Normalize(V3(0.3f, 0.2f, 0.2f)) * DawnIntensity;
  v3 SunColor  = Normalize(V3(0.2f, 0.2f, 0.3f)) * SunIntensity;
  v3 DuskColor = Normalize(V3(0.4f, 0.2f, 0.2f)) * DuskIntensity;
  v3 MoonColor = Normalize(V3(0.2f, 0.2f, 0.5f)) * MoonIntensity;

  if (tDaytime > 0.f)
  {
    if (tPostApex > 0.f)
    {
      SG->Sun.Color = Lerp(tDaytime, DuskColor, SunColor);
    }
    else
    {
      SG->Sun.Color = Lerp(tDaytime, DawnColor, SunColor);
    }
  }
  else
  {
    /* SG->Sun.Color = V3(0.15f); */
    if (tPostApex > 0.f)
    {
      SG->Sun.Color = Lerp(Abs(tDaytime), DuskColor, MoonColor);
    }
    else
    {
      SG->Sun.Color = Lerp(Abs(tDaytime), DawnColor, MoonColor);
    }
  }

  SG->Sun.Position.x = Sin(MappedGameTime);
  SG->Sun.Position.y = tDaytime;
  SG->Sun.Position.z = tDaytime*0.7f + 1.3f;


  // NOTE(Jesse): GBuffer and ShadowMap must be rendered in series because they
  // both do operate on the total scene geometry The rest of the render passes
  // operate on the textures they create and only render a quad.
  RenderGBuffer(GpuMap, Graphics);
  RenderShadowMap(GpuMap, Graphics);

  RenderLuminanceTexture(GpuMap, Lighting, Graphics);
  RenderAoTexture(AoGroup);

  /* GaussianBlurTexture(&Graphics->Gaussian, AoGroup->Texture); */
  if (Graphics->Settings.UseLightingBloom) { GaussianBlurTexture(&Graphics->Gaussian, Graphics->Lighting.BloomTex, &Graphics->Lighting.BloomTextureFBO); }

  CompositeAndDisplay(Plat, Graphics);

  /* Debug_DrawTextureToDebugQuad(&Graphics->Gaussian.DebugTextureShader1); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->Gaussian.DebugTextureShader0); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->Lighting.DebugBloomShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->Lighting.DebugLightingShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->SG->DebugTextureShader); */
  /* Debug_DrawTextureToDebugQuad(&AoGroup->DebugSsaoShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->gBuffer->DebugColorShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->gBuffer->DebugPositionShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->gBuffer->DebugNormalShader); */

  GpuMap->Buffer.At = 0;
  GL.DisableVertexAttribArray(0);
  GL.DisableVertexAttribArray(1);
  GL.DisableVertexAttribArray(2);

  /* DebugVisualize(Ui, &Resources->MeshFreelist); */
  /* DebugVisualize(Ui, Resources->World->FreeChunks, (s32)Resources->World->FreeChunkCount); */


  {
    /* DoEngineDebugMenu(Ui); */

    // NOTE(Jesse): This should probably render the last such that we can
    // capture all the debug tracking info we want to
    /* Ui->ScreenDim = V2(Plat->WindowWidth, Plat->WindowHeight); */
    /* FlushCommandBuffer(Ui, Ui->CommandBuffer); */
  }

  b32 Result = True;
  return Result;
}

link_internal void
WorkerThreadDefaultImplementations(BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS)
{
  world *World = Thread->EngineResources->World;

  work_queue_entry_type Type = Entry->Type;
  switch (Type)
  {
    InvalidCase(type_work_queue_entry_noop);
    InvalidCase(type_work_queue_entry__align_to_cache_line_helper);

    case type_work_queue_entry_init_asset:
    {
      InvalidCodePath();
    } break;

    case type_work_queue_entry_update_world_region:
    {
      work_queue_entry_update_world_region *Job = SafeAccess(work_queue_entry_update_world_region, Entry);

      picked_voxel Location = Job->Location;
      r32 Radius = Job->Radius;

      DoWorldUpdate(&Thread->EngineResources->Plat->LowPriority, World, Job->ChunkBuffer, Job->ChunkCount, &Location, Job->MinP, Job->MaxP, Job->Op, Job->ColorIndex, Radius, Thread);
    } break;

    case type_work_queue_entry_sim_particle_system:
    {
      work_queue_entry_sim_particle_system *Job = SafeAccess(work_queue_entry_sim_particle_system, Entry);
      SimulateParticleSystem(Job);
    } break;

    case type_work_queue_entry_rebuild_mesh:
    {
      work_queue_entry_rebuild_mesh *Job = SafeAccess(work_queue_entry_rebuild_mesh, Entry);
      world_chunk *Chunk = Job->Chunk;
      RebuildWorldChunkMesh(Thread, Chunk);
    } break;

    case type_work_queue_entry_init_world_chunk:
    {
      work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;

      counted_string AssetFilename = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);
      native_file AssetFile = OpenFile(AssetFilename, "r+b");

      if (ChunkIsGarbage(Chunk))
      {
        // NOTE(Jesse): This is an optimization; the engine marks chunks that
        // have moved outside of the visible region as garbage.
        Chunk->Flags = Chunk_Uninitialized;
      }
      else
      {
        s32 Frequency = 50;
        s32 Amplititude = 15;
        s32 StartingZDepth = -5;

        Assert(Chunk->Dim == World->ChunkDim);
        InitializeWorldChunkPerlinPlane( Thread,
                                         Chunk,
                                         Chunk->Dim,
                                         &AssetFile,
                                         Frequency,
                                         Amplititude,
                                         StartingZDepth,
                                         ChunkInitFlag_ComputeStandingSpots );
      }

    } break;

    case type_work_queue_entry_copy_buffer_ref:
    {
      work_queue_entry_copy_buffer_ref *CopyJob = SafeAccess(work_queue_entry_copy_buffer_ref, Entry);
      DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
    } break;

    case type_work_queue_entry_copy_buffer_set:
    {
      TIMED_BLOCK("Copy Set");
      volatile work_queue_entry_copy_buffer_set *CopySet = SafeAccess(work_queue_entry_copy_buffer_set, Entry);
      for (u32 CopyIndex = 0; CopyIndex < CopySet->Count; ++CopyIndex)
      {
        work_queue_entry_copy_buffer_ref *CopyJob = (work_queue_entry_copy_buffer_ref *)CopySet->CopyTargets + CopyIndex;
        DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
      }
      END_BLOCK("Copy Set");
    } break;
  }
}

link_export void
WorkerInit(engine_resources *Engine, thread_startup_params *StartupParams)
{
  if (ThreadLocal_ThreadIndex == -1) { SetThreadLocal_ThreadIndex(StartupParams->ThreadIndex); }
#if DEBUG_SYSTEM_API 
  Assert(GetDebugState());
#endif
  DEBUG_REGISTER_THREAD(StartupParams);
}
