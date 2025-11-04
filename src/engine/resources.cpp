link_internal void
DeallocateAndClearWorldChunk(engine_resources *Engine, world_chunk *Chunk)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Assert(Chunk->IsOnFreelist == False);

  /* Assert( (Chunk->Flags & Chunk_Queued) == 0); */
  /* Assert( Chunk->Flags & (Chunk_Deallocate|Chunk_VoxelsInitialized)); */

  if (HasGpuMesh(Chunk))
  {
    DeallocateHandles(LoRenderQ, &Chunk->Handles);
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

  Engine->GameMemory = AllocateArena();
  Engine->WorldUpdateMemory = AllocateArena();

  Engine->Heap                    = InitHeap(Gigabytes(2)); // TODO(Jesse): Is this actually used?
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

link_internal void
CancelAllWorkQueueJobs(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  AssertWorkerThreadsSuspended(Engine);

  CancelAllWorkQueueJobs(Plat, &Plat->HighPriority);
  CancelAllWorkQueueJobs(Plat, &Plat->LowPriority);
  /* CancelAllWorkQueueJobs(Plat, &Plat->WorldUpdateQ); */

  // NOTE(Jesse): The RendeQ flushes before it suspends, and at the time of
  // this writing the application depends on this behavior.  Some render queue
  // jobs have knowledge of who to call next (because we don't have a way of
  // specifying the next next job when we submit one).  This makes it difficult
  // to free resources that are only known to the jobs..
  //
  // We just call CancelAllWorkQueueJobs to reset the queues.
  //
  Assert(QueueIsEmpty(&Plat->HiRenderQ));
  Assert(QueueIsEmpty(&Plat->LoRenderQ));
  CancelAllWorkQueueJobs(Plat, &Plat->HiRenderQ);
  CancelAllWorkQueueJobs(Plat, &Plat->LoRenderQ);

  Assert(QueueIsEmpty(&Plat->HighPriority));
  Assert(QueueIsEmpty(&Plat->LowPriority));

  PushBonsaiRenderCommandCancelAllNoiseReadbackJobs(&Plat->LoRenderQ);

  UnsignalFutex(&Plat->WorkerThreadsSuspendFutex);

  while (!QueueIsEmpty(&Plat->HiRenderQ)) { SleepMs(1); }
  while (!QueueIsEmpty(&Plat->LoRenderQ)) { SleepMs(1); }

  SignalAndWaitForWorkers(&Plat->WorkerThreadsSuspendFutex);
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

  CancelAllWorkQueueJobs(Engine);

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

  // NOTE(Jesse): We have to walk the octree to free all the GPU buffers :/
  FreeOctreeChildren(Engine, &World->Root);

  Engine->Graphics.NoiseFinalizeJobsPending = 0;
  Engine->Graphics.TotalChunkJobsActive = 0;

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

link_internal void
HardResetEngine(engine_resources *Engine, hard_reset_flags Flags = HardResetFlag_None)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Info("Hard Reset Begin");

  CancelAllWorkQueueJobs(Engine);

  RangeIterator_t(u32, EntityIndex, TOTAL_ENTITY_COUNT)
  {
    if ( (Flags&HardResetFlag_NoResetCamera) && Graphics->GameCamera.GhostId.Index == EntityIndex ) { continue; }
    Unspawn(EntityTable[EntityIndex]);
  }

  VaporizeArena(Engine->GameMemory);
  Engine->GameMemory = AllocateArena();

  HardResetEditor(&Engine->Editor);

  HardResetWorld(Engine);

  SoftResetGraphics(Graphics);

  // TODO(Jesse)(leak): This leaks the texture handles; make a HardResetEngineDebug()
  Leak("?");
  VaporizeArena(Engine->EngineDebug.Memory);
  Engine->EngineDebug = {};
  Engine->EngineDebug.Memory = AllocateArena();

  HardResetAssets(Engine);

  Assert(ThreadLocal_ThreadIndex == 0);
  thread_local_state *MainThread = GetThreadLocalState(ThreadLocal_ThreadIndex);
  auto GameApi = &Engine->Stdlib.AppApi;
  if (GameApi->GameInit)
  {
    Engine->GameState = GameApi->GameInit(Engine, MainThread);
  }

  Info("Hard Reset End");
}

