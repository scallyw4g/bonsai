
struct world;
struct platform;
struct entity;
struct memory_arena;
struct random_series;

struct game_state
{
  random_series Entropy;

  perlin_noise Noise;

  model   *Models;
  event_queue EventQueue;

  entity *Player;

#if BONSAI_DEBUG_SYSTEM_API
  get_debug_state_proc GetDebugState;
#endif
};

