enum entity_state
{
  EntityState_Free        = 0,
  EntityState_Spawned     = 1 << 0,
  EntityState_Destroyed   = 1 << 1,
  EntityState_Reserved    = 1 << 2,
};

#if !POOF_PREPROCESSOR
enum entity_type : u32;
/* link_weak void DoEditorUi(renderer_2d *Ui, entity_type *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS); */
#endif

enum entity_behavior_flags
{
  EntityBehaviorFlags_None                             =       0,
  EntityBehaviorFlags_Gravity                          = (1 << 0),
  EntityBehaviorFlags_WorldCollision                   = (1 << 1),
  EntityBehaviorFlags_EntityCollision                  = (1 << 2),
  EntityBehaviorFlags_UnspawnOnParticleSystemTerminate = (1 << 3),

  EntityBehaviorFlags_Default = (EntityBehaviorFlags_Gravity       |
                                 EntityBehaviorFlags_WorldCollision|
                                 EntityBehaviorFlags_EntityCollision),
};

poof(generate_string_table(entity_state))
#include <generated/generate_string_table_entity_state.h>
poof(do_editor_ui_for_enum(entity_state))
#include <generated/do_editor_ui_for_enum_entity_state.h>

poof(generate_string_table(entity_behavior_flags))
#include <generated/generate_string_table_entity_behavior_flags.h>
poof(do_editor_ui_for_enum(entity_behavior_flags))
#include <generated/do_editor_ui_for_enum_entity_behavior_flags.h>

struct entity
{
  umm Id;

  model Model;
  v3 CollisionVolumeRadius;

  particle_system *Emitter;

  physics Physics;

  canonical_position P;

  Quaternion Rotation;

  entity_state          State;
  entity_type           Type;
  entity_behavior_flags Behavior;

  r32 Scale;

  void *UserData;
};

poof( block_array(entity, {4}) )
#include <generated/block_array_entity_688856407.h>
