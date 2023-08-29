#define MAX_PICKED_WORLD_CHUNKS (64)


struct world;
struct world_chunk;
struct heap_allocator;
struct entity;


struct picked_world_chunk
{
  world_chunk *Chunk;
  r32 tChunk; // f32_MAX indicates not picked
};

enum picked_voxel_position
{
  PickedVoxel_LastEmpty,
  PickedVoxel_FirstFilled,
  PickedVoxel_Count,
};

struct picked_voxel
{
  picked_world_chunk PickedChunk;

  // Relative to origin of chunk
  v3 Picks[PickedVoxel_Count]; // use picked_voxel_position to index into this
};


// TODO(Jesse)(metaprogramming, ptr): Once poof can accept pointer types we can generate this struct
/* poof(static_buffer(world_chunk*, 64)) */
/* #include <generated/buffer_world_chunk.h> */
struct picked_world_chunk_static_buffer
{
  picked_world_chunk E[MAX_PICKED_WORLD_CHUNKS];
  u64 At;
};

link_internal void
Push(picked_world_chunk_static_buffer *Buf, world_chunk *Chunk, r32 t)
{
  if (Buf->At < MAX_PICKED_WORLD_CHUNKS)
  {
    Buf->E[Buf->At].Chunk = Chunk;
    Buf->E[Buf->At].tChunk = t;

    ++Buf->At;
  }
}

enum pick_chunk_state
{
  PickedChunkState_None,
  PickedChunkState_Hover,
};

struct engine_debug
{
  picked_world_chunk_static_buffer PickedChunks;
  texture_cursor Textures;

  ui_debug UiDebug;

  b8 DrawEntityCollisionVolumes;
  b8 DrawWorldAxies;

  b8 TriggerRuntimeBreak;

  u8 PickedChunkState;
  world_chunk *PickedChunk;
};


link_internal engine_debug* GetEngineDebug();
