
link_export b32
Bonsai_OnLibraryLoad(engine_resources *Resources)
{
  // We should only ever call this from the main thread, and this sets our
  // thread index such that the game doesn't have to worry about doing it.
  if (ThreadLocal_ThreadIndex == -1) { SetThreadLocal_ThreadIndex(0); }
  else { Assert(ThreadLocal_ThreadIndex == 0); }

  Global_ThreadStates = Resources->Stdlib.ThreadStates;
  Global_EngineResources = Resources;

  /* Initialize_Global_UpdateWorldCallbackTable(); */

  /* b32 Result = InitializeOpenglFunctions(); */
  return True;
}

link_export b32
Bonsai_Init(engine_resources *Resources)
{
  TIMED_FUNCTION();

  b32 Result = True;

  Result &= InitEngineDebug(&Resources->EngineDebug);
  Result &= InitEditor(&Resources->Editor);
  Result &= InitEngineResources(Resources);

  /* Initialize_Global_UpdateWorldCallbackTable(); */

  return Result;
}

link_internal void
SimulateCameraGhost_AndSet_OffsetWorldCenterToGrid(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  if (entity *CameraGhost = GetEntity(EntityTable, Camera->GhostId))
  {
    SimulateEntity(Engine, CameraGhost, Plat->dt, V3i(World->VisibleRegionSize), &GpuMap->Buffer, &Graphics->Transparency.GpuBuffer.Buffer, &Plat->HighPriority);

    v3 CameraTargetSimP = GetSimSpaceP(World, CameraGhost);
    Graphics->OffsetOfWorldCenterToGrid = (CameraTargetSimP % V3(Graphics->Settings.MajorGridDim));
  }
}

// NOTE(Jesse): This function was useful for sanity checking the hashtable state at
// known-empty points.  It's not valid to have it in there all the time (because
// at startup entities can queue pieces of the world, and as such the first frame
// can have stuff in the hashtable before these checks run.  Keeping the function
// around in case I ever want it again.
link_internal void
DEBUG_AssertWorldChunkHashtableIsEmpty(engine_resources *Engine, world_chunk **Table)
{
#if 0
  RangeIterator_t(u32, HashIndex, Engine->World->HashSize)
  {
    Assert(Table[HashIndex] == 0);
  }
#endif
}

link_export b32
Bonsai_FrameBegin(engine_resources *Resources)
{
  TIMED_FUNCTION();

  PushBonsaiRenderCommandClearAllFramebuffers(&Resources->Stdlib.Plat.RenderQ);

  // NOTE(Jesse): This gets cleared before CollectUnusedChunks because that's
  // the thing that is populating the next hashtable
  /* Resources->World->HashSlotsUsed = 0; */

  /* DEBUG_AssertWorldChunkHashtableIsEmpty(Resources, NextWorldHashtable(Resources)); */

  // NOTE(Jesse): Must come before we update the frame index becaues
  // CollectUnusedChunks picks the hashtable based on the frame index.
  //
  /* CollectUnusedChunksAndClearCurrentTable(Resources, Resources->World->VisibleRegion); */

  /* DEBUG_AssertWorldChunkHashtableIsEmpty(Resources, CurrentWorldHashtable(Resources)); */

  // NOTE(Jesse): Must be updated before we simulate camera ghost because the
  // sim pulls chunks out of the hashtable.
  //
  Resources->FrameIndex += 1;


  b32 VRChanged = False;
  DETECT_CHANGES_ON(Resources->World->VisibleRegionSize, PrevValue,
  {
    Info("Changed From (%S) to (%S)", ToString(PrevValue), ToString(Resources->World->VisibleRegionSize));
    VRChanged = True;
  });


  if ( VRChanged                                                  ||
       Resources->Graphics.WorldEditRC.Program.HotReloaded        ||
       Resources->Graphics.TerrainShapingRC.Program.HotReloaded   ||
       Resources->Graphics.TerrainDerivsRC.Program.HotReloaded    ||
       Resources->Graphics.TerrainDecorationRC.Program.HotReloaded )
  {
    auto Plat = &Resources->Stdlib.Plat;

    SignalAndWaitForWorkers(&Plat->WorkerThreadsSuspendFutex);
    CancelAllWorkQueueJobs(Resources);
    Assert(Count(&Resources->Graphics.NoiseReadbackJobs) == 0);

    HardResetWorld(Resources);
    ApplyEditBufferToOctree(Resources, &Resources->Editor.Edits);
    /* HardResetEditor(&Resources->Editor); */

    UnsignalFutex(&Plat->WorkerThreadsSuspendFutex);
  }

  MaintainWorldOctree(Resources);

  /* Resources->World->ChunkHash = CurrentWorldHashtable(Resources); */

  /* DEBUG_AssertWorldChunkHashtableIsEmpty(Resources, NextWorldHashtable(Resources)); */

  /* ComputeDrawListsAndQueueUnallocatedChunks(Resources); */

  /* DEBUG_AssertWorldChunkHashtableIsEmpty(Resources, NextWorldHashtable(Resources)); */

  // NOTE(Jesse): This is a special-case entity simulation routine such that we
  // can dispatch the draw commands for world chunks afterwards.  The reason we
  // need this is because the world grid requires an offset which is computed
  // from the sim-space position of the camera ghost.
  //
  // @early_camera_ghost_simulation
  //
  SimulateCameraGhost_AndSet_OffsetWorldCenterToGrid(Resources);

  // Must come before UNPACK_ENGINE_RESOURCES such that we unpack the correct GpuMap
  /* Resources->Graphics.GpuBufferWriteIndex = 0; */
  /* Resources->Graphics.GpuBufferWriteIndex = (Resources->FrameIndex) % 2; */


  UNPACK_ENGINE_RESOURCES(Resources);

  if (GetEngineDebug()->DrawWorldAxies)
  {
    untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL*3);
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(10000, 0, 0), RGB_RED,   0.35f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 10000, 0), RGB_GREEN, 0.35f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 0, 10000), RGB_BLUE,  0.35f );
  }

  /* if (GetEngineDebug()->DrawGameCameraLocation) */
  {
    if (Resources->Graphics.Camera == &Resources->Graphics.DebugCamera)
    {
      /* DEBUG_HighlightVoxel(Engine, Camera->ViewingTarget, RED); */
      DEBUG_HighlightVoxel(Resources, Graphics->GameCamera.CurrentP, RGB_PINK, 10.f);
    }
  }


  Graphics->Lighting.Lights.Count = 0;

  // NOTE(Jesse): This is a little weird in that we're using the ray from last
  // frame, but we have to update the camera after we do the UI draw in case
  // the UI captures the input
  //
  // @camera-update-ui-update-frame-jank
  //
  //

  f32 LineWidth = 3.f;
  LineWidth = .1f;
  if (UiHoveredMouseInput(Ui))
  {
    // We want to preserve these when we swap to the debug camera
    if (Camera == &Graphics->GameCamera)
    {
      Resources->MaybeMouseRay   = {};
      Resources->MousedOverVoxel = {};
      Resources->HoverEntity     = {};
    }
  }
  else
  {
    if (Camera == &Graphics->GameCamera)
    {
      Resources->MaybeMouseRay   = ComputeCameraSpaceRayFromCursor(Resources, &gBuffer->ViewProjection, &Resources->Graphics.GameCamera, World->ChunkDim);

      ray *Ray = &Resources->MaybeMouseRay.Ray;
      v3 GameCameraSimSpaceP = GetSimSpaceP(World, Graphics->GameCamera.CurrentP);
      Ray->Origin = GameCameraSimSpaceP;
    }

    ray *Ray = &Resources->MaybeMouseRay.Ray;

    /* DEBUG_VALUE_r32(Ray->Origin.x); */
    /* DEBUG_VALUE_r32(Ray->Origin.y); */
    /* DEBUG_VALUE_r32(Ray->Origin.z); */

    /* DEBUG_VALUE_r32(Ray->Dir.x); */
    /* DEBUG_VALUE_r32(Ray->Dir.y); */
    /* DEBUG_VALUE_r32(Ray->Dir.z); */

    Resources->MousedOverVoxel = MousePickVoxel(Resources, Ray);
    Resources->HoverEntity     = GetClosestEntityIntersectingRay(World, EntityTable, &Resources->MaybeMouseRay.Ray);
  }

#if 0
  {
    ray *Ray = &Resources->MaybeMouseRay.Ray;

    {
      untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_LINE);
      DEBUG_DrawLine(&Mesh, V3(0.,0.,LineWidth) + Ray->Origin, Ray->Origin + Ray->Dir*50000.f, RGB_PINK, LineWidth );
    }

    picked_octree_node_block_array NodeList = GetOctreeLeafNodesIntersectingRay(World, Ray, GetTranArena());
    IterateOver(&NodeList, PickedNode, NodeIndex)
    {
      random_series Entropy = {Cast(u64, PickedNode->Node)};
      v3 Color = RandomV3Unilateral(&Entropy);
      aabb AABB = GetSimSpaceAABB(World, PickedNode->Node);
      DEBUG_DrawSimSpaceAABB(Resources, &AABB, Color, 3.f);

      v3 IntersectionPoint = Ray->Origin + Ray->Dir*PickedNode->t;
      DEBUG_HighlightVoxel(Resources, IntersectionPoint, Color, 3);

    }
  }
#endif

  // Find closest standing spot to cursor
  {
    Resources->ClosestStandingSpotToCursor = {};

    f32 ShortestDistanceToPlayerSq = f32_MAX;
    umm ClosestTileIndex = umm_MAX;
    if (Resources->MousedOverVoxel.Tag)
    {
      cp MouseVoxelCP = Canonical_Position(&Resources->MousedOverVoxel.Value);
      v3 MouseVoxelSimP = GetSimSpaceP(World, MouseVoxelCP);
      standing_spot_buffer Spots = GetStandingSpotsWithinRadius(World, MouseVoxelCP, 8, GetTranArena());
      IterateOver(&Spots, Spot, SpotIndex)
      {
        v3 SpotSimP = GetSimSpaceP(World, Spot->P) + Global_StandingSpotHalfDim;
        r32 ThisDist = DistanceSq(SpotSimP, MouseVoxelSimP);
        if (ThisDist < ShortestDistanceToPlayerSq)
        {
          ShortestDistanceToPlayerSq = ThisDist;
          ClosestTileIndex = SpotIndex;
        }
      }

      if (ClosestTileIndex < Spots.Count)
      {
        Resources->ClosestStandingSpotToCursor.Tag = Maybe_Yes;
        Resources->ClosestStandingSpotToCursor.Value = Spots.Start[ClosestTileIndex];
      }
    }
  }

  UiFrameBegin(Ui);         // Clear UI interactions

  DoEngineDebug(Resources); // Do Editor/Debug UI

  // We just drew these values, reset for this frame
#if BONSAI_DEBUG_SYSTEM_API
  GetDebugState()->DrawCallCountLastFrame = 0;
  GetDebugState()->VertexCountLastFrame = 0;
#endif


#if 0
  {
    local_persist window_layout TestWindow = WindowLayout("TestWindow");
    PushWindowStart(Ui, &TestWindow);
      PushTableStart(Ui);
        RenderAndInteractWithThumbnailTexture(Ui, &TestWindow, "test_foo_string", &Editor->NoiseLayer.Preview.Thumbnail);
      PushTableEnd(Ui);
      PushNewRow(Ui); // TODO(Jesse)(bug, ui): If you take this out, the textures overlap one another.  Bug
      PushTableStart(Ui);
        RenderAndInteractWithThumbnailTexture(Ui, &TestWindow, "test_foo_string", &Editor->NoiseLayer.Preview.Thumbnail);
      PushTableEnd(Ui);
    PushWindowEnd(Ui, &TestWindow);
  }
#endif

#if 0
  // NOTE(Jesse): This is fixed
  // @enum_button_group_aligns_poorly_after_toggle_button
  {
    local_persist window_layout TestWindow = WindowLayout("TestWindow");
    PushWindowStart(Ui, &TestWindow);

    ui_id BogusInteractionId = UiId(&TestWindow, "test_window_toggle_interaction", 0u);
      if (ToggleButton(Ui, CSz("v Label"), CSz("v Label"), BogusInteractionId))
      {

        PushTableStart(Ui);
          Button(Ui, CSz("Foo"), BogusInteractionId);
          Button(Ui, CSz("Boo"), BogusInteractionId);
          Button(Ui, CSz("Goo"), BogusInteractionId);
        PushTableEnd(Ui);
      }

    PushWindowEnd(Ui, &TestWindow);
  }
#endif

#if 0
  {
    local_persist window_layout TestWindow = WindowLayout("TestWindow");
    PushWindowStart(Ui, &TestWindow);

      PushTableStart(Ui);
        DoEditorUi(Ui, &TestWindow, &Resources->Ui.ToggleTable, CSz("ToggleTable"));
      PushTableEnd(Ui);

    PushWindowEnd(Ui, &TestWindow);
  }
#endif

#if 0
  // NOTE(Jesse): This is a start on debugging some UI layout issues
  {
    local_persist window_layout TestWindow = WindowLayout("TestWindow");

    PushWindowStart(Ui, &TestWindow);

    PushTableStart(Ui);
      PushColumn(Ui, CSz("foo"));
      PushColumn(Ui, CSz("bar"));
      PushColumn(Ui, CSz("baz"));
      PushNewRow(Ui);

      u32 I = StartColumn(Ui);
        PushTableStart(Ui);
          PushColumn(Ui, CSz("foo"));
        PushTableEnd(Ui);
      EndColumn(Ui, I);

    PushTableEnd(Ui);

    PushWindowEnd(Ui, &TestWindow);
  }
#endif

  b32 Result = True;
  return Result;
}

link_export b32
Bonsai_FrameEnd(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  b32 Result = True;

  if (!AreEqual(&Graphics->Settings, &Graphics->PrevSettings))
  {
    u8_cursor_block_array Bytes = BeginSerialization();
    Serialize(&Bytes, &Graphics->Settings);
    FinalizeSerialization(&Bytes, RUNTIME_SETTINGS__GRAPHICS_SETTINGS_PATH);
  }

  Graphics->GameCamera.Frust.FOV = Graphics->Settings.GameCameraFOV;

  Graphics->PrevSettings = Graphics->Settings;

  return Result;
}

link_export b32
Bonsai_Simulate(engine_resources *Resources)
{
  TIMED_FUNCTION();

  SignalFutex(&Resources->Stdlib.Plat.HighPriorityModeFutex);

  UNPACK_ENGINE_RESOURCES(Resources);

  SimulateEntities(Resources, Plat->dt, V3i(World->VisibleRegionSize), &GpuMap->Buffer, &Graphics->Transparency.GpuBuffer.Buffer, &Plat->HighPriority);
  /* DispatchSimulateParticleSystemJobs(&Plat->HighPriority, EntityTable, World->ChunkDim, &GpuMap->Buffer, Graphics, Plat->dt); */

  UnsignalFutex(&Resources->Stdlib.Plat.HighPriorityModeFutex);

  if (Input->F4.Clicked)
  {
    if (Graphics->Camera == &Graphics->GameCamera)
    {
      /* entity_id Prev = Graphics->DebugCamera.GhostId; */
      /* Graphics->DebugCamera = Graphics->GameCamera; */
      /* Graphics->DebugCamera.GhostId = Prev; */

      Graphics->Camera = &Graphics->DebugCamera;
    }
    else
    {
      Graphics->Camera = &Graphics->GameCamera;
    }
    Camera = Graphics->Camera;
  }

  // NOTE(Jesse): Has to come after the UI happens such that we don't get a
  // frame of camera-jank if the UI captures mouse input
  //
  // The specific case here is that if the camera updates, then the UI draws,
  // and we're in paint-single mode (which captures the input) there's a frame
  // where the camera updates, then freezes, which feels ultra-janky.  Updating
  // the UI, then the camera, avoids this order-of-operations issue.
  //
  // @camera-update-ui-update-frame-jank
  //
  // Unfortunately, this actually re-introduces another order-of-operations issue
  // which I've fixed in the past, which is that the immediate geometry is a
  // frame late. 
  //
  // @immediate-geometry-is-a-frame-late
  //
  // NOTE(Jesse): This has to come before we draw any of the game geometry.
  // Specifically, if it comes after we draw bounding boxes for anything
  // the bounding box lines shift when we move the camera because they're
  // then a frame late.
  //
  // @immediate-geometry-is-a-frame-late
  //
  // UPDATE(Jesse): This bug has been reintroduced because of @camera-update-ui-update-frame-jank
  // More info and a solution documented at : https://github.com/scallyw4g/bonsai/issues/30
  //

  cp CameraTargetP = {};

  entity *CameraGhost = GetEntity(EntityTable, Camera->GhostId);
  if (CameraGhost == 0)
  {
    // Allocate default camera ghost
    Camera->GhostId = GetFreeEntity(EntityTable);
    CameraGhost = GetEntity(EntityTable, Camera->GhostId);
    CameraGhost->Behavior = entity_behavior_flags(CameraGhost->Behavior | EntityBehaviorFlags_DefatulCameraGhostBehavior);
    SpawnEntity(CameraGhost);
  }

  if (CameraGhost) { CameraTargetP = CameraGhost->P; }

  b32 DoPositionDelta = (!UiCapturedMouseInput(Ui) && UiInteractionWasViewport(Ui));

  // NOTE(Jesse): Don't remember what bug I fixed by changing this to DoPositionDelta,
  // but it broke scrolling on laptop trackpads (where nothing is clicked).  Going to
  // remove it until I remember what the bug was ..
  /* b32 DoZoomDelta = DoPositionDelta; */
  b32 DoZoomDelta = UiHoveredMouseInput(Ui) == False;

  v2 MouseDelta = GetMouseDelta(Plat);
  UpdateGameCamera(World, MouseDelta, &Plat->Input, CameraTargetP, Camera, Plat->dt, DoPositionDelta, DoZoomDelta);

  // TODO(Jesse)(correctness, nopush): This should actually be passing the back-buffer resolution??


  m4 ViewMat = ViewMatrix(World->ChunkDim, Camera);
  m4 ProjMat = ProjectionMatrix(Camera, Plat->ScreenDim);

  Resources->Graphics.gBuffer->InverseViewMatrix = Inverse(ViewMat);
  Resources->Graphics.gBuffer->InverseProjectionMatrix = Inverse(ProjMat);
  Resources->Graphics.gBuffer->ViewProjection = ProjMat * ViewMat;

#if BONSAI_DEBUG_SYSTEM_API
  Debug_DoWorldChunkPicking(Resources);
#endif



  // Draw terrain
  PushBonsaiRenderCommandGlTimerStart(&Plat->RenderQ, Graphics->gBuffer->GlTimerObject);

  PushBonsaiRenderCommandSetupShader(&Plat->RenderQ, BonsaiRenderCommand_ShaderId_gBuffer);
  PushBonsaiRenderCommandDrawWorldChunkDrawList(&Plat->RenderQ, &Graphics->MainDrawList, &Graphics->gBuffer->gBufferShader);
  PushBonsaiRenderCommandTeardownShader(&Plat->RenderQ, BonsaiRenderCommand_ShaderId_gBuffer);

  PushBonsaiRenderCommandSetupShader(&Plat->RenderQ, BonsaiRenderCommand_ShaderId_ShadowMap);
  PushBonsaiRenderCommandDrawWorldChunkDrawList(&Plat->RenderQ, &Graphics->ShadowMapDrawList, &Graphics->SG->Shader.Program);
  PushBonsaiRenderCommandTeardownShader(&Plat->RenderQ, BonsaiRenderCommand_ShaderId_ShadowMap);

  PushBonsaiRenderCommandGlTimerEnd(&Plat->RenderQ, Graphics->gBuffer->GlTimerObject);


  b32 Result = True;
  return Result;
}

link_internal void
UpdateKeyLightColor(graphics *Graphics, r32 tDay)
{
  auto SG = Graphics->SG;
  r32 tDaytime = Cos(tDay);
  r32 tPostApex = Sin(tDay);

  lighting_settings *Lighting = &Graphics->Settings.Lighting;

  v3 DawnColor = HSVtoRGB(Lighting->DawnHSV) * Lighting->DawnIntensity;
  v3 SunColor  = HSVtoRGB(Lighting->SunHSV ) * Lighting->SunIntensity;
  v3 DuskColor = HSVtoRGB(Lighting->DuskHSV) * Lighting->DuskIntensity;
  v3 MoonColor = HSVtoRGB(Lighting->MoonHSV) * Lighting->MoonIntensity;

  Lighting->SunP.x = Sin(((Graphics->SunBasis.x*PI32)) + tDay);
  Lighting->SunP.y = Cos(((Graphics->SunBasis.y*PI32))+ tDay);
  Lighting->SunP.z = (1.3f+Cos(((Graphics->SunBasis.z*PI32)) + tDay))/2.f;

  if (tDaytime > 0.f)
  {
    if (tPostApex > 0.f)
    {
      Lighting->CurrentSunColor = Lerp(tDaytime, DuskColor, SunColor);
    }
    else
    {
      Lighting->CurrentSunColor = Lerp(tDaytime, DawnColor, SunColor);
    }
  }
  else
  {
    if (tPostApex > 0.f)
    {
      Lighting->CurrentSunColor = Lerp(Abs(tDaytime), DuskColor, MoonColor);
    }
    else
    {
      Lighting->CurrentSunColor = Lerp(Abs(tDaytime), DawnColor, MoonColor);
    }
  }

  switch (Graphics->Settings.ToneMappingType)
  {
    case ToneMappingType_None:
    case ToneMappingType_Reinhard:
    case ToneMappingType_Exposure:
      { } break;

    case ToneMappingType_AGX:
    case ToneMappingType_AGX_Sepia:
    case ToneMappingType_AGX_Punchy:
    {
      if (LengthSq(Lighting->CurrentSunColor) > 1.f)
      {
        Lighting->CurrentSunColor = Normalize(Lighting->CurrentSunColor);
      }
    } break;
  }
}

link_export b32
Bonsai_Render(engine_resources *Engine)
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Engine);

  if (Graphics->Settings.Lighting.AutoDayNightCycle)
  {
    Graphics->Settings.Lighting.tDay += SafeDivide0(Plat->dt, Graphics->Settings.Lighting.tDaySpeed);
  }
  UpdateKeyLightColor(Graphics, Graphics->Settings.Lighting.tDay);


  /*     BindUniformByName(Shader, "DrawMinorGrid", False); */
  shader_uniform MinorGridUniform =
  {
    .Type = ShaderUniform_U32,
    .U32 = &Global_False,
    .ID = INVALID_SHADER_UNIFORM,
    .Name = "DrawMinorGrid",
  };

  shader_uniform MajorGridUniform =
  {
    .Type = ShaderUniform_U32,
    .U32 = &Global_False,
    .ID = INVALID_SHADER_UNIFORM,
    .Name = "DrawMajorGrid",
  };


  PushBonsaiRenderCommandSetupShader(&Plat->RenderQ, BonsaiRenderCommand_ShaderId_gBuffer);
  PushBonsaiRenderCommandSetShaderUniform(&Plat->RenderQ, MinorGridUniform, &Graphics->gBuffer->gBufferShader, -1);
  PushBonsaiRenderCommandSetShaderUniform(&Plat->RenderQ, MajorGridUniform, &Graphics->gBuffer->gBufferShader, -1);
  PushBonsaiRenderCommandDrawAllEntities(&Plat->RenderQ, &Graphics->gBuffer->gBufferShader);
  PushBonsaiRenderCommandTeardownShader(&Plat->RenderQ, BonsaiRenderCommand_ShaderId_gBuffer);


  PushBonsaiRenderCommandSetupShader(&Plat->RenderQ, BonsaiRenderCommand_ShaderId_ShadowMap);
  PushBonsaiRenderCommandDrawAllEntities(&Plat->RenderQ, &Graphics->SG->Shader.Program);
  PushBonsaiRenderCommandTeardownShader(&Plat->RenderQ, BonsaiRenderCommand_ShaderId_ShadowMap);


  /* DoModalInteraction(Ui, RectMinDim(V2(0), *Ui->ScreenDim)); */


  {
    layout DefaultLayout = {};
    render_state RenderState = {};
    RenderState.Layout = &DefaultLayout;

    SetWindowZDepths(Ui->CommandBuffer);
    FlushCommandBuffer(Ui, &RenderState, Ui->CommandBuffer, &DefaultLayout);
  }


  PushBonsaiRenderCommandDoStuff(&Plat->RenderQ);

  PushBonsaiRenderCommandGlTimerReadValueAndHistogram(&Plat->RenderQ, Graphics->gBuffer->GlTimerObject);

  Engine->Graphics.RenderGate = True;
  while (Engine->Graphics.RenderGate == True) { SleepMs(1); }

  b32 Result = True;
  return Result;
}

struct bonsai_thread_user_data
{
  u64 Magic0 = 0x69;

  world_chunk_freelist SynChunkFreelist;

  u64 Magic1 = 0x420;
};

link_internal void
FreeWorldChunk(world_chunk_freelist *Freelist, world_chunk *Chunk)
{
  Free(Freelist, Chunk);
}

link_internal world_chunk *
GetOrAllocate(world_chunk_freelist *Freelist, v3i WorldP, v3i Dim, v3i DimInChunks, memory_arena *Memory)
{
  Freelist->Memory = Memory;
  world_chunk *Result = GetOrAllocate(Freelist);

  if (Result->Voxels == 0)
  {
    AllocateWorldChunk(Result, WorldP, Dim, DimInChunks, Memory);
  }

  return Result;
}

link_weak void
WorkerThread_ApplicationDefaultImplementation(BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS)
{
  engine_resources *EngineResources = GetEngineResources();
  world *World = EngineResources->World;

  auto RenderQ = &EngineResources->Stdlib.Plat.RenderQ;

  if (Thread->UserData == 0)
  {
    Thread->UserData = Allocate(bonsai_thread_user_data, Thread->PermMemory, 1);
    bonsai_thread_user_data *UserData = Cast(bonsai_thread_user_data*, Thread->UserData);
    *UserData = {};
  }

  bonsai_thread_user_data *UserData = Cast(bonsai_thread_user_data*, Thread->UserData);
  Assert(UserData->Magic0 == 0x69);
  Assert(UserData->Magic1 == 0x420);

  tswitch (Entry)
  {
    InvalidCase(type_work_queue_entry_noop);
    InvalidCase(type_work_queue_entry__align_to_cache_line_helper);
    /* InvalidCase(type_work_queue_entry_update_world_region); */

    // NOTE(Jesse): Render commands should never end up on a general purpose work queue
    InvalidCase(type_work_queue_entry__bonsai_render_command);

    { tmatch(work_queue_entry_async_function_call, Entry, Job)
      DispatchAsyncFunctionCall(Job);
    } break;

    { tmatch(work_queue_entry_init_asset, Entry, Job)
      InitAsset(Job->Asset, Thread);
    } break;

    { tmatch(work_queue_entry_sim_particle_system, Entry, Job)
      SimulateParticleSystem(Job);
    } break;

    { tmatch(work_queue_entry_finalize_noise_values, Entry, Job)
      auto Node = Job->DestNode;
      auto Chunk = Node->Chunk;

      u16 *NoiseValues = Job->NoiseData;
      v3i NoiseDim = Job->NoiseDim;
      Assert(NoiseValues);
      Assert(Chunk);

      world_chunk *DestChunk = Node->Chunk;
      world_chunk *SynChunk = GetOrAllocate(&UserData->SynChunkFreelist, {}, Chunk->Dim + V3i(0, 2, 2), Chunk->DimInChunks, Thread->PermMemory);
      /* SynChunk->Flags = Chunk_Queued; */

      Assert(NoiseDim == V3i(66, 66, 66));
      Assert(SynChunk->Dim == V3i(64, 66, 66));

      v3i WorldBasis = {};
      v3i SrcToDest = {};
      s64 zMin = 0;

      u32 ChunkSum = FinalizeOccupancyMasksFromNoiseValues(SynChunk, WorldBasis, NoiseDim, NoiseValues, SrcToDest, zMin);

      if (ChunkSum && ChunkSum < u32(Volume(SynChunk->Dim)))
      {
        MakeFaceMasks_NoExteriorFaces(SynChunk->Occupancy, SynChunk->xOccupancyBorder, SynChunk->FaceMasks, SynChunk->Voxels, SynChunk->Dim, {}, SynChunk->Dim);

        Assert(SynChunk->Dim.x == 64);
        Assert(SynChunk->Dim.y == 66);
        Assert(SynChunk->Dim.z == 66);

        Assert(DestChunk->FilledCount == 0);
        Assert(DestChunk->Dim.x == 64);
        Assert(DestChunk->Dim.y == 64);
        Assert(DestChunk->Dim.z == 64);
        RangeIterator(z, 64)
        RangeIterator(y, 64)
        {
          u64 Occ = SynChunk->Occupancy[(y+1) + ((z+1)*66)];
          DestChunk->Occupancy[y + (z*64)] = Occ;
          DestChunk->FilledCount += CountBitsSet_Kernighan(Occ);
        }

        Assert(DestChunk->FilledCount <= s32(Volume(DestChunk->Dim)));

        /* FinalizeChunkInitialization(SynChunk); */

        s32 FacesRequired = CountRequiredFacesForMesh_Naieve(SynChunk->FaceMasks, SynChunk->Dim);
        if (FacesRequired)
        {
          /* Info("Chunk faces (%d)", FacesRequired); */
          PushBonsaiRenderCommandAllocateAndMapGpuElementBuffer(
              RenderQ, DataType_v3_u8, u32(FacesRequired*VERTS_PER_FACE), &SynChunk->Mesh, SynChunk, Node);
        }
        else
        {
          if (HasGpuMesh(&DestChunk->Mesh))
          {
            PushDeallocateBuffersCommand(RenderQ, &DestChunk->Mesh.Handles);
          }
          FinalizeNodeInitializaion(Node);
          FreeWorldChunk(&UserData->SynChunkFreelist, SynChunk);
        }
      }
      else
      {
        /* Info("Chunk had no mesh : ChunkSum(%d)", ChunkSum); */
        if (HasGpuMesh(&DestChunk->Mesh))
        {
          PushDeallocateBuffersCommand(RenderQ, &DestChunk->Mesh.Handles);
        }
        FinalizeNodeInitializaion(Node);
        FreeWorldChunk(&UserData->SynChunkFreelist, SynChunk);
      }

      auto Graphics = &EngineResources->Graphics;

      // NOTE(Jesse): The CPU initializer obviously doesn't need to deallocate
      // a PBO, so it sets the PBO handle to -1
      Assert(Job->PBOBuf.PBO != INVALID_PBO_HANDLE);
      PushBonsaiRenderCommandUnmapAndDeallocateBuffer(RenderQ, Job->PBOBuf);
      Assert(Graphics->NoiseFinalizeJobsPending);
      AtomicDecrement(&Graphics->NoiseFinalizeJobsPending);

    } break;

    { tmatch(work_queue_entry_build_chunk_mesh, Entry, Job)

#if 0
      NotImplemented;
#else
      world_chunk               *SynChunk     = Job->SynChunk;

      octree_node               *DestNode     = Job->DestNode;
      world_chunk               *DestChunk    = DestNode->Chunk;
      gpu_mapped_element_buffer GpuMappedBuf  = SynChunk->Mesh;

      /* Assert(HasGpuMesh(&DestChunk->Mesh) == True); */
      Assert(HasGpuMesh(&GpuMappedBuf) == True);

      RebuildWorldChunkMesh(Thread, SynChunk, {}, {}, MeshBit_Lod0, &GpuMappedBuf.Buffer, Thread->TempMemory);

      SynChunk->Mesh = {};

      Assert(GpuMappedBuf.Buffer.At == GpuMappedBuf.Buffer.End);
      FreeWorldChunk(&UserData->SynChunkFreelist, SynChunk);

      PushBonsaiRenderCommandUnmapGpuElementBuffer(RenderQ, GpuMappedBuf, DestNode);
#endif
    } break;

    { tmatch(work_queue_entry_rebuild_mesh, Entry, Job)
      NotImplemented;
    } break;

    { tmatch(work_queue_entry_init_world_chunk, Entry, Job)

#if 1
      NotImplemented;
#else
      world_chunk *Chunk = Job->Chunk;

      counted_string AssetFilename = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);
      native_file AssetFile = OpenFile(AssetFilename, FilePermission_Read);

      if (ChunkIsGarbage(Chunk))
      {
        // NOTE(Jesse): This is an optimization; the engine marks chunks that
        // have moved outside of the visible region as garbage.
        Chunk->Flags = Chunk_Uninitialized;
      }
      else
      {
        s32 Period = 150;
        s32 Amplititude = 10;
        s32 StartingZDepth = 120;
        v3 Color = RGB_GRASS_GREEN;

        Assert(Chunk->Dim == World->ChunkDim);
        u32 Octaves = 1;
        /* InitializeChunkWithNoise( Terrain_Perlin2D, Thread, Chunk, Chunk->Dim, &AssetFile, V3(Period), Amplititude, StartingZDepth, Color, MeshBit_Lod0, ChunkInitFlag_ComputeStandingSpots, &Octaves); */
      }
#endif

    } break;

    { tmatch(work_queue_entry_copy_buffer_ref, Entry, Job)
      DoCopyJob(Job, &EngineResources->geo_u3d_MeshFreelist, Thread->PermMemory);
    } break;

    { tmatch(work_queue_entry_copy_buffer_set, Entry, Job)
      TIMED_BLOCK("Copy Set");
      volatile work_queue_entry_copy_buffer_set *CopySet = SafeAccess(work_queue_entry_copy_buffer_set, Entry);
      for (u32 CopyIndex = 0; CopyIndex < CopySet->Count; ++CopyIndex)
      {
        work_queue_entry_copy_buffer_ref *CopyJob = (work_queue_entry_copy_buffer_ref *)CopySet->CopyTargets + CopyIndex;
        DoCopyJob(CopyJob, &EngineResources->geo_u3d_MeshFreelist, Thread->PermMemory);
      }
      END_BLOCK("Copy Set");
    } break;
  }
}

link_internal void
RequestGameLibReload(engine_resources *Engine, file_traversal_node FileNode)
{
  Engine->RequestedGameLibReloadNode = FileNode;
}
