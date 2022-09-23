
struct world;
struct platform;
struct entity;
struct memory_arena;
struct random_series;

struct game_state
{
  world         *World;
  platform      *Plat;

  memory_arena*   Memory;
  heap_allocator  Heap;

  random_series Entropy;

  perlin_noise Noise;

  model   *Models;
  event_queue EventQueue;

  mesh_freelist MeshFreelist;

  entity** EntityTable;
  entity *Player;

#if BONSAI_DEBUG_SYSTEM_API
  get_debug_state_proc GetDebugState;
#endif
};

