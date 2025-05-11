

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

poof(serdes_struct(canonical_position))
#include <generated/serdes_struct_canonical_position.h>

poof(serdes_struct(plane))
#include <generated/serdes_struct_plane.h>

poof(serdes_struct(frustum))
#include <generated/serdes_struct_frustum.h>

#if 1
link_internal b32 Serialize(u8_cursor_block_array *File, untextured_3d_geometry_buffer *Data);
#endif


poof(serdes_struct(voxel))
#include <generated/serdes_struct_voxel.h>

/* poof(serdes_array(voxel)) */
/* #include <generated/serdes_array_voxel.h> */

poof(serdes_struct(voxel_lighting))
#include <generated/serdes_struct_voxel_lighting.h>
/* poof(serdes_array(voxel_lighting)) */
/* #include <generated/serdes_array_voxel_lighting.h> */

poof(serdes_struct(world_chunk_0))
#include <generated/serdes_struct_world_chunk_0.h>

link_internal void
Marshal(world_chunk_0 *Stored, world_chunk *Live)
{
poof(default_marshal(world_chunk_0))
#include <generated/default_marshal_world_chunk_0.h>
}


poof(serdes_struct(world_chunk))
#include <generated/serdes_struct_world_chunk.h>


/* poof(serdes_array(v3)) */
/* #include <generated/serdes_array_v3.h> */

poof(serdes_struct(entity_id))
#include <generated/serdes_struct_entity_id.h>


poof(deserialize_struct(camera_1))
#include <generated/deserialize_struct_camera_1.h>

poof(deserialize_struct(camera_0))
#include <generated/deserialize_struct_camera_0.h>

link_internal void
Marshal(camera_1 *C0, camera *C1)
{
  C1->Frust = C0->Frust;
  C1->CurrentP = C0->CurrentP;
  C1->RenderSpacePosition = C0->RenderSpacePosition;

  C1->TargetPitch = C0->Pitch;
  C1->TargetRoll = C0->Roll;
  C1->TargetYaw = C0->Yaw;
  C1->TargetDistanceFromTarget = C0->DistanceFromTarget;

  C1->Front = C0->Front;
  C1->Right = C0->Right;
  C1->Up = C0->Up;
  C1->GhostId = C0->GhostId;;
}

link_internal void
Marshal(camera_0 *C0, camera *C1)
{

  C1->Frust = C0->Frust;
  C1->CurrentP = C0->CurrentP;
  C1->RenderSpacePosition = C0->RenderSpacePosition;

  C1->TargetPitch = C0->Pitch;
  C1->TargetRoll = C0->Roll;
  C1->TargetYaw = C0->Yaw;
  C1->TargetDistanceFromTarget = C0->DistanceFromTarget;

  C1->Front = C0->Front;
  C1->Right = C0->Right;
  C1->Up = C0->Up;
  C1->GhostId = C0->GhostId;;

  // TODO(Jesse)(poof): Make this work
#if 0
  poof(
    func(camera_0, camrea)
    {
      camera_0.map(member)
      {
        camera.contains(member)?
        {
          C0->c0_member = C1->c0_member;
        }
      }
    }
  )
#endif
}

poof(serdes_struct(camera))
#include <generated/serdes_struct_camera.h>

/* poof(deserialize_versioned_struct(camera, 0)) */
/* #include <generated/deserialize_versioned_struct_camera_1.h> */

poof(serdes_struct(vertex_material))
#include <generated/serdes_struct_vertex_material.h>
/* poof(serdes_array(vertex_material)) */
/* #include <generated/serdes_array_vertex_material.h> */


link_internal b32
Serialize(u8_cursor_block_array *Bytes, chunk_data *Data, umm Count = 1)
{
  Assert(Count == 1);
  b32 Result = True;

#if 1
  umm ElementCount = umm(Volume(Data->Dim));

  Result &= Write(Bytes, Cast(u8*, Data), sizeof(chunk_data));

  if (ElementCount)
  {
    Serialize(Bytes, Data->Voxels, ElementCount);
    Serialize(Bytes, Data->VoxelLighting, ElementCount);
  }
#endif

  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, chunk_data *Data, memory_arena *Memory, umm Count = 1)
{
  Assert(Count == 1);
  b32 Result = True;

#if 1
  Result &= ReadBytesIntoBuffer(Bytes, Cast(u8*, Data), sizeof(chunk_data));

  umm ElementCount = umm(Volume(Data->Dim));
  Data->Voxels = 0;
  Data->VoxelLighting = 0;

  if (ElementCount)
  {
    Result &= Deserialize(Bytes, Data->Voxels, Memory, ElementCount);
    Result &= Deserialize(Bytes, Data->VoxelLighting, Memory, ElementCount);
  }
#endif

  return Result;
}

// TODO(Jesse): Do these for-realz?  Mark @no_serialize?
link_internal b32
Serialize(u8_cursor_block_array *File, animation *Data)
{
  return True;
}

link_internal b32
Deserialize(u8_stream *Bytes, animation *Data, memory_arena *Memory)
{
  return True;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, untextured_3d_geometry_buffer *Data)
{
  b32 Result = True;

#if 1
  Result &= Write(Bytes, Cast(u8*, Data), sizeof(untextured_3d_geometry_buffer));

  umm ElementCount = umm(Data->At);
  if (ElementCount)
  {
    u32 ElementSize = DataTypeToElementSize[Data->Type];
    Result &= Serialize(Bytes, Cast(u8*, Data->Verts),   ElementSize*ElementCount);
    Result &= Serialize(Bytes, Cast(u8*, Data->Normals), ElementSize*ElementCount);
    Result &= Serialize(Bytes,           Data->Mat,                  ElementCount);
  }
#endif

  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, untextured_3d_geometry_buffer *Data, memory_arena *Memory)
{
  b32 Result = True;
#if 1
  Result &= ReadBytesIntoBuffer(Bytes, Cast(u8*, Data), sizeof(untextured_3d_geometry_buffer));

  umm ElementCount = Data->At;

  Data->Verts = 0;
  Data->Normals = 0;
  Data->Mat = 0;

  if (ElementCount)
  {
    u32 ElementSize = DataTypeToElementSize[Data->Type];
    Result &= Deserialize(Bytes, Cast(u8*, Data->Verts),   Memory, ElementSize*ElementCount);
    Result &= Deserialize(Bytes, Cast(u8*, Data->Normals), Memory, ElementSize*ElementCount);
    Result &= Deserialize(Bytes,           Data->Mat,      Memory,             ElementCount);
  }
#endif

  return Result;
}

link_internal b32
DeserializeVersioned(u8_stream *Bytes, entity *Element, bonsai_type_info *TypeInfo, u64 Version, memory_arena *Memory);












poof(serdes_vector(Quaternion))
#include <generated/serdes_vector_Quaternion.h>

poof(serdes_struct(keyframe))
#include <generated/serdes_struct_keyframe.h>

/* poof(serdes_struct(animation)) */
/* #include <generated/serdes_struct_animation.h> */

poof(serdes_struct(rect3))
#include <generated/serdes_struct_rect3.h>

poof(serdes_struct(rect3i))
#include <generated/serdes_struct_rect3i.h>

poof(serdes_struct(rect3cp))
#include <generated/serdes_struct_rect3cp.h>

poof(serdes_struct(random_series))
#include <generated/serdes_struct_random_series.h>

link_internal void
Marshal(particle_system_0 *Stored, particle_system *Live)
{
  poof(default_marshal(particle_system_0))
#include <generated/default_marshal_particle_system_0.h>
}

poof(serdes_struct(particle_system_0))
#include <generated/serdes_struct_particle_system_0.h>

poof(serdes_struct(particle_system))
#include <generated/serdes_struct_particle_system.h>

poof(serdes_struct(vox_data))
#include <generated/serdes_struct_vox_data.h>

poof(serdes_struct(physics))
#include <generated/serdes_struct_physics.h>

poof(serdes_struct(model))
#include <generated/serdes_struct_model.h>

poof(serdes_struct(file_traversal_node))
#include <generated/serdes_struct_file_traversal_node.h>

/* poof(serdes_struct(asset_slot)) */
/* #include <generated/serdes_struct_asset_slot.h> */

// Had to put a hack in to set (asset_id::Index = INVALID_ASSET_INDEX)
/* poof(serdes_struct(asset_id)) */
#include <generated/serdes_struct_asset_id.h>




link_internal void
Marshal(entity_1 *E0, entity *E1)
{
  E1->Id = E0->Id;
  E1->P = E0->P;
  E1->EulerAngles = E0->EulerAngles;
  E1->Scale = E0->Scale;
  E1->_CollisionVolumeRadius = E0->_CollisionVolumeRadius;
  E1->Physics = E0->Physics;
  E1->AssetId = E0->AssetId;
  E1->ModelIndex = E0->ModelIndex;
  E1->LastResolvedCollision = E0->LastResolvedCollision;
  E1->LastResolvedPosInfo = E0->LastResolvedPosInfo;
  E1->Emitter = E0->Emitter;
  E1->State = E0->State;
  E1->Behavior = E0->Behavior;
  E1->UserType = E0->UserType;
  E1->UserData = E0->UserData;
}

link_internal void
Marshal(entity_0 *E0, entity *E1)
{
  E1->Id = E0->Id;
  E1->P = E0->P;
  E1->EulerAngles = E0->EulerAngles;
  E1->Scale = E0->Scale;
  E1->_CollisionVolumeRadius = E0->_CollisionVolumeRadius;
  E1->Physics = E0->Physics;
  E1->AssetId = E0->AssetId;
  E1->ModelIndex = E0->ModelIndex;
  E1->LastResolvedCollision = E0->LastResolvedCollision;
  E1->LastResolvedPosInfo = E0->LastResolvedPosInfo;
  E1->Emitter = E0->Emitter;
  E1->State = E0->State;
  E1->Behavior = E0->Behavior;
  E1->UserType = E0->UserType;
  E1->UserData = E0->UserData;
}

poof(serdes_struct(entity_1))
#include <generated/serdes_struct_entity_1.h>

poof(serdes_struct(entity_0))
#include <generated/serdes_struct_entity_0.h>

poof(deserialize_struct(entity))
#include <generated/deserialize_struct_entity.h>

poof(serialize_struct(entity))
#include <generated/serialize_struct_entity.h>



poof( block_array(entity, {4}) )
#include <generated/block_array_entity_688856407.h>

poof(serdes_struct(lighting_settings))
#include <generated/serdes_struct_lighting_settings.h>

poof(serdes_struct(render_settings))
#include <generated/serdes_struct_render_settings.h>

poof(serdes_struct(bonsai_type_info))
#include <generated/serdes_struct_bonsai_type_info.h>
poof(serdes_struct(bonsai_type_info_buffer))
#include <generated/serdes_struct_bonsai_type_info_buffer.h>

poof(deserialize_struct(level_header))
#include <generated/deserialize_struct_level_header.h>

poof(serialize_struct(level_header))
#include <generated/serialize_struct_level_header.h>
















poof(serdes_struct(white_noise_params))
#include <generated/serdes_struct_white_noise_params.h>
poof(serdes_struct(perlin_noise_params))
#include <generated/serdes_struct_perlin_noise_params.h>
poof(serdes_struct(voronoi_noise_params))
#include <generated/serdes_struct_voronoi_noise_params.h>
poof(serdes_struct(noise_layer_0))
#include <generated/serdes_struct_noise_layer_0.h>
poof(serdes_struct(noise_layer))
#include <generated/serdes_struct_noise_layer.h>

poof(serdes_struct(world_update_op_shape_params_rect))
#include <generated/serdes_struct_world_update_op_shape_params_rect.h>
poof(serdes_struct(world_update_op_shape_params_sphere))
#include <generated/serdes_struct_world_update_op_shape_params_sphere.h>
poof(serdes_struct(world_update_op_shape_params_cylinder))
#include <generated/serdes_struct_world_update_op_shape_params_cylinder.h>
poof(serdes_struct(world_update_op_shape_params_asset))
#include <generated/serdes_struct_world_update_op_shape_params_asset.h>
poof(serdes_struct(world_update_op_shape_params_chunk_data))
#include <generated/serdes_struct_world_update_op_shape_params_chunk_data.h>
// @dottedboxguy (Step3) Add new asset struct here to generate serialization code
//

poof(serdes_struct(shape_layer))
#include <generated/serdes_struct_shape_layer.h>


poof(serdes_struct(brush_settings))
#include <generated/serdes_struct_brush_settings.h>

poof(serdes_struct(brush_layer))
#include <generated/serdes_struct_brush_layer.h>

/* poof(serdes_dunion(world_edit_shape)) */
/* #include <generated/serdes_world_edit_shape.h> */

poof(serdes_struct(layered_brush))
#include <generated/serdes_struct_layered_brush.h>

poof(serdes_struct(world_edit_brush))
#include <generated/serdes_struct_world_edit_brush.h>

poof(serdes_struct(world_edit))
#include <generated/serdes_struct_world_edit.h>
