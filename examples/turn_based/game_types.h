
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
  EntityType_ItemSpawn,
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

link_internal u64
PackEntityAggregateType(entity_aggregate_type Type)
{
  u64 Reuslt = ReinterpretCast(u64, Type);
  return Reuslt;
}

link_internal u64
PackEntityAggregateType( entity_type Type, entity_status Status )
{
  entity_aggregate_type T = {Type, Status};
  u64 Result = PackEntityAggregateType(T);
  return Result;
}

link_internal entity_aggregate_type*
UserTypeToAggregateTypePtr(u64 *UserType)
{
  entity_aggregate_type *Result = Cast(entity_aggregate_type*, UserType);
  return Result;
}

link_internal entity_aggregate_type
UserTypeToAggregateType(u64 UserType)
{
  entity_aggregate_type Result = ReinterpretCast(entity_aggregate_type, UserType);
  return Result;
}


poof(generate_string_table(entity_type))
#include <generated/generate_string_table_entity_type.h>

poof(generate_string_table(entity_status))
#include <generated/generate_string_table_entity_status.h>

poof(do_editor_ui_for_enum(entity_type))
#include <generated/do_editor_ui_for_enum_entity_type.h>

poof(do_editor_ui_for_enum(entity_status))
#include <generated/do_editor_ui_for_enum_entity_status.h>

poof(do_editor_ui_for_compound_type(entity_aggregate_type));
#include <generated/do_editor_ui_for_compound_type_entity_aggregate_type.h>



struct entity_game_data
{
  u32 FireballChargeLevel;
  u32 FireballCharges;

  u32 IceBlockCharges;
  u32 HoldingItem;
};

poof(do_editor_ui_for_compound_type(entity_game_data))
#include <generated/do_editor_ui_for_compound_type_entity_game_data.h>

struct fireball_state
{
  r32 ChargeLevel;
  cp TargetP;
};

poof(do_editor_ui_for_compound_type(fireball_state))
#include <generated/do_editor_ui_for_compound_type_fireball_state.h>

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

