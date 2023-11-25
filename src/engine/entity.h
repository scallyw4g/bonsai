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

  cp P;
  // NOTE(Jesse): This is a v4 because it used to be a quaternion, and the
  // savefiles have magic numbers so they have to deal with elements that are
  // the same size.  The w coordinate is unused
  v4 EulerAngles;
  r32 Scale;

  v3 CollisionVolumeRadius;
  physics Physics;

  model           *Model;
  particle_system *Emitter;

  entity_state          State;
  entity_behavior_flags Behavior;

  u64 UserType;
  umm UserData;
};

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












#if 0
link_internal vox_data*
Deserialize_vox_data(u8_stream *Bytes)
{
  vox_data *Result = 0;

  NotImplemented;

  umm ElementCount = Cast(umm, Volume(Result->Dim));

  if (Result->Voxels) { Result->Voxels, DeserializeArray_voxel(Bytes, ElementCount); }
  if (Result->VoxelLighting) { Result->VoxelLighting, DeserializeArray_voxel_lighting(Bytes, ElementCount); }

  return Result;
}
link_internal chunk_data*
Deserialize_chunk_data(u8_stream *Bytes)
{
  chunk_data *Result = 0;
  NotImplemented;
  return Result;
}

link_internal b32
Deserialize_untextured_3d_geometry_buffer(u8_stream *Bytes, untextured_3d_geometry_buffer *Data)
{
  NotImplemented;
  return 0;
}
#endif



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

poof(serdes_struct(entity))
#include <generated/serdes_struct_entity.h>

poof( block_array(entity, {4}) )
#include <generated/block_array_entity_688856407.h>
