enum entity_state
{
  EntityState_Free        = 0,
  EntityState_Spawned     = 1 << 0,
  EntityState_Destroyed   = 1 << 1,
  EntityState_Reserved    = 1 << 2,
};

enum entity_type
{
  EntityType_None             = 0,

  EntityType_Player           = 1 << 0,
  EntityType_Enemy            = 1 << 1,
  EntityType_EnemyProjectile  = 1 << 2,
  EntityType_PlayerProjectile = 1 << 3,
  EntityType_Loot             = 1 << 4,
  EntityType_PlayerProton     = 1 << 5,
  EntityType_ParticleSystem   = 1 << 6,

  EntityType_Static           = 1 << 7,

  EntityType_Default           = 1 << 8, // Nothing special about it, just needed an entity
};

poof(generate_string_table(entity_type))
#include <generated/generate_string_table_entity_type.h>

poof(do_editor_ui_for_enum(entity_type))
#include <generated/do_editor_ui_for_enum_entity_type.h>

global_variable const entity_type ENTITY_TYPES = (entity_type)
  ( EntityType_Player           |
    EntityType_Enemy            |
    EntityType_EnemyProjectile  |
    EntityType_PlayerProjectile |
    EntityType_Loot             |
    EntityType_PlayerProton     |
    EntityType_ParticleSystem
   );


struct entity;
typedef void (*update_callback)(engine_resources *, entity *);

struct entity
{
  model Model;
  v3 CollisionVolumeRadius;

  particle_system* Emitter;

  physics Physics;

  canonical_position P;

  Quaternion Rotation;

  entity_state State;
  entity_type Type;

  r32 Scale;

   // TODO(Jesse, id: 86, tags: memory_consumption, entity): Unneeded for projectiles. factor out of here?
  r32 RateOfFire;
  r32 FireCooldown;

  s32 Health;

  /* update_callback Update; */
  void* UserData;
};

poof( block_array(entity, {4}) )
#include <generated/block_array_entity_688856407.h>
