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

enum entity_behavior_flags poof(@bitfield)
{
  EntityBehaviorFlags_None                             =       0,
  EntityBehaviorFlags_Gravity                          = (1 << 0),
  EntityBehaviorFlags_WorldCollision                   = (1 << 1),
  EntityBehaviorFlags_EntityCollision                  = (1 << 2),
  EntityBehaviorFlags_UnspawnOnParticleSystemTerminate = (1 << 3),
  EntityBehaviorFlags_FitCollisionVolumeToModel        = (1 << 4),

  // NOTE(Jesse): This is more of an engine_entity_type, but I didn't see a
  // reason to make a whole new thing for that for just one value.  If this
  // blows out to more than that it might be worth it..
  EntityBehaviorFlags_DefatulCameraGhostBehavior       = (1 << 5),

  EntityBehaviorFlags_WorldCenter                      = (1 << 6),

  EntityBehaviorFlags_Default = ( EntityBehaviorFlags_Gravity                   |
                                  EntityBehaviorFlags_WorldCollision            |
                                  EntityBehaviorFlags_FitCollisionVolumeToModel |
                                  EntityBehaviorFlags_EntityCollision           ),
};

poof(generate_string_table(entity_state))
#include <generated/generate_string_table_entity_state.h>

poof(generate_string_table(entity_behavior_flags))
#include <generated/generate_string_table_entity_behavior_flags.h>

struct collision_event
{
  u32 FrameIndex;
  u32 Count;
  cp MinP;
  cp MaxP;
};

struct entity_position_info
{
  cp P;
  v3 _CollisionVolumeRadius;
  r32 Scale;
  v3 EulerAngles;
};

// Game can implement this function to serialize/deserialize their custom entity data.
link_weak b32 EntityUserDataSerialize(native_file *, u64 UserType, u64 UserData);
link_weak b32 EntityUserDataDeserialize(u8_cursor *, u64 *UserType, u64 *UserData, memory_arena*);

#define ENTITY_VERSION (1)
struct entity
{
  /* u64 Version; */

  entity_id Id;

  // NOTE(Jesse): This is a pretty dirty hack to get the Offset in CP to have
  // a custom value range.  I couldn't figure out a better way while thinking about
  // it for 15 minutes, so I settled on this for now.  Hopefully I figure out
  // something better soon, cause this is un-tenable for the long-term
  //
  // @dirty_entity_P_format_hack
  //
  cp P;           poof(@custom_ui(DoEditorUi_entity_P(Ui, Window, Element, CSz("cp P"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)))

  v3 EulerAngles; poof(@ui_value_range(-PI32, PI32))
  r32 Scale;

  // NOTE(Jesse): This must be updated with UpdateCollisionVolumeRadius.
  // Entity pointers are stored on world chunks, which are used during
  // collision detection.  The chunks to store the entity to are computed using
  // the CollisionVolumeRadius so if we update it we have to also check if the
  // number of chunks we're overlapping changed, and update the difference.
  v3 _CollisionVolumeRadius;

  physics Physics;

  asset_id AssetId;
  u64      ModelIndex;

  collision_event      LastResolvedCollision; poof(@no_serialize)
  entity_position_info LastResolvedPosInfo;   poof(@no_serialize)

  particle_system *Emitter;

  entity_state          State;
  entity_behavior_flags Behavior;

  entity_id Carrying;

  u64 UserType;
  u64 UserData;
  poof(  @custom_serialize(  if (EntityUserDataSerialize)   {Result &= EntityUserDataSerialize(File, Element->UserType, Element->UserData);})
         @custom_deserialize(if (EntityUserDataDeserialize) {Result &= EntityUserDataDeserialize(Bytes, &Element->UserType, &Element->UserData, Memory);})  )
};

struct entity_1
{
  /* u64 Version; */

  entity_id Id;

  // NOTE(Jesse): This is a pretty dirty hack to get the Offset in CP to have
  // a custom value range.  I couldn't figure out a better way while thinking about
  // it for 15 minutes, so I settled on this for now.  Hopefully I figure out
  // something better soon, cause this is un-tenable for the long-term
  //
  // @dirty_entity_P_format_hack
  //
  cp P;           poof(@custom_ui(DoEditorUi_entity_P(Ui, Window, Element, CSz("cp P"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)))

  v3 EulerAngles; poof(@ui_value_range(-180.f, 180.f))
  r32 Scale;

  // NOTE(Jesse): This must be updated with UpdateCollisionVolumeRadius.
  // Entity pointers are stored on world chunks, which are used during
  // collision detection.  The chunks to store the entity to are computed using
  // the CollisionVolumeRadius so if we update it we have to also check if the
  // number of chunks we're overlapping changed, and update the difference.
  v3 _CollisionVolumeRadius;

  physics Physics;

  asset_id AssetId;
  u64      ModelIndex;

  collision_event      LastResolvedCollision; poof(@no_serialize)
  entity_position_info LastResolvedPosInfo;   poof(@no_serialize)

  particle_system *Emitter;

  entity_state          State;
  entity_behavior_flags Behavior;

  entity_id Carrying;

  u64 UserType;

  u64 UserData;
  poof(  @custom_serialize(  if (EntityUserDataSerialize)   {Result &= EntityUserDataSerialize(File, Element->UserType, Element->UserData);})
         @custom_deserialize(if (EntityUserDataDeserialize) {Result &= EntityUserDataDeserialize(Bytes, &Element->UserType, &Element->UserData, Memory);})  )
};

struct entity_0
{
  /* u64 Version; */

  entity_id Id;

  // NOTE(Jesse): This is a pretty dirty hack to get the Offset in CP to have
  // a custom value range.  I couldn't figure out a better way while thinking about
  // it for 15 minutes, so I settled on this for now.  Hopefully I figure out
  // something better soon, cause this is un-tenable for the long-term
  //
  // @dirty_entity_P_format_hack
  //
  cp P;           poof(@custom_ui(DoEditorUi_entity_P(Ui, Window, Element, CSz("cp P"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)))

  v3 EulerAngles; poof(@ui_value_range(-180.f, 180.f))
  r32 Scale;

  // NOTE(Jesse): This must be updated with UpdateCollisionVolumeRadius.
  // Entity pointers are stored on world chunks, which are used during
  // collision detection.  The chunks to store the entity to are computed using
  // the CollisionVolumeRadius so if we update it we have to also check if the
  // number of chunks we're overlapping changed, and update the difference.
  v3 _CollisionVolumeRadius;

  physics Physics;

  asset_id AssetId;
  u64      ModelIndex;

  collision_event      LastResolvedCollision; poof(@no_serialize)
  entity_position_info LastResolvedPosInfo;   poof(@no_serialize)

  particle_system *Emitter;

  entity_state          State;
  entity_behavior_flags Behavior;

  u64 UserType;
  u64 UserData;
  poof(  @custom_serialize(  if (EntityUserDataSerialize)   {Result &= EntityUserDataSerialize(File, Element->UserType, Element->UserData);})
         @custom_deserialize(if (EntityUserDataDeserialize) {Result &= EntityUserDataDeserialize(Bytes, &Element->UserType, &Element->UserData, Memory);})  )
};

link_internal void
FinalizeEntityUpdate(entity *Entity)
{
  Assert(IsCanonical(GetWorld(), Entity->P));
  poof( func (entity_position_info Info)
  {
    Info.map(member)
    {
      Entity->LastResolvedPosInfo.member.name = Entity->member.name;
    }
  })
#include <generated/anonymous_entity_position_info_ynGg9Dhj.h>
}

link_internal void
DropEntityFromPreviouslyOccupiedChunks(world *World, entity *Entity, memory_arena *TempMemory)
{
  // nocheckin
  /* Assert(Entity->Behavior & EntityBehaviorFlags_EntityCollision); */

  Assert(ThreadLocal_ThreadIndex == 0);

  rect3cp EntityArea = RectMinMax(Entity->LastResolvedPosInfo.P, Canonicalize(World->ChunkDim, Entity->LastResolvedPosInfo.P + Entity->LastResolvedPosInfo._CollisionVolumeRadius*2.f));
  world_chunk_ptr_buffer Chunks = GatherChunksOverlappingArea(World, EntityArea, TempMemory);

  if (Chunks.Count)
  {
    /* Info("Attempting to drop Entity(%p) from (%d) Chunks", Entity, Chunks.Count); */
    RangeIterator_t(umm, ChunkIndex, Chunks.Count)
    {
      world_chunk *Chunk = Chunks.Start[ChunkIndex];
      /* Info("Attempting to drop Entity(%p) from Chunk(%p)", Entity, Chunk); */

      b32 Got = False;
      IterateOver(&Chunk->Entities, TestEntity, TestEntityIndex)
      {
        if (*TestEntity == Entity)
        {
          RemoveUnordered(&Chunk->Entities, TestEntityIndex);
          Got = True;
          break;
        }
      }
      // NOTE(Jesse): This is a bit sketch.  For it to work 100% correctly we
      // cannot ever have entities that overlap uninitialized bits of the world.
      //
      // Currently it cannot be in, because an entity can be spawned outside
      // the world just fine, then when the visible region moves to enclose the
      // entity chunks will get initialized, but the entity will never have
      // been added to the freshly minted chunks.
      //
      // We'd have to do some nonsense like check every entity for every chunk
      // if it overlaps when we insert, which is not a thing.
      /* Assert(Got); */
    }
  }
}

link_internal void
InsertEntityIntoChunks(world *World, entity *Entity, memory_arena *TempMemory)
{
  // nocheckin
  /* Assert(Entity->Behavior & EntityBehaviorFlags_EntityCollision); */

  auto MinP = Entity->LastResolvedPosInfo.P;
  auto MaxP = Canonicalize(World->ChunkDim, Entity->LastResolvedPosInfo.P + Entity->LastResolvedPosInfo._CollisionVolumeRadius*2.f);

  rect3cp EntityArea = RectMinMax(MinP, MaxP);
  world_chunk_ptr_buffer Chunks = GatherChunksOverlappingArea(World, EntityArea, TempMemory);
  if (Chunks.Count)
  {
    /* Info("Attempting to insert Entity(%p) into (%d) Chunks", Entity, Chunks.Count); */
    RangeIterator_t(umm, ChunkIndex, Chunks.Count)
    {
      world_chunk *Chunk = Chunks.Start[ChunkIndex];
      /* Info("Attempting to insert Entity(%p) into Chunk(%p)", Entity, Chunk); */

      Push(&Chunk->Entities, &Entity);
    }
  }
}

link_internal void
UpdateCollisionVolumeRadius(world *World, entity *Entity, v3 NewRadius, memory_arena *TempMemory)
{
  /* DropEntityFromOccupiedChunks(World, Entity, TempMemory); */
  Entity->_CollisionVolumeRadius = NewRadius;
  /* InsertEntityIntoChunks(World, Entity, TempMemory); */
}

link_internal collision_event
GetCollision( world *World, canonical_position TestP, v3 CollisionDim );

link_internal collision_event
GetCollision( world *World, aabb SimSpaceCollisionDim );
