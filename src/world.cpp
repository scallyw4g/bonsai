function world*
AllocateAndInitWorld( game_state *GameState,
                      world_position Center, voxel_position WorldChunkDim, chunk_dimension VisibleRegion)
{
  world *World = Allocate(world, GameState->Memory, 1 );
  GameState->World = World;

  World->ChunkHash = Allocate(world_chunk*, GameState->Memory, WORLD_HASH_SIZE );
  World->FreeChunks = Allocate(world_chunk*, GameState->Memory, FREELIST_SIZE );

  Assert(World->FreeChunkCount == 0);

  World->ChunkDim = WorldChunkDim;
  World->VisibleRegion = VisibleRegion;

  World->Center = Center;

  return World;
}
