
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

  // TODO(Jesse): Use the one on the engine_resources
  entity *CameraGhost;

  b32 DidPlayerAction;

#if DEBUG_SYSTEM_API
  get_debug_state_proc GetDebugState;
#endif
};

