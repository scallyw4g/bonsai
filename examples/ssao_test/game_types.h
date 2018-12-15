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

  server_state *ServerState;
  perlin_noise Noise;

  game_mode Mode;
  model   *Models;
  noise_3d *Turb;
  event_queue EventQueue;
  mesh_freelist MeshFreelist;

  untextured_3d_geometry_buffer Mesh;

  entity *EntityTable[TOTAL_ENTITY_COUNT];

  entity *Players[MAX_CLIENTS];
};

