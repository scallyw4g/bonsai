
link_export b32
Bonsai_OnLibraryLoad(engine_resources *Resources)
{
  // We should only ever call this from the main thread, and this sets our
  // thread index such that the game doesn't have to worry about doing it.
  if (ThreadLocal_ThreadIndex == -1) { SetThreadLocal_ThreadIndex(0); }
  else { Assert(ThreadLocal_ThreadIndex == 0); }

#if DEBUG_SYSTEM_API
  Global_DebugStatePointer = Resources->DebugState;
#endif

  Global_ThreadStates = Resources->Stdlib.ThreadStates;
  Global_EngineResources = Resources;

  b32 Result = InitializeOpenglFunctions();
  return Result;
}

link_export b32
Bonsai_Init(engine_resources *Resources)
{
  TIMED_FUNCTION();

  platform *Plat = &Resources->Stdlib.Plat;

  b32 Result = True;

  memory_arena *BonsaiInitArena = AllocateArena(Megabytes(256));;
  DEBUG_REGISTER_ARENA(BonsaiInitArena, 0);

  Resources->Memory = BonsaiInitArena;
  Resources->Heap = InitHeap(Gigabytes(2));

  Init_Global_QuadVertexBuffer();

  Resources->World = Allocate(world, BonsaiInitArena, 1);
  if (!Resources->World) { Error("Allocating World"); return False; }

  Resources->Graphics = GraphicsInit(AllocateArena());
  if (!Resources->Graphics) { Error("Initializing Graphics"); return False; }

  InitRenderer2D(&Resources->Ui, &Resources->Heap, AllocateArena(), &Plat->MouseP, &Plat->MouseDP, &Plat->ScreenDim, &Plat->Input);

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
  Resources->Graphics->GpuBufferWriteIndex = 0;
  Resources->Graphics->GpuBufferWriteIndex = (Resources->FrameIndex) % 2;

  UNPACK_ENGINE_RESOURCES(Resources);

  if (Input->F12.Pressed) { EngineDebug->TriggerRuntimeBreak = True; }

  World->ChunkHash = CurrentWorldHashtable(Resources);

  ClearFramebuffers(Graphics, &Resources->RTTGroup);


#if 0
  // TODO(Jesse): Is this random leftover debug code that can be removed?
  if (EngineDebug->SelectedAsset.Type)
  {
    asset *Asset = GetAsset(Resources, &EngineDebug->SelectedAsset);
    if (Asset->LoadState == AssetLoadState_Loaded)
    {
      /* UpdateGameCamera(World, {}, 0, Canonical_Position(V3(Asset->Model.Dim/2), V3i(0)), Resources->RTTGroup.Camera); */
      /* RenderToTexture(Resources, &Asset->Model.Mesh); */
    }
  }
#endif

  MapGpuElementBuffer(GpuMap);
  MapGpuElementBuffer(&Graphics->Transparency.GpuBuffer);

  if (GetEngineDebug()->DrawWorldAxies)
  {
    untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL*3);
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(10000, 0, 0), RED, 0.15f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.15f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 0, 10000), BLUE, 0.15f );
  }


  Graphics->Lighting.Lights.Count = 0;

  {
    f32 CameraSpeed = 80.f;
    v3 Offset = GetCameraRelativeInput(Hotkeys, Camera);
    Offset.z = 0; // Constrain to XY plane

    if (Input->E.Pressed) { Offset.z += 1.f; }
    if (Input->Q.Pressed) { Offset.z -= 1.f; }

    Offset = Normalize(Offset);
    /* Camera->ViewingTarget.Offset += Offset; */
    if (Resources->CameraGhost) { Resources->CameraGhost->P.Offset += Offset * Plat->dt * CameraSpeed; }

    // NOTE(Jesse): This has to come before we draw any of the game geometry.
    // Specifically, if it comes after we draw bounding boxes for anything
    // the bounding box lines shift when we move the camera because they're
    // then a frame late.
    //
    cp CameraGhostP = Resources->CameraGhost ? Resources->CameraGhost->P : Canonical_Position(0);

    input *InputForCamera = 0;
    v2 MouseDelta = GetMouseDelta(Plat);

    /* Info("UiCapturedMouseInput %d", UiCapturedMouseInput(Ui)); */
    if (UiCapturedMouseInput(Ui) == False) { InputForCamera = &Plat->Input; }
    UpdateGameCamera(World, MouseDelta, InputForCamera, CameraGhostP, Camera, DEFAULT_CAMERA_BLENDING*Plat->dt);

    Resources->Graphics->gBuffer->ViewProjection =
      ProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
      ViewMatrix(World->ChunkDim, Camera);

    if (World->Flags & WorldFlag_WorldCenterFollowsCameraTarget) { World->Center = CameraGhostP.WorldP; }
  }

  Resources->MaybeMouseRay = ComputeRayFromCursor(Resources, &gBuffer->ViewProjection, Camera, World->ChunkDim);
  Resources->MousedOverVoxel = MousePickVoxel(Resources);

  UiFrameBegin(&Resources->Ui);
  DoEngineDebug(Resources);

  b32 Result = True;
  return Result;
}

link_export b32
Bonsai_FrameEnd(engine_resources *Resources)
{
  UiFrameEnd(&Resources->Ui);

  b32 Result = True;
  return Result;
}

link_export b32
Bonsai_SimulateAndBufferGeometry(engine_resources *Resources)
{
  TIMED_FUNCTION();

  SignalFutex(&Resources->Stdlib.Plat.HighPriorityModeFutex);

  UNPACK_ENGINE_RESOURCES(Resources);

#if 0 // DEBUG_SYSTEM_API
  if (GetDebugState()->UiGroup.PressedInteractionId != StringHash("GameViewport"))
  {
    GameInput = 0;
  }
#endif

  SimulateEntities(Resources, Plat->dt, World->VisibleRegion, &GpuMap->Buffer, &Graphics->Transparency.GpuBuffer.Buffer, &Plat->HighPriority);
  /* DispatchSimulateParticleSystemJobs(&Plat->HighPriority, EntityTable, World->ChunkDim, &GpuMap->Buffer, Graphics, Plat->dt); */

  BufferEntities( EntityTable, &GpuMap->Buffer, &Graphics->Transparency.GpuBuffer.Buffer, Graphics, World, Plat->dt);
  /* BufferEntities( EntityTable, &Graphics->Transparency.GpuBuffer.Buffer, Graphics, World, Plat->dt); */

  UnsignalFutex(&Resources->Stdlib.Plat.HighPriorityModeFutex);

#if DEBUG_SYSTEM_API
  Debug_DoWorldChunkPicking(Resources);
#endif

  b32 Result = True;
  return Result;
}

link_internal void
DoDayNightCycle(graphics *Graphics, r32 MappedGameTime)
{
  auto SG = Graphics->SG;
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

  if (Graphics->Settings.DoDayNightCycle)
  {
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
  }
  else
  {
    SG->Sun.Color = SunColor;
  }

  SG->Sun.Position.x = Sin(MappedGameTime);
  SG->Sun.Position.y = tDaytime;
  SG->Sun.Position.z = tDaytime*0.7f + 1.3f;

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
  DoDayNightCycle(Graphics, MappedGameTime);

  v3 CameraTargetSimP = GetSimSpaceP(World, Resources->CameraGhost);
  Graphics->Settings.OffsetOfWorldCenterToGrid.x = fmodf(CameraTargetSimP.x, Graphics->Settings.MajorGridDim);
  Graphics->Settings.OffsetOfWorldCenterToGrid.y = fmodf(CameraTargetSimP.y, Graphics->Settings.MajorGridDim);
  Graphics->Settings.OffsetOfWorldCenterToGrid.z = fmodf(CameraTargetSimP.z, Graphics->Settings.MajorGridDim);

  EngineDebug->Render.BytesSolidGeoLastFrame = GpuMap->Buffer.At;
  EngineDebug->Render.BytesTransGeoLastFrame = Graphics->Transparency.GpuBuffer.Buffer.At;

  DrawWorldToGBuffer(Plat, World, Graphics);
  DrawWorldToShadowMap(Resources);

  // TODO(Jesse): Move into engine debug
  DebugHighlightWorldChunkBasedOnState(Graphics, EngineDebug->PickedChunk, &GpuMap->Buffer);

  // NOTE(Jesse): GBuffer and ShadowMap must be rendered in series because they
  // both do operate on the total scene geometry. The rest of the render passes
  // operate on the textures they create and only render a quad.
  RenderGBuffer(GpuMap, Graphics);
  RenderShadowMap(GpuMap, Graphics);

  // NOTE(Jesse): I observed the AO lagging a frame behind if this is re-ordered
  // after the transparency/luminance textures.  I have literally 0 ideas as to
  // why that would be, but here we are.
  if (Graphics->Settings.UseSsao) { RenderAoTexture(AoGroup); }

  RenderTransparencyBuffers(&Graphics->Settings, &Graphics->Transparency);
  RenderLuminanceTexture(GpuMap, Lighting, Graphics);

  /* GaussianBlurTexture(&Graphics->Gaussian, AoGroup->Texture); */
  if (Graphics->Settings.UseLightingBloom) { GaussianBlurTexture(&Graphics->Gaussian, Graphics->Lighting.BloomTex, &Graphics->Lighting.BloomTextureFBO); }

  CompositeAndDisplay(Plat, Graphics);

  GpuMap->Buffer.At = 0;
  GL.DisableVertexAttribArray(0);
  GL.DisableVertexAttribArray(1);
  GL.DisableVertexAttribArray(2);


  /* DebugVisualize(Ui, &Resources->MeshFreelist); */
  /* DebugVisualize(Ui, Resources->World->FreeChunks, (s32)Resources->World->FreeChunkCount); */

  b32 Result = True;
  return Result;
}

link_weak void
WorkerThread_ApplicationDefaultImplementation(BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS)
{
  engine_resources *EngineResources = GetEngineResources();
  world *World = EngineResources->World;

  work_queue_entry_type Type = Entry->Type;
  switch (Type)
  {
    InvalidCase(type_work_queue_entry_noop);
    InvalidCase(type_work_queue_entry__align_to_cache_line_helper);

    case type_work_queue_entry_init_asset:
    {
      work_queue_entry_init_asset *Job = SafeAccess(work_queue_entry_init_asset, Entry);
      InitAsset(Job->Asset, Thread);
    } break;

    case type_work_queue_entry_update_world_region:
    {
      work_queue_entry_update_world_region *Job = SafeAccess(work_queue_entry_update_world_region, Entry);
      DoWorldUpdate(&EngineResources->Stdlib.Plat.LowPriority, World, Thread, Job);
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

      untextured_3d_geometry_buffer *TempMesh = AllocateTempWorldChunkMesh(Thread->TempMemory);

      RebuildWorldChunkMesh(Thread, Chunk, {}, Chunk->Dim, MeshBit_Lod0, TempMesh, Thread->TempMemory);
      TempMesh->At = 0;
      RebuildWorldChunkMesh(Thread, Chunk, {}, Chunk->Dim, MeshBit_Lod1, TempMesh, Thread->TempMemory);
      TempMesh->At = 0;
      RebuildWorldChunkMesh(Thread, Chunk, {}, Chunk->Dim, MeshBit_Lod2, TempMesh, Thread->TempMemory);
      TempMesh->At = 0;
      RebuildWorldChunkMesh(Thread, Chunk, {}, Chunk->Dim, MeshBit_Lod3, TempMesh, Thread->TempMemory);
      TempMesh->At = 0;
      RebuildWorldChunkMesh(Thread, Chunk, {}, Chunk->Dim, MeshBit_Lod4, TempMesh, Thread->TempMemory);
      TempMesh->At = 0;

      /* UnsetBitfield(chunk_flag, Chunk->Flags, Chunk_Queued); */
      Chunk->Flags = chunk_flag(Chunk->Flags & ~Chunk_Queued);
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
      DoCopyJob(CopyJob, &EngineResources->MeshFreelist, Thread->PermMemory);
    } break;

    case type_work_queue_entry_copy_buffer_set:
    {
      TIMED_BLOCK("Copy Set");
      volatile work_queue_entry_copy_buffer_set *CopySet = SafeAccess(work_queue_entry_copy_buffer_set, Entry);
      for (u32 CopyIndex = 0; CopyIndex < CopySet->Count; ++CopyIndex)
      {
        work_queue_entry_copy_buffer_ref *CopyJob = (work_queue_entry_copy_buffer_ref *)CopySet->CopyTargets + CopyIndex;
        DoCopyJob(CopyJob, &EngineResources->MeshFreelist, Thread->PermMemory);
      }
      END_BLOCK("Copy Set");
    } break;
  }
}
