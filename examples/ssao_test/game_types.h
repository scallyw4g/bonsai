struct world;
struct platform;
struct entity;
struct memory_arena;
struct random_series;

struct game_state
{
  world         *World;
  platform      *Plat;
  entity        *Player;
  entity        *Player2;
  memory_arena *Memory;

  random_series Entropy;

  server_state ServerState = {};

  network_connection Network;

  game_mode Mode;
  model   *Models;
  noise_3d *Turb;
  event_queue EventQueue;
  aabb *FolieTable;
  entity *EntityTable[TOTAL_ENTITY_COUNT];
};

