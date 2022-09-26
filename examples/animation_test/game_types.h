struct world;
struct platform;
struct entity;
struct memory_arena;
struct random_series;

struct game_state
{
  world         *World;
  platform      *Plat;
  memory_arena  *Memory;
  graphics      *Graphics;

  heap_allocator Heap;

  random_series Entropy;

  perlin_noise Noise;

  model   *Models;
  event_queue EventQueue;

  entity *EntityTable[TOTAL_ENTITY_COUNT];
  entity *Player;
};

