
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

  // NOTE(Jesse): This list is append-only!
  EntityType_Enemy,
  EntityType_Player,

  EntityType_Fireball,

  EntityType_Loot,
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
  PlayerAction_ChargeFireball,
  PlayerAction_Throw,
  PlayerAction_IceBlock,
  PlayerAction_Dig,
  PlayerAction_Grab,
  PlayerAction_ShovelSmack,

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

struct fireball_state
{
  r32 ChargeLevel;
  cp TargetP;
};

struct game_state
{
  memory_arena Memory;
  heap_allocator Heap;

  random_series Entropy;

  model      *Models;
  event_queue EventQueue;

  entity_id PlayerId;

  b32 PlayerActed;
  player_action ProposedAction;

  u32 FireballsSimulated;

  u32 TurnIndex;
  turn_mode TurnMode;
  r32 TransitionDuration;
};

