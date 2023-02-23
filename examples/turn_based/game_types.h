
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
  entity *Enemy;
  entity *CameraTarget;

#if DEBUG_SYSTEM_API
  get_debug_state_proc GetDebugState;
#endif
};

