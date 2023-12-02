link_internal world *
AllocateWorld(world* World, world_position Center, voxel_position WorldChunkDim, chunk_dimension VisibleRegion)
{
  memory_arena *WorldMemory = AllocateArena(Gigabytes(2));
  DEBUG_REGISTER_ARENA(WorldMemory, 0);

  World->Memory = WorldMemory;

  World->HashSize = (u32)(Volume(VisibleRegion)*4); //WorldHashSize;
  /* World->HashSize = (u32)(Volume(VisibleRegion)*4); //WorldHashSize; */
  World->ChunkHashMemory[0] = Allocate(world_chunk*, WorldMemory, World->HashSize );
  World->ChunkHashMemory[1] = Allocate(world_chunk*, WorldMemory, World->HashSize );

  World->ChunkHash = World->ChunkHashMemory[0];
  World->FreeChunks = Allocate(world_chunk*, WorldMemory, FREELIST_SIZE );

  World->ChunkDim = WorldChunkDim;
  World->VisibleRegion = VisibleRegion;
  World->Center = Center;

  World->ColorPalette = V3Cursor(u16_MAX, WorldMemory);

  RangeIterator_t(u32, ColorIndex, u8_MAX)
  {
    Push(&World->ColorPalette, MAGICAVOXEL_DEFAULT_PALETTE[ColorIndex]);
  }

  return World;
}

