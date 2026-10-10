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
AssertWorkerThreadsSuspended(engine_resources *Engine)
{
  Assert(Engine->Stdlib.Plat.WorkerThreadsSuspendFutex.SignalValue != FUTEX_UNSIGNALLED_VALUE);
  Assert(Engine->Stdlib.Plat.WorkerThreadsSuspendFutex.ThreadsWaiting == GetWorkerThreadCount());
}

link_internal b32
WorkQueuesAreEmpty(platform *Plat)
{
  return QueueIsEmpty(&Plat->HighPriority) &&
         QueueIsEmpty(&Plat->LowPriority) &&
         QueueIsEmpty(&Plat->HiRenderQ) &&
         QueueIsEmpty(&Plat->LoRenderQ);
}

link_internal void
DrainAndSuspendWorkers(engine_resources *Engine)
{
  auto Plat = &Engine->Stdlib.Plat;
  AssertWorkerThreadsSuspended(Engine);
  Assert(FutexNotSignaled(&Plat->HighPriorityModeFutex));

  while (!WorkQueuesAreEmpty(Plat))
  {
    UnsignalFutex(&Plat->WorkerThreadsSuspendFutex);
    WaitForWorkerThreads(&Plat->WorkerThreadsSuspendFutex.ThreadsWaiting);
    while (!WorkQueuesAreEmpty(Plat)) { SleepMs(1); }
    SignalAndWaitForWorkers(&Plat->WorkerThreadsSuspendFutex);
    // An in-flight task or the renderer's final drain can enqueue another CPU task.
  }
}

link_internal void
PrepareForWorldReset(engine_resources *Engine)
{
  DrainAndSuspendWorkers(Engine);
  Assert(Engine->Graphics.NoiseFinalizeJobsPending == 0);
  Assert(Engine->Graphics.TotalChunkJobsActive == 0);

  // Finish readback/mesh job chains before freeing any of their world data.
  world *World = Engine->World;
  FreeOctreeChildren(Engine, &World->Root);
  while (octree_node *Node = World->OctreeNodeDeferFreelist.First)
  {
    Assert((Node->Flags & Chunk_Queued) == 0);
    World->OctreeNodeDeferFreelist.First = Node->Next;
    FreeWorldChunk(Engine, Node->Chunk);
  }
  if (World->Root.Chunk)
  {
    FreeWorldChunk(Engine, World->Root.Chunk);
    World->Root.Chunk = 0;
  }
}

link_internal void
HardResetAssets(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  AssertWorkerThreadsSuspended(Engine);

  DeinitHeap(&Engine->AssetSystem.AssetMemory);

  Engine->AssetSystem = {};
  Engine->AssetSystem.AssetMemory = InitHeap(Gigabytes(1), True);
}

// NOTE(Jesse): This function soft-resets the engine to a state similar to that
// at which it was when the game init routine was called.  This is useful when
// resetting the game state.  For a more invasive 
#if 0
link_internal void
SoftResetEngine(engine_resources *Engine, hard_reset_flags Flags = HardResetFlag_None)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  PrepareForWorldReset(Engine);

  FreeOctreeChildren(Engine, &World->Root);
  if (World->Root.Chunk) { FreeWorldChunk(Engine, World->Root.Chunk); }
  InitOctreeNode(World, &World->Root, {}, World->VisibleRegion, {});

  RangeIterator_t(u32, EntityIndex, TOTAL_ENTITY_COUNT)
  {
    if ( (Flags&HardResetFlag_NoResetCamera) && Graphics->GameCamera.GhostId.Index == EntityIndex ) { continue; }
    Unspawn(EntityTable[EntityIndex]);
  }

  HardResetAssets(Engine);
}
#endif



/* link_internal void */
/* SoftResetWorld(engine_resources *Engine) */
/* { */
/*   world *World = Engine->World; */

/*   MergeOctreeChildren(Engine, &World->Root); */

/*   if (World->Root.Chunk) */
/*   { */
/*     FreeWorldChunk(Engine, World->Root.Chunk); */
/*   } */
/*   World->Root = {}; */

/*   InitOctreeNode(World, &World->Root, {}, World->VisibleRegion); */
/*   World->Root.Chunk = AllocateWorldChunk( {}, World->ChunkDim, World->VisibleRegion, World->ChunkMemory); */

/* } */

link_internal void
HardResetWorld(engine_resources *Engine)
{
  world *World = Engine->World;

  // Job chains and GPU heap allocations must be retired before releasing world memory.
  Assert(World->Root.Type == OctreeNodeType_Leaf);
  Assert(World->Root.Children[0] == 0);
  Assert(World->Root.Children[1] == 0);
  Assert(World->Root.Children[2] == 0);
  Assert(World->Root.Children[3] == 0);
  Assert(World->Root.Children[4] == 0);
  Assert(World->Root.Children[5] == 0);
  Assert(World->Root.Children[6] == 0);
  Assert(World->Root.Children[7] == 0);

  Assert(Engine->Graphics.NoiseFinalizeJobsPending == 0);
  Assert(Engine->Graphics.TotalChunkJobsActive == 0);
  Engine->Graphics.MainDrawList.ElementCount = 0;
  Engine->Graphics.ShadowMapDrawList.ElementCount = 0;

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

  Graphics->TerrainShapingRC.ReshapeFunc = {};
}

link_internal shared_lib
HardResetEngine( engine_resources *Engine,
                       const char *NewLibName,
                 hard_reset_flags  Flags       = HardResetFlag_None )
{
  UNPACK_ENGINE_RESOURCES(Engine);

  shared_lib Result = {};

  Info("Hard Reset Begin");

  AssertWorkerThreadsSuspended(Engine);
  Assert(WorkQueuesAreEmpty(Plat));

  RangeIterator_t(u32, EntityIndex, TOTAL_ENTITY_COUNT)
  {
    if ( (Flags&HardResetFlag_NoResetCamera) && Graphics->GameCamera.GhostId.Index == EntityIndex ) { continue; }
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



  application_api *GameApi   = &Engine->Stdlib.AppApi;
  engine_api      *EngineApi = &Engine->EngineApi;

  /* if (NewLibName) */
  {
    if (GameApi->GameDeInit)
    {
      Assert(ThreadLocal_ThreadIndex == 0);
      thread_local_state *MainThread = GetThreadLocalState(ThreadLocal_ThreadIndex);
      GameApi->GameDeInit(Engine, MainThread);
    }

    Result = OpenLibrary(NewLibName);
    Ensure(InitializeEngineApi(EngineApi, Result));
    Ensure(InitializeGameApi(GameApi, Result));
    // Hook up global pointers
    Ensure( EngineApi->OnLibraryLoad(Engine) );

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
  }




  Info("Hard Reset End");

  return Result;
}

