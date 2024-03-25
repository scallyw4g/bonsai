
link_internal b32
InitEngineResources(engine_resources *Engine)
{
  b32 Result = True;

  platform *Plat = &Engine->Stdlib.Plat;

  Assert(Global_ShaderHeaderCode.Start == 0);
  LoadGlobalShaderHeaderCode(Engine->Settings.Graphics.ShaderLanguage);

  memory_arena *WorldAndEntityArena = AllocateArena(Megabytes(256));
  DEBUG_REGISTER_ARENA(WorldAndEntityArena, 0);

  Engine->GameMemory = AllocateArena();
  Engine->WorldUpdateMemory = AllocateArena();

  Engine->Heap                    = InitHeap(Gigabytes(2)); // TODO(Jesse): Is this actually used?
  Engine->AssetSystem.AssetMemory = InitHeap(Gigabytes(1));

  Init_Global_QuadVertexBuffer();

  Engine->World = Allocate(world, WorldAndEntityArena, 1);
  if (!Engine->World) { Error("Allocating World"); Result = False; }

  Engine->Graphics = GraphicsInit(&Engine->Settings, AllocateArena());
  if (!Engine->Graphics) { Error("Initializing Graphics"); Result = False; }

  {
    memory_arena *UiMemory = AllocateArena();
    InitRenderer2D(&Engine->Ui, &Engine->Heap, UiMemory, &Plat->MouseP, &Plat->MouseDP, &Plat->ScreenDim, &Plat->Input);

    bitmap_block_array Bitmaps = { .Memory = GetTranArena() };
    /* LoadBitmapsFromFolderUnordered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/64x64_sprites"), &Bitmaps); */
    /* LoadBitmapsFromFolderUnordered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/44x44_sprites"), &Bitmaps); */
    /* Engine->Ui.SpriteTextureArray = CreateTextureArrayFromBitmapBlockArray(&Bitmaps, V2i(64,64)); */

    LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/64x64_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
    LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/44x44_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
    Engine->Ui.SpriteTextureArray = CreateTextureArrayFromBitmapBlockArray(&Bitmaps, V2i(64,64));

    // TODO(Jesse): Can this be turned into a bitmap_buffer_buffer statically somehow so as to avoid the ArrayCount?
    /* bitmap_buffer Buffers[] = { Bitmaps64, Bitmaps44 }; */
    /* Engine->Ui.SpriteTextureArray = CreateTextureArrayFromBitmapBufferArray(Buffers, ArrayCount(Buffers), V2i(64,64)); */
  }

  Engine->EntityTable = AllocateEntityTable(WorldAndEntityArena, TOTAL_ENTITY_COUNT);
  return Result;
}

link_internal b32
InitEngineDebug(engine_debug *Debug)
{
  b32 Result = True;

  Debug->Memory = AllocateArena();

  Debug->Textures.Memory = Debug->Memory;

  {
    Debug->WorldEditDebugThumb.Texture = MakeTexture_RGB(V2i(256), 0, CSz("WorldEditDebugTexture"));
    StandardCamera(&Debug->WorldEditDebugThumb.Camera, 10000.f, 1000.f, 30.f);
    AllocateMesh(&Debug->WorldEditDebugMesh,  u32(Kilobytes(64*16)), Debug->Memory);
  }

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

