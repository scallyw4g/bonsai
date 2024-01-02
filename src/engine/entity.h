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

  // NOTE(Jesse): This is more of an engine_entity_type, but I didn't see a
  // reason to make a whole new thing for that for just one value.  If this
  // blows out to more than that it might be worth it..
  EntityBehaviorFlags_CameraGhost                      = (1 << 4),

  EntityBehaviorFlags_Default = (EntityBehaviorFlags_Gravity       |
                                 EntityBehaviorFlags_WorldCollision|
                                 EntityBehaviorFlags_EntityCollision),
};

#if 0
link_internal b32
Deserialize(u8_stream *Bytes, void *Data)
{
  return True;
}

link_internal b32
Serialize(native_file *File, void *Data)
{
  return True;
}
#endif

poof(generate_string_table(entity_state))
#include <generated/generate_string_table_entity_state.h>

poof(do_editor_ui_for_enum(entity_state))
#include <generated/do_editor_ui_for_enum_entity_state.h>

poof(generate_string_table(entity_behavior_flags))
#include <generated/generate_string_table_entity_behavior_flags.h>

poof(do_editor_ui_for_enum(entity_behavior_flags))
#include <generated/do_editor_ui_for_enum_entity_behavior_flags.h>

struct collision_event
{
  u32 FrameIndex;
  u32 Count;
  canonical_position MinP;
  canonical_position MaxP;
};

struct entity_position_info
{
  cp P;
  v3 _CollisionVolumeRadius;
  r32 Scale;
  v4 EulerAngles;
};

struct entity
{
  u64 Version;

  u64 Id;

  cp P;
  // NOTE(Jesse): This is a v4 because it used to be a quaternion, and the
  // savefiles have magic numbers so they have to deal with elements that are
  // the same size.  The w coordinate is unused
  v4 EulerAngles;
  r32 Scale;

  // NOTE(Jesse): This must be updated with UpdateCollisionVolumeRadius.
  // Entity pointers are stored on world chunks, which are used during
  // collision detection.  The chunks to store the entity to are computed using
  // the CollisionVolumeRadius so if we update it we have to also check if the
  // number of chunks we're overlapping changed, and update the difference.
  v3 _CollisionVolumeRadius;

  physics Physics;

  asset_id             AssetId;

  collision_event      LastResolvedCollision; poof(@no_serialize)
  entity_position_info LastResolvedPosInfo;   poof(@no_serialize)

  particle_system *Emitter;

  entity_state          State;
  entity_behavior_flags Behavior;

  u64 UserType;
  u64 UserData;
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

poof(serdes_vector(v2))
#include <generated/serdes_vector_v2.h>

poof(serdes_vector(v3))
#include <generated/serdes_vector_v3.h>

poof(serdes_vector(v4))
#include <generated/serdes_vector_v4.h>

poof(serdes_vector(v2i))
#include <generated/serdes_vector_v2i.h>

poof(serdes_vector(v3i))
#include <generated/serdes_vector_v3i.h>

poof(serdes_primitive({chunk_flag}))
#include <generated/serdes_primitive_85387614.h>

poof(serdes_cursor(v3))
#include <generated/serdes_cursor_v3.h>

#if 1
link_internal b32 Serialize(native_file *File, untextured_3d_geometry_buffer *Data);
#endif


poof(serdes_struct(voxel))
#include <generated/serdes_struct_voxel.h>

poof(serdes_array(voxel))
#include <generated/serdes_array_voxel.h>

poof(serdes_struct(voxel_lighting))
#include <generated/serdes_struct_voxel_lighting.h>
poof(serdes_array(voxel_lighting))
#include <generated/serdes_array_voxel_lighting.h>

poof(serdes_array(v3))
#include <generated/serdes_array_v3.h>

poof(serdes_struct(vertex_material))
#include <generated/serdes_struct_vertex_material.h>
poof(serdes_array(vertex_material))
#include <generated/serdes_array_vertex_material.h>



link_internal b32
Serialize(native_file *File, chunk_data *Data)
{
  b32 Result = True;

#if 1
  umm ElementCount = umm(Volume(Data->Dim));

  Result &= WriteToFile(File, Cast(u8*, Data), sizeof(chunk_data));

  if (ElementCount)
  {
    SerializeArray(File, Data->Voxels, ElementCount);
    SerializeArray(File, Data->VoxelLighting, ElementCount);
  }
#endif

  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, chunk_data *Data, memory_arena *Memory)
{
  b32 Result = True;

#if 1
  Result &= ReadBytesIntoBuffer(Bytes, sizeof(chunk_data), Cast(u8*, Data));

  umm ElementCount = umm(Volume(Data->Dim));
  Data->Voxels = 0;
  Data->VoxelLighting = 0;

  if (ElementCount)
  {
    Result &= DeserializeArray(Bytes, &Data->Voxels, ElementCount, Memory);
    Result &= DeserializeArray(Bytes, &Data->VoxelLighting, ElementCount, Memory);
  }
#endif

  return Result;
}

link_internal b32
Serialize(native_file *File, animation *Data)
{
  return True;
}

link_internal b32
Deserialize(u8_stream *Bytes, animation *Data, memory_arena *Memory)
{
  return True;
}

link_internal b32
Serialize(native_file *File, untextured_3d_geometry_buffer *Data)
{
  b32 Result = True;

#if 1
  Result &= WriteToFile(File, Cast(u8*, Data), sizeof(untextured_3d_geometry_buffer));

  umm ElementCount = umm(Data->At);
  if (ElementCount)
  {
    Result &= SerializeArray(File, Data->Verts, ElementCount);
    Result &= SerializeArray(File, Data->Normals, ElementCount);
    Result &= SerializeArray(File, Data->Mat, ElementCount);
  }
#endif

  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, untextured_3d_geometry_buffer *Data, memory_arena *Memory)
{
  b32 Result = True;
#if 1
  Result &= ReadBytesIntoBuffer(Bytes, sizeof(untextured_3d_geometry_buffer), Cast(u8*, Data));

  umm ElementCount = Data->At;

  Data->Verts = 0;
  Data->Normals = 0;
  Data->Mat = 0;

  if (ElementCount)
  {
    Result &= DeserializeArray(Bytes, &Data->Verts, ElementCount, Memory);
    Result &= DeserializeArray(Bytes, &Data->Normals, ElementCount, Memory);
    Result &= DeserializeArray(Bytes, &Data->Mat, ElementCount, Memory);
  }
#endif

  return Result;
}











poof(serdes_vector(Quaternion))
#include <generated/serdes_vector_Quaternion.h>

poof(serdes_struct(keyframe))
#include <generated/serdes_struct_keyframe.h>

/* poof(serdes_struct(animation)) */
/* #include <generated/serdes_struct_animation.h> */

poof(serdes_struct(aabb))
#include <generated/serdes_struct_aabb.h>

poof(serdes_struct(random_series))
#include <generated/serdes_struct_random_series.h>

poof(serdes_struct(particle_system))
#include <generated/serdes_struct_particle_system.h>

poof(serdes_struct(vox_data))
#include <generated/serdes_struct_vox_data.h>

poof(serdes_struct(physics))
#include <generated/serdes_struct_physics.h>

poof(serdes_struct(model))
#include <generated/serdes_struct_model.h>

poof(serdes_struct(canonical_position))
#include <generated/serdes_struct_canonical_position.h>


link_internal b32 Serialize(native_file *File, particle_system *Element);
link_internal b32 Deserialize(u8_stream *Bytes, particle_system*);
link_internal particle_system * Deserialize_particle_system(u8_stream *Bytes);

poof(serdes_struct(file_traversal_node))
#include <generated/serdes_struct_file_traversal_node.h>

poof(serdes_struct(asset_slot))
#include <generated/serdes_struct_asset_slot.h>

poof(serdes_struct(asset_id))
#include <generated/serdes_struct_asset_id.h>

poof(serdes_struct(entity))
#include <generated/serdes_struct_entity.h>

poof( block_array(entity, {4}) )
#include <generated/block_array_entity_688856407.h>

link_internal cp
GetEntityBaseP(world *World, entity *Entity)
{
  cp BaseP = Entity->P;
  BaseP.Offset += Entity->_CollisionVolumeRadius.xy;
  BaseP = Canonicalize(World, BaseP);
  return BaseP;
}
