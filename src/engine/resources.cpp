#if PLATFORM_WINDOW_IMPLEMENTATIONS
link_internal void
RenderLoop(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  // Map immediate GPU buffers for first frame
  MapGpuElementBuffer(GpuMap);
  MapGpuElementBuffer(&Graphics->Transparency.GpuBuffer);

  os *Os         = &Engine->Stdlib.Os;
  /* platform *Plat = &Engine->Stdlib.Plat; */
  while (true)
  {
    v2 LastMouseP = Plat->MouseP;
    while ( ProcessOsMessages(Os, Plat) );
    Plat->MouseDP = LastMouseP - Plat->MouseP;
    Assert(Plat->ScreenDim.x > 0);
    Assert(Plat->ScreenDim.y > 0);

    while (Graphics->RenderGate == False) { SleepMs(1); }

    ClearFramebuffers(Graphics, &Engine->RTTGroup);

    UiFrameEnd(&Engine->Ui);

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

    /* DrawWorldToGBuffer(Engine, GetApplicationResolution(&Engine->Settings)); */
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
    DrawStuffToGBufferTextures(Engine, GetApplicationResolution(&Engine->Settings));

    DrawWorldAndEntitiesToShadowMap(GetShadowMapResolution(&Engine->Settings), Engine);

    // TODO(Jesse): Move into engine debug
    DebugHighlightWorldChunkBasedOnState(Graphics, EngineDebug->PickedChunk, &GpuMap->Buffer);

    AssertNoGlErrors;

    Ensure( FlushBuffersToCard(GpuMap) );
    /* Ensure( FlushBuffersToCard(&Graphics->Transparency.GpuBuffer)); */

    if (GpuMap->Buffer.At)
    {
      RenderImmediateGeometryToGBuffer(GetApplicationResolution(&Engine->Settings), GpuMap, Graphics);
      RenderImmediateGeometryToShadowMap(GpuMap, Graphics);
    }

    Clear(&GpuMap->Buffer);

    // NOTE(Jesse): I observed the AO lagging a frame behind if this is re-ordered
    // after the transparency/luminance textures.  I have literally 0 ideas as to
    // why that would be, but here we are.
    if (Graphics->Settings.UseSsao) { RenderAoTexture(GetApplicationResolution(&Engine->Settings), AoGroup); }

    {
      RenderTransparencyBuffers(GetApplicationResolution(&Engine->Settings), &Graphics->Settings, &Graphics->Transparency);
      RenderLuminanceTexture(GetApplicationResolution(&Engine->Settings), GpuMap, Lighting, Graphics);
    }

    if (Graphics->Settings.UseLightingBloom) { RunBloomRenderPass(Graphics); }
    /* if (Graphics->Settings.UseLightingBloom) { GaussianBlurTexture(&Graphics->Gaussian, &Graphics->Lighting.BloomTex, &Graphics->Lighting.BloomFBO); } */


    CompositeAndDisplay(Plat, Graphics);


    GpuMap->Buffer.At = 0;

    GL.DisableVertexAttribArray(0);
    GL.DisableVertexAttribArray(1);
    GL.DisableVertexAttribArray(2);
    GL.DisableVertexAttribArray(3);

    BonsaiSwapBuffers(&Engine->Stdlib.Os);

    // Map immediate GPU buffers for next frame
    MapGpuElementBuffer(GpuMap);
    MapGpuElementBuffer(&Graphics->Transparency.GpuBuffer);

    Graphics->RenderGate = False;
  }

}

link_internal THREAD_MAIN_RETURN
RenderMain(void *vThreadStartupParams)
{
  b32 InitResult = True;
  thread_startup_params *ThreadParams = Cast(thread_startup_params*, vThreadStartupParams);

  Assert(ThreadParams->ThreadIndex > 0);
  SetThreadLocal_ThreadIndex(ThreadParams->ThreadIndex);

  engine_resources *Engine = GetEngineResources();
  os *Os = &Engine->Stdlib.Os;
  platform *Plat = &Engine->Stdlib.Plat;

  s32 VSyncFrames = 0;
  InitResult &= OpenAndInitializeWindow(Os, Plat, VSyncFrames);

  if (InitResult) { InitResult &= InitializeOpenglFunctions(); }

  if (InitResult) { InitResult &= GraphicsInit(&Engine->Graphics, &Engine->Settings, AllocateArena()); }

  if (InitResult)
  {
    memory_arena *UiMemory = AllocateArena();
    InitRenderer2D(&Engine->Ui, &Engine->Heap, UiMemory, &Plat->MouseP, &Plat->MouseDP, &Plat->ScreenDim, &Plat->Input);

    bitmap_block_array Bitmaps = { .Memory = GetTranArena() };
    LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/64x64_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
    LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/44x44_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
    Engine->Ui.SpriteTextureArray = CreateTextureArrayFromBitmapBlockArray(&Bitmaps, V2i(64,64));
  }

  FullBarrier;

  Engine->Graphics.Initialized = True;

  if (InitResult)
  {
    RenderLoop(Engine);
  }

  return 0;
}
#endif

link_internal b32
InitEngineResources(engine_resources *Engine)
{
  b32 Result = True;

  platform *Plat = &Engine->Stdlib.Plat;

  memory_arena *WorldAndEntityArena = AllocateArena(Megabytes(256));
  DEBUG_REGISTER_ARENA(WorldAndEntityArena, 0);

  Engine->GameMemory = AllocateArena();
  Engine->WorldUpdateMemory = AllocateArena();

  Engine->Heap                    = InitHeap(Gigabytes(2)); // TODO(Jesse): Is this actually used?
  Engine->AssetSystem.AssetMemory = InitHeap(Gigabytes(1));

  Engine->World = Allocate(world, WorldAndEntityArena, 1);
  if (!Engine->World) { Error("Allocating World"); Result = False; }

  Assert(Global_ShaderHeaderCode.Start == 0);
  LoadGlobalShaderHeaderCode(Engine->Settings.Graphics.ShaderLanguage);

#if PLATFORM_WINDOW_IMPLEMENTATIONS
  /* PlatformCreateThread( RenderMain, Cast(void*, Engine), INVALID_THREAD_LOCAL_THREAD_INDEX ); */
#endif

  Engine->EntityTable = AllocateEntityTable(WorldAndEntityArena, TOTAL_ENTITY_COUNT);

  return Result;
}

link_internal b32
InitEngineDebug(engine_debug *Debug)
{
  b32 Result = True;

  Debug->Memory = AllocateArena();

  Debug->Textures.Memory = Debug->Memory;

  /* { */
  /*   Debug->WorldEditDebugThumb.Texture = MakeTexture_RGB(V2i(256), 0, CSz("WorldEditDebugTexture")); */
  /*   StandardCamera(&Debug->WorldEditDebugThumb.Camera, 10000.f, 1000.f, 30.f); */
  /*   AllocateMesh(&Debug->WorldEditDebugMesh,  u32(Kilobytes(64*16)), Debug->Memory); */
  /* } */

  return Result;
}

enum hard_reset_flags
{
  HardResetFlag_None = 0,
  HardResetFlag_NoResetCamera = (1 << 0),
};


link_internal void
AssertWorkerThreadsSuspended(engine_resources *Engine)
{
  Assert(Engine->Stdlib.Plat.WorkerThreadsSuspendFutex.SignalValue != FUTEX_UNSIGNALLED_VALUE);
  Assert(Engine->Stdlib.Plat.WorkerThreadsSuspendFutex.ThreadsWaiting == GetWorkerThreadCount());
}

link_internal void
CancelAllWorkQueueJobs(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  AssertWorkerThreadsSuspended(Engine);

  CancelAllWorkQueueJobs(Plat, &Plat->HighPriority);
  CancelAllWorkQueueJobs(Plat, &Plat->LowPriority);
}

link_internal void
HardResetAssets(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  AssertWorkerThreadsSuspended(Engine);

  DeinitHeap(&Engine->AssetSystem.AssetMemory);

  Engine->AssetSystem = {};
  Engine->AssetSystem.AssetMemory = InitHeap(Gigabytes(1));
}

// NOTE(Jesse): This function soft-resets the engine to a state similar to that
// at which it was when the game init routine was called.  This is useful when
// resetting the game state.  For a more invasive 
link_internal void
SoftResetEngine(engine_resources *Engine, hard_reset_flags Flags = HardResetFlag_None)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  CancelAllWorkQueueJobs(Engine);

  u32 ChunksFreed = 0;
  RangeIterator(HashIndex, s32(World->HashSize))
  {
    if (world_chunk *Chunk = World->ChunkHash[HashIndex])
    {
      Chunk->Flags = Chunk_VoxelsInitialized;
      FreeWorldChunk(World, Chunk, &Engine->MeshFreelist);
      World->ChunkHash[HashIndex] = 0;
      ++ChunksFreed;
    }
  }

  RangeIterator_t(u32, EntityIndex, TOTAL_ENTITY_COUNT)
  {
    if ( (Flags&HardResetFlag_NoResetCamera) && Graphics->GameCamera.GhostId.Index == EntityIndex ) { continue; }
    Unspawn(EntityTable[EntityIndex]);
  }

  HardResetAssets(Engine);
}

link_internal void
HardResetEngine(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  CancelAllWorkQueueJobs(Engine);

  RangeIterator_t(u32, EntityIndex, TOTAL_ENTITY_COUNT)
  {
    Unspawn(EntityTable[EntityIndex]);
  }

  VaporizeArena(Engine->GameMemory);
  Engine->GameMemory = AllocateArena();

  VaporizeArena(Engine->World->ChunkMemory);
  Engine->World->ChunkMemory = AllocateArena();

  HardResetEditor(&Engine->Editor);

  // TODO(Jesse)(leak): This leaks the texture handles; make a HardResetEngineDebug()
  VaporizeArena(Engine->EngineDebug.Memory);
  Engine->EngineDebug = {};
  Engine->EngineDebug.Memory = AllocateArena();

  HardResetAssets(Engine);
}

