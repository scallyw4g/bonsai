// TODO(Jesse)(immediate): Pass arena in?
link_internal world*
AllocateWorld()
{
  memory_arena* WorldMemory = AllocateArena(Gigabytes(5));
  DEBUG_REGISTER_ARENA(WorldMemory, 0);
  world *World = Allocate(world, WorldMemory, 1 );
  World->Memory = WorldMemory;

  World->ChunkHash = Allocate(world_chunk*, WorldMemory, WORLD_HASH_SIZE );
  World->FreeChunks = Allocate(world_chunk*, WorldMemory, FREELIST_SIZE );

  return World;
}

link_internal void
InitWorld(world* World, world_position Center, voxel_position WorldChunkDim, chunk_dimension VisibleRegion)
{
  World->ChunkDim = WorldChunkDim;
  World->VisibleRegion = VisibleRegion;
  World->Center = Center;
}
