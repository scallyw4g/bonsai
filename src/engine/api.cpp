
link_export b32
Bonsai_OnLibraryLoad(engine_resources *Resources)
{
  // We should only ever call this from the main thread, and this sets our
  // thread index such that the game doesn't have to worry about doing it.
  if (ThreadLocal_ThreadIndex == -1) { SetThreadLocal_ThreadIndex(0); }
  else { Assert(ThreadLocal_ThreadIndex == 0); }

#if BONSAI_DEBUG_SYSTEM_API
  Global_DebugStatePointer = Resources->DebugState;
#endif

  Global_ThreadStates = Resources->Stdlib.ThreadStates;
  Global_EngineResources = Resources;

  /* Initialize_Global_UpdateWorldCallbackTable(); */

  b32 Result = InitializeOpenglFunctions();
  return Result;
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

link_export b32
Bonsai_FrameBegin(engine_resources *Resources)
{
  TIMED_FUNCTION();

  DoWorldChunkStuff();

  // Must come before we update the frame index
  CollectUnusedChunks(Resources, &Resources->MeshFreelist, Resources->World->VisibleRegion);

  Resources->FrameIndex += 1;

  // Must come before UNPACK_ENGINE_RESOURCES such that we unpack the correct GpuMap
  Resources->Graphics->GpuBufferWriteIndex = 0;
  Resources->Graphics->GpuBufferWriteIndex = (Resources->FrameIndex) % 2;

  UNPACK_ENGINE_RESOURCES(Resources);

  /* if (Input->F12.Pressed) { EngineDebug->TriggerRuntimeBreak = True; } */

  World->ChunkHash = CurrentWorldHashtable(Resources);

  ClearFramebuffers(Graphics, &Resources->RTTGroup);

  MapGpuElementBuffer(GpuMap);
  MapGpuElementBuffer(&Graphics->Transparency.GpuBuffer);

  if (GetEngineDebug()->DrawWorldAxies)
  {
    untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL*3);
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(10000, 0, 0), RED,   0.35f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.35f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 0, 10000), BLUE,  0.35f );
  }


  Graphics->Lighting.Lights.Count = 0;

  // NOTE(Jesse): This is a little weird in that we're using the ray from last
  // frame, but we have to update the camera after we do the UI draw in case
  // the UI captures the input
  //
  // @camera-update-ui-update-frame-jank
  if (UiHoveredMouseInput(Ui))
  {
    Resources->MaybeMouseRay   = {};
    Resources->MousedOverVoxel = {};
    Resources->HoverEntity     = {};
  }
  else
  {
    Resources->MaybeMouseRay   = ComputeRayFromCursor(Resources, &gBuffer->ViewProjection, Camera, World->ChunkDim);
    Resources->MousedOverVoxel = MousePickVoxel(Resources);
    Resources->HoverEntity     = GetClosestEntityIntersectingRay(World, EntityTable, &Resources->MaybeMouseRay.Ray);
  }

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
Bonsai_FrameEnd(engine_resources *Resources)
{
  UiFrameEnd(&Resources->Ui);

  b32 Result = True;
  return Result;
}

link_export b32
Bonsai_Simulate(engine_resources *Resources)
{
  TIMED_FUNCTION();

  SignalFutex(&Resources->Stdlib.Plat.HighPriorityModeFutex);

  UNPACK_ENGINE_RESOURCES(Resources);

  SimulateEntities(Resources, Plat->dt, World->VisibleRegion, &GpuMap->Buffer, &Graphics->Transparency.GpuBuffer.Buffer, &Plat->HighPriority);
  /* DispatchSimulateParticleSystemJobs(&Plat->HighPriority, EntityTable, World->ChunkDim, &GpuMap->Buffer, Graphics, Plat->dt); */

  UnsignalFutex(&Resources->Stdlib.Plat.HighPriorityModeFutex);

  if (Input->F4.Clicked)
  {
    if (Graphics->Camera == &Graphics->GameCamera)
    {
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
  input *InputForCamera = &Plat->Input;

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
  UpdateGameCamera(World, MouseDelta, InputForCamera, CameraTargetP, Camera, Plat->dt, DoPositionDelta, DoZoomDelta);

  // TODO(Jesse)(correctness, nopush): This should actually be passing the back-buffer resolution??


  m4 ViewMat = ViewMatrix(World->ChunkDim, Camera);
  m4 ProjMat = ProjectionMatrix(Camera, Plat->ScreenDim);

  Resources->Graphics->gBuffer->InverseViewMatrix = Inverse(ViewMat);
  Resources->Graphics->gBuffer->InverseProjectionMatrix = Inverse(ProjMat);
  Resources->Graphics->gBuffer->ViewProjection = ProjMat * ViewMat;

#if BONSAI_DEBUG_SYSTEM_API
  Debug_DoWorldChunkPicking(Resources);
#endif

  b32 Result = True;
  return Result;
}

link_internal void
DoDayNightCycle(graphics *Graphics, r32 tDay)
{
  auto SG = Graphics->SG;
  r32 tDaytime = Cos(tDay);
  r32 tPostApex = Sin(tDay);

  lighting_settings *Lighting = &Graphics->Settings.Lighting;

  v3 DawnColor = Normalize(Lighting->DawnColor) * Lighting->DawnIntensity;
  v3 SunColor  = Normalize(Lighting->SunColor ) * Lighting->SunIntensity;
  v3 DuskColor = Normalize(Lighting->DuskColor) * Lighting->DuskIntensity;
  v3 MoonColor = Normalize(Lighting->MoonColor) * Lighting->MoonIntensity;

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
Bonsai_Render(engine_resources *Resources)
{
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Resources);

  ao_render_group     *AoGroup = Graphics->AoGroup;
  shadow_render_group *SG      = Graphics->SG;

  if (Graphics->Settings.Lighting.AutoDayNightCycle)
  {
    Graphics->Settings.Lighting.tDay += Plat->dt/18.0f;
  }
  DoDayNightCycle(Graphics, Graphics->Settings.Lighting.tDay);

#if 0
  NotImplemented;
#else
  entity *CameraGhost = GetEntity(EntityTable, Camera->GhostId);
  if (CameraGhost)
  {
    v3 CameraTargetSimP = GetSimSpaceP(World, CameraGhost);
    Graphics->Settings.OffsetOfWorldCenterToGrid.x = Mod(CameraTargetSimP.x, Graphics->Settings.MajorGridDim);
    Graphics->Settings.OffsetOfWorldCenterToGrid.y = Mod(CameraTargetSimP.y, Graphics->Settings.MajorGridDim);
    Graphics->Settings.OffsetOfWorldCenterToGrid.z = Mod(CameraTargetSimP.z, Graphics->Settings.MajorGridDim);
  }
#endif

  EngineDebug->Render.BytesSolidGeoLastFrame = GpuMap->Buffer.At;
  EngineDebug->Render.BytesTransGeoLastFrame = Graphics->Transparency.GpuBuffer.Buffer.At;

  /* DrawWorldToGBuffer(Resources, GetApplicationResolution(&Resources->Settings)); */
  /* DrawEditorChunkPreviewToGBuffer(); */

#if 1
  s32 ColorCount = s32(AtElements(&Graphics->ColorPalette));
  if (ColorCount != Graphics->ColorPaletteTexture.Dim.x)
  {
    if (Graphics->ColorPaletteTexture.ID) { DeleteTexture(&Graphics->ColorPaletteTexture); }
    Graphics->ColorPaletteTexture =
      MakeTexture_RGB( V2i(ColorCount, 1), Graphics->ColorPalette.Start, CSz("ColorPalette"));
  }
#endif

  // Editor preview, World, Entities
  DrawStuffToGBufferTextures(Resources, GetApplicationResolution(&Resources->Settings));

  DrawWorldAndEntitiesToShadowMap(GetShadowMapResolution(&Resources->Settings), Resources);

  // TODO(Jesse): Move into engine debug
  DebugHighlightWorldChunkBasedOnState(Graphics, EngineDebug->PickedChunk, &GpuMap->Buffer);

  Ensure( FlushBuffersToCard(GpuMap) );
  /* Ensure( FlushBuffersToCard(&Graphics->Transparency.GpuBuffer)); */

  if (GpuMap->Buffer.At)
  {
    RenderImmediateGeometryToGBuffer(GetApplicationResolution(&Resources->Settings), GpuMap, Graphics);
    RenderImmediateGeometryToShadowMap(GpuMap, Graphics);
  }

  Clear(&GpuMap->Buffer);

  // NOTE(Jesse): I observed the AO lagging a frame behind if this is re-ordered
  // after the transparency/luminance textures.  I have literally 0 ideas as to
  // why that would be, but here we are.
  if (Graphics->Settings.UseSsao) { RenderAoTexture(GetApplicationResolution(&Resources->Settings), AoGroup); }

  {
    RenderTransparencyBuffers(GetApplicationResolution(&Resources->Settings), &Graphics->Settings, &Graphics->Transparency);
    RenderLuminanceTexture(GetApplicationResolution(&Resources->Settings), GpuMap, Lighting, Graphics);
  }

  if (Graphics->Settings.UseLightingBloom) { RunBloomRenderPass(Graphics); }
  /* if (Graphics->Settings.UseLightingBloom) { GaussianBlurTexture(&Graphics->Gaussian, &Graphics->Lighting.BloomTex, &Graphics->Lighting.BloomFBO); } */

  CompositeAndDisplay(Plat, Graphics);


  GpuMap->Buffer.At = 0;
  GL.DisableVertexAttribArray(0);
  GL.DisableVertexAttribArray(1);
  GL.DisableVertexAttribArray(2);
  GL.DisableVertexAttribArray(3);


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

      if (Job->Flags & ChunkInitFlag_ComputeStandingSpots)
      {
        ComputeStandingSpots( Chunk->Dim,
                              Chunk->Voxels,
                              {},

                              {},
                              Global_TileDim,

                              Chunk->Dim,
                              0,
                              &Chunk->StandingSpots,
                              /* memory_arena *PermMemory, */
                              Thread->TempMemory );
      }

      /* UnsetBitfield(chunk_flag, Chunk->Flags, Chunk_Queued); */
      Chunk->Flags = chunk_flag(Chunk->Flags & ~Chunk_Queued);
      /* Chunk->Flags = chunk_flag(Chunk->Flags & ~Chunk_MeshUploadedToGpu); */
    } break;

    case type_work_queue_entry_init_world_chunk:
    {
      work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;

      counted_string AssetFilename = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);
      native_file AssetFile = OpenFile(AssetFilename, FilePermission_Read);

      if (ChunkIsGarbage(Chunk))
      {
        // NOTE(Jesse): This is an optimization; the engine marks chunks that // have moved outside of the visible region as garbage.
        Chunk->Flags = Chunk_Uninitialized;
      }
      else
      {
        s32 Period = 150;
        s32 Amplititude = 10;
        s32 StartingZDepth = 0;
        u16 Color = GRASS_GREEN;

        Assert(Chunk->Dim == World->ChunkDim);
        u32 Octaves = 1;
        InitializeChunkWithNoise( Terrain_Perlin2D, Thread, Chunk, Chunk->Dim, &AssetFile, V3(Period), Amplititude, StartingZDepth, Color, MeshBit_Lod0, ChunkInitFlag_ComputeStandingSpots, &Octaves);
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

link_internal void
RequestGameLibReload(engine_resources *Engine, file_traversal_node FileNode)
{
  Engine->RequestedGameLibReloadNode = FileNode;
}
