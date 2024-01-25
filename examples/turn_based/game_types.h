
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

  // NOTE(Jesse): This list is append-only because they're serialized
  EntityType_Enemy,
  EntityType_Player,
  EntityType_Fireball,
  EntityType_Loot,
};

enum entity_status
#if !POOF_PREPROCESSOR
 : u32
#endif
{
  EntityStatus_None,   // No special status
  EntityStatus_Carried,
  EntityStatus_Thrown,
};

struct entity_aggregate_type
{
  entity_type Type;
  entity_status Status;
};
CAssert(sizeof(entity_aggregate_type) == 8); // This has to fit into entity::UserType

poof(generate_string_table(entity_type))
#include <generated/generate_string_table_entity_type.h>

poof(do_editor_ui_for_enum(entity_type))
#include <generated/do_editor_ui_for_enum_entity_type.h>

enum player_action
{
  PlayerAction_None,

  // NOTE(Jesse): Move must come first because we start iterating from it.
  PlayerAction_Move,
  PlayerAction_Shovel,
  PlayerAction_Grab,
  PlayerAction_ChargeFireball,
  PlayerAction_IceBlock,
  PlayerAction_Throw,

  PlayerAction_Count,
};

global_variable s32
LastBackgroundSprite = 3;

global_variable s32
Global_SpriteIndexFromActionIndex[PlayerAction_Count] =
{
  -1, // TODO(Jesse): What should the invalid marker be?
  LastBackgroundSprite + 153, // move
  LastBackgroundSprite + 350, // shovel
  LastBackgroundSprite + 44,  // grab
  LastBackgroundSprite + 273, // fireball
  LastBackgroundSprite + 307, // ice
  LastBackgroundSprite + 256, // thrwo
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

