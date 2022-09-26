// TODO(Jesse)(immediate): Pass arena in?
link_internal world*
AllocateWorld()
{
  memory_arena* Memory = AllocateArena();
  world *World = Allocate(world, Memory, 1 );
  World->Memory = Memory;

  World->ChunkHash = Allocate(world_chunk*, Memory, WORLD_HASH_SIZE );
  World->FreeChunks = Allocate(world_chunk*, Memory, FREELIST_SIZE );

  return World;
}

link_internal void
InitWorld(world* World, world_position Center, voxel_position WorldChunkDim, chunk_dimension VisibleRegion)
{
  World->ChunkDim = WorldChunkDim;
  World->VisibleRegion = VisibleRegion;
  World->Center = Center;
}
