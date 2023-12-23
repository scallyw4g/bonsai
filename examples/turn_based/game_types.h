
struct world;
struct platform;
struct entity;
struct memory_arena;
struct random_series;


enum entity_type
#if !POOF_PREPROCESSOR
 : u32
#endif
{
  EntityType_Default,

  EntityType_Enemy,
  EntityType_Player,
};

poof(generate_string_table(entity_type))
#include <generated/generate_string_table_entity_type.h>

poof(do_editor_ui_for_enum(entity_type))
#include <generated/do_editor_ui_for_enum_entity_type.h>

enum player_action
{
  PlayerAction_None,

  // NOTE(Jesse): Move must come first because we start iterating from it.
  PlayerAction_Move,
  PlayerAction_Charge,
  PlayerAction_Fire,
  PlayerAction_Jump,

  PlayerAction_Count,
};


poof(generate_string_table(player_action))
#include <generated/generate_string_table_player_action.h>


enum turn_mode
{
  TurnMode_Default,
  TurnMode_Transition,
};

poof(generate_string_table(turn_mode))
#include <generated/generate_string_table_turn_mode.h>

struct game_state
{
  random_series Entropy;

  model      *Models;
  event_queue EventQueue;

  entity *Player;

  player_action SelectedAction;
  u32 PlayerChargeLevel;

  b32 PlayerActed;

  turn_mode TurnMode;
  r32 TransitionDuration;

#if DEBUG_SYSTEM_API
  get_debug_state_proc GetDebugState;
#endif
};

