
struct world;
struct platform;
struct entity;
struct memory_arena;
struct random_series;

struct game_state
{
  world         *World;
  platform      *Plat;
  graphics      *Graphics;

  memory_arena*   Memory;
  heap_allocator  Heap;

  random_series Entropy;

  perlin_noise Noise;

  model   *Models;
  event_queue EventQueue;

  mesh_freelist MeshFreelist;

  entity *EntityTable[TOTAL_ENTITY_COUNT];
  entity *Player;

  gpu_mapped_element_buffer GpuBuffers[2];
  u32 GpuBufferWriteIndex;

#if BONSAI_INTERNAL
  get_debug_state_proc GetDebugState;
#endif
};

