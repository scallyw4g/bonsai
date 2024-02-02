
struct world;
struct platform;
struct entity;
struct memory_arena;
struct random_series;

struct game_state
{
  random_series Entropy;

  model   *Models;
  event_queue EventQueue;

  entity *Player;

};

