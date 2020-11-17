bonsai_function world*
AllocateAndInitWorld(world_position Center, voxel_position WorldChunkDim, chunk_dimension VisibleRegion)
{

  memory_arena* Memory = AllocateArena();
  world *World = Allocate(world, Memory, 1 );
  World->Memory = Memory;

  World->ChunkHash = Allocate(world_chunk*, Memory, WORLD_HASH_SIZE );
  World->FreeChunks = Allocate(world_chunk*, Memory, FREELIST_SIZE );

  Assert(World->FreeChunkCount == 0);

  World->ChunkDim = WorldChunkDim;
  World->VisibleRegion = VisibleRegion;

  World->Center = Center;

  return World;
}
