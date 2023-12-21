
struct world;
struct platform;
struct entity;
struct memory_arena;
struct random_series;

enum player_action
{
  PlayerAction_None,

  PlayerAction_Move,
  PlayerAction_Charge,
  PlayerAction_Fire,
  PlayerAction_Jump,

  PlayerAction_Count,
};


poof(generate_string_table(player_action))
#include <generated/generate_string_table_player_action.h>


struct game_state
{
  random_series Entropy;

  model   *Models;
  event_queue EventQueue;

  entity *Player;

  player_action SelectedAction;
  u32 PlayerChargeLevel;

  // TODO(Jesse): Use the one on the engine_resources
  /* entity *CameraGhost; */

  b32 DidPlayerAction;

#if DEBUG_SYSTEM_API
  get_debug_state_proc GetDebugState;
#endif
};

