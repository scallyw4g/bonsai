link_internal void
DeallocateAndClearWorldChunk(engine_resources *Engine, world_chunk *Chunk)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Assert(Chunk->IsOnFreelist == False);

  /* Assert( (Chunk->Flags & Chunk_Queued) == 0); */
  /* Assert( Chunk->Flags & (Chunk_Deallocate|Chunk_VoxelsInitialized)); */

  if (HasGpuMesh(Chunk))
  {
    GpuHeapDeallocate(&Engine->Graphics.GpuHeap, &Chunk->Mesh);
  }

  ClearWorldChunk(Chunk);

  /* Assert(Chunk->Flags == 0); */
  Assert(Chunk->Next == 0);

  FullBarrier;
}

link_internal b32
InitEngineResources(engine_resources *Engine)
{
  b32 Result = True;

  platform *Plat = &Engine->Stdlib.Plat;

  memory_arena *WorldAndEntityArena = AllocateArena(Megabytes(256));
  DEBUG_REGISTER_ARENA(WorldAndEntityArena, 0);

  Engine->ChunkCompletionCallbacks.Memory = WorldAndEntityArena;

  Engine->GameMemory        = AllocateArena();
  Engine->WorldUpdateMemory = AllocateArena();

  Engine->Heap                    = InitHeap(Gigabytes(2));       // TODO(Jesse): Is this actually used?
  Engine->AssetSystem.AssetMemory = InitHeap(Gigabytes(1), True); // NOTE(Jesse): Asset system needs to be able to allocate from the render thread.

  Engine->World = Allocate(world, WorldAndEntityArena, 1);
  if (!Engine->World) { Error("Allocating World"); Result = False; }

  /* Assert(Global_ShaderHeaderCode.Start == 0); */
  /* LoadGlobalShaderHeaderCode(Engine->Settings.Graphics.ShaderLanguage); */

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
HardResetAssets(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  DeinitHeap(&Engine->AssetSystem.AssetMemory);

  Engine->AssetSystem = {};
  Engine->AssetSystem.AssetMemory = InitHeap(Gigabytes(1), True);
}

link_internal void
HardResetWorld(engine_resources *Engine)
{
  world *World = Engine->World;

  // NOTE(Jesse): We have to walk the octree to free all the GPU buffers :/
  // We also can't do it from in here because the render thread is stopped and
  // we run the risk of filling the render queue before we've freed the entire
  // world.  Instead, we're going to assert here and put the onus on the caller
  // to free the octree before the render thread has stopped.
  /* FreeOctreeChildren(Engine, &World->Root); */
  Assert(World->Root.Type == OctreeNodeType_Leaf);
  Assert(World->Root.Children[0] == 0);
  Assert(World->Root.Children[1] == 0);
  Assert(World->Root.Children[2] == 0);
  Assert(World->Root.Children[3] == 0);
  Assert(World->Root.Children[4] == 0);
  Assert(World->Root.Children[5] == 0);
  Assert(World->Root.Children[6] == 0);
  Assert(World->Root.Children[7] == 0);

  RangeIterator_t(u32, JobIndex, TERRAIN_JOB_COUNT)
  {
    Engine->Graphics.TerrainJobContexts.Start[JobIndex].Job = 0;
  }

  VaporizeArena(World->ChunkMemory);
  VaporizeArena(World->OctreeMemory);

  *World = {};

  // The game init function is responsible for allocating the world .. we just
  // clear it here.
  /* v3i Center             = World->Center; */
  /* auto VisibleRegionSize = World->VisibleRegionSize; */
  /* AllocateWorld(World, Center, ChunkDim, VisibleRegionSize); */
}

link_internal void
SoftResetGraphics(graphics *Graphics)
{
  Graphics->MainDrawList.ElementCount = 0;;
  Graphics->ShadowMapDrawList.ElementCount = 0;

  Graphics->TerrainGenRC.ReshapeFunc = {};
}

link_internal void
SoftResetEngine( engine_resources *Engine )
{
  auto Plat = &Engine->Stdlib.Plat;

  InitQueue(&Plat->HighPriority);
  InitQueue(&Plat->LowPriority);
  InitQueue(&Plat->HiRenderQ);
  InitQueue(&Plat->LoRenderQ);
  InitQueue(&Plat->TransferQ);

  FreeOctreeChildren(Engine, &Engine->World->Root);

  world *World = Engine->World;
  v3i Center             = World->Center;
  auto VisibleRegionSize = World->VisibleRegionSize;

  HardResetWorld(Engine);
  AllocateWorld(World, Center, VisibleRegionSize);

  /* ApplyEditBufferToOctree(Engine, &Engine->Editor.Edits); */
  IterateOver(&Engine->Editor.Edits, Edit, EditIndex)
  {
    if (BitfieldIsSet(Edit->Flags, WorldEditFlag_Tombstone)) { continue; }
    ApplyEditToOctree(Engine, Edit, GetTranArena());
  }
  /* HardResetEditor(&Engine->Editor); */

  /* UnsignalFutex(&Plat->WorkerThreadsSuspendFutex); */
}


link_internal void
HotReloadExecutableCode(engine_resources *Engine)
{
  application_api *GameApi   = &Engine->Stdlib.AppApi;
  engine_api      *EngineApi = &Engine->EngineApi;

  if (GameApi->GameDeInit)
  {
    Assert(ThreadLocal_ThreadIndex == 0);
    thread_local_state *MainThread = GetThreadLocalState(ThreadLocal_ThreadIndex);
    GameApi->GameDeInit(Engine, MainThread);
  }

  CloseLibrary(Engine->GameLib);
  Engine->GameLib = OpenLibrary(Engine->GameLibFile.File.Path);

  *EngineApi = {};
  *GameApi = {};

  Ensure(InitializeEngineApi(EngineApi, Engine->GameLib));
  Ensure(InitializeApplicationApi(GameApi, Engine->GameLib));

  // Hook up global pointers
  Ensure( EngineApi->OnLibraryLoad(Engine) );
}

link_internal void
HardResetEngine( engine_resources *Engine,
                 hard_reset_flags  Flags = HardResetFlag_None)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  auto Stdlib = &Engine->Stdlib;

  Info("Hard Reset Begin");

  ShutdownWorkerThreads(Plat);
  /* FlushAllQueuesAndSuspendWorkers(Plat); */
  /* CancelAllWorkQueueJobs(Engine); */

  InitQueue(&Plat->HighPriority);
  InitQueue(&Plat->LowPriority);
  InitQueue(&Plat->HiRenderQ);
  InitQueue(&Plat->LoRenderQ);
  InitQueue(&Plat->TransferQ);


  // This should probably go with the game lib reload since that's why it's here
  //
  DuplicateMetaTableNameStrings(Engine);

  b32 ResetCamera = (Flags & HardResetFlag_NoResetCamera) == 0 ;
  if (ResetCamera)
  {
    StandardCamera(&Graphics->GameCamera);
    StandardCamera(&Graphics->DebugCamera);
    Graphics->Camera = &Graphics->GameCamera;
  }

  RangeIterator_t(u32, EntityIndex, TOTAL_ENTITY_COUNT)
  {
    // Skip camera entities if we asked to not reset the camera
    if ( ResetCamera == False &&
           (Graphics->DebugCamera.GhostId.Index == EntityIndex ||
            Graphics->GameCamera.GhostId.Index == EntityIndex   ) )
    { continue; }

    Unspawn(EntityTable[EntityIndex]);
  }

  HardResetEditor(&Engine->Editor);

  HardResetWorld(Engine);

  SoftResetGraphics(Graphics);

  // TODO(Jesse)(leak): This leaks the texture handles; make a HardResetEngineDebug()
  Leak("Leaking EngineDebug texture handles");
  VaporizeArena(Engine->EngineDebug.Memory);
  Engine->EngineDebug = {};
  Engine->EngineDebug.Memory = AllocateArena();

  HardResetAssets(Engine);

  HotReloadExecutableCode(Engine);

  application_api *GameApi   = &Stdlib->AppApi;

  Assert(ThreadLocal_ThreadIndex == 0);
  thread_local_state *MainThread = GetThreadLocalState(ThreadLocal_ThreadIndex);

  {

    auto OldGameMemory = Engine->GameMemory;
    Engine->GameMemory = AllocateArena();

    if (GameApi->GameInit)
    {
      Engine->GameState = GameApi->GameInit(Engine, MainThread);
    }

    VaporizeArena(OldGameMemory);
  }

  /* UnsignalFutex(&Plat->WorkerThreadsSuspendFutex); */
  {
    SignalFutex(&Plat->WorkerThreadsReady);
    LaunchWorkerThreads(Plat, GameApi, &Stdlib->WorkerThreadProcs);
    WaitForWorkers(&Plat->WorkerThreadsReady);
    UnsignalFutex(&Plat->WorkerThreadsReady);
  }


  Info("Hard Reset End");
}

