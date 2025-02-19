link_internal void
DeallocateAndClearWorldChunk(engine_resources *Engine, world_chunk *Chunk)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(Chunk->DEBUG_OwnedByThread == 0);
  Chunk->DEBUG_OwnedByThread = ThreadLocal_ThreadIndex;

  Assert( (Chunk->Flags & Chunk_Queued) == 0);
  Assert( Chunk->Flags & (Chunk_Deallocate|Chunk_VoxelsInitialized));

  if (HasGpuMesh(&Chunk->Mesh))
  {
    PushDeallocateBuffersCommand(RenderQ, &Chunk->Mesh.Handles);
  }

  ClearWorldChunk(Chunk);

  Assert(Chunk->DEBUG_OwnedByThread == 0);
  Assert(Chunk->Flags == 0);
  Assert(Chunk->Next == 0);

  FullBarrier;
}

link_internal void
RenderOctree(engine_resources *Engine, shader *Shader)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  b32     Continue = True;

  octree_node_ptr_stack Stack = OctreeNodePtrStack(1024, World->OctreeMemory);
  Push(&Stack, &World->Root);

  /* RuntimeBreak(); */
  while (CurrentCount(&Stack))
  {
    octree_node *Node = Pop(&Stack);

    if (Node->Type == OctreeNodeType_Leaf)
    {
      DrawLod(Engine, Shader, &Node->Chunk->Mesh, 0, {}, Quaternion(), V3(1));
    }

    if (Node->Children[0]) { Push(&Stack, Node->Children[0]); }
    if (Node->Children[1]) { Push(&Stack, Node->Children[1]); }
    if (Node->Children[2]) { Push(&Stack, Node->Children[2]); }
    if (Node->Children[3]) { Push(&Stack, Node->Children[3]); }
    if (Node->Children[4]) { Push(&Stack, Node->Children[4]); }
    if (Node->Children[5]) { Push(&Stack, Node->Children[5]); }
    if (Node->Children[6]) { Push(&Stack, Node->Children[6]); }
    if (Node->Children[7]) { Push(&Stack, Node->Children[7]); }
  }
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
  CancelAllWorkQueueJobs(Plat, &Plat->WorldUpdateQ);

  // NOTE(Jesse): The RendeQ flushes before it suspends, and at the time of
  // this writing the application depends on this behavior.  Some render queue
  // jobs have knowledge of who to call next (because we don't have a way of
  // specifying the next next job when we submit one).  This makes it difficult
  // to free resources that are only known to the jobs..
  CancelAllWorkQueueJobs(Plat, &Plat->RenderQ);

  Assert(QueueIsEmpty(&Plat->HighPriority));
  Assert(QueueIsEmpty(&Plat->LowPriority));
  Assert(QueueIsEmpty(&Plat->WorldUpdateQ));
  Assert(QueueIsEmpty(&Plat->RenderQ));
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

  /* FreeOctreeChildren(Engine, &World->Root); */
  /* if (World->Root.Chunk) { FreeWorldChunk(Engine, World->Root.Chunk); } */
  /* InitOctreeNode(World, &World->Root, {}, World->VisibleRegion, {}); */

  VaporizeArena(World->ChunkMemory);
  VaporizeArena(World->OctreeMemory);

  v3i Center = World->Center;
  v3i ChunkDim = World->ChunkDim;
  v3i VisibleRegion = World->VisibleRegion;
  Clear(World);

  AllocateWorld(World, Center, ChunkDim, VisibleRegion);
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

  // TODO(Jesse)(leak): This leaks the texture handles; make a HardResetEngineDebug()
  Leak("?");
  VaporizeArena(Engine->EngineDebug.Memory);
  Engine->EngineDebug = {};
  Engine->EngineDebug.Memory = AllocateArena();

  HardResetAssets(Engine);

  umm ReadbackJobCount = TotalElements(&Graphics->NoiseReadbackJobs);
  RangeIterator_t(umm, JobIndex, ReadbackJobCount)
  {
    dummy_work_queue_entry_build_chunk_mesh_block_array_index I = ZerothIndex(&Graphics->NoiseReadbackJobs);
    RemoveUnordered(&Graphics->NoiseReadbackJobs, I);
  }

  Info("Hard Reset End");
}

