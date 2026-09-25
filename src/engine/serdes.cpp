

poof(serdes_vector(v2))
#include <generated/serdes_vector$v2$Ow007Kor.h>

poof(serdes_vector(v3))
#include <generated/serdes_vector$v3$YL8cm15R.h>

poof(serdes_vector(v4))
#include <generated/serdes_vector$v4$CedHQjT0.h>

poof(serdes_vector(v2i))
#include <generated/serdes_vector$v2i$XXW5FQwq.h>

poof(serdes_vector(v3i))
#include <generated/serdes_vector$v3i$VhDk9UIl.h>

poof(serdes_primitive({chunk_flag}))
#include <generated/serdes_primitive$85387614$880VxtBi.h>

poof(serdes_cursor(v3))
#include <generated/serdes_cursor$v3$Ukl5CPl5.h>

poof(serdes_struct(canonical_position))
#include <generated/serdes_struct$canonical_position$7QGuSllj.h>

poof(serdes_struct(m4))
#include <generated/serdes_struct$m4$2f8nnvpY.h>

poof(serdes_struct(plane))
#include <generated/serdes_struct$plane$vb5il7Rv.h>

poof(serdes_struct(frustum))
#include <generated/serdes_struct$frustum$mtIlGOHQ.h>

#if 1
link_internal b32 Serialize(u8_cursor_block_array *File, untextured_3d_geometry_buffer *Data);
#endif


poof(serdes_struct(voxel))
#include <generated/serdes_struct$voxel$GkEiJ0Lm.h>

/* poof(serdes_array(voxel)) */
/* #include <generated/serdes_array_voxel.h> */

poof(serdes_struct(voxel_lighting))
#include <generated/serdes_struct$voxel_lighting$V9wtEKJF.h>
/* poof(serdes_array(voxel_lighting)) */
/* #include <generated/serdes_array_voxel_lighting.h> */

poof(serdes_struct(world_chunk_0))
#include <generated/serdes_struct$world_chunk_0$5Qes3E1i.h>

link_internal void
Marshal(world_chunk_0 *Stored, world_chunk *Live)
{
/* poof(default_marshal(world_chunk_0)) */
/* #include <generated/default_marshal_world_chunk_0.h> */
}


poof(serdes_struct(world_chunk))
#include <generated/serdes_struct$world_chunk$3mmclepX.h>


/* poof(serdes_array(v3)) */
/* #include <generated/serdes_array_v3.h> */

poof(serdes_struct(entity_id))
#include <generated/serdes_struct$entity_id$mdn0bR6X.h>


poof(deserialize_struct(camera_1))
#include <generated/deserialize_struct$camera_1$RgxFKKYa.h>

poof(deserialize_struct(camera_0))
#include <generated/deserialize_struct$camera_0$yVTUJmkF.h>

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
#include <generated/serdes_struct$camera$OO0EHFKT.h>

/* poof(deserialize_versioned_struct(camera, 0)) */
/* #include <generated/deserialize_versioned_struct_camera_1.h> */

poof(serdes_struct(vertex_material))
#include <generated/serdes_struct$vertex_material$RUiGgOvf.h>
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
    NotImplemented;
    /* Serialize(Bytes, Data->Voxels, ElementCount); */
    /* Serialize(Bytes, Data->VoxelLighting, ElementCount); */
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
  /* Data->Voxels = 0; */
  /* Data->VoxelLighting = 0; */

  if (ElementCount)
  {
    NotImplemented;
    /* Result &= Deserialize(Bytes, Data->Voxels, Memory, ElementCount); */
    /* Result &= Deserialize(Bytes, Data->VoxelLighting, Memory, ElementCount); */
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
#include <generated/serdes_vector$Quaternion$MQfOnbSb.h>

poof(serdes_struct(keyframe))
#include <generated/serdes_struct$keyframe$3NyC7Lr4.h>

/* poof(serdes_struct(animation)) */
/* #include <generated/serdes_struct_animation.h> */

poof(serdes_struct(rect3))
#include <generated/serdes_struct$rect3$bK4c8Ji7.h>

poof(serdes_struct(rect3i))
#include <generated/serdes_struct$rect3i$bsQSAwSl.h>

poof(serdes_struct(rect3cp))
#include <generated/serdes_struct$rect3cp$kjeKRmhv.h>

poof(serdes_struct(random_series))
#include <generated/serdes_struct$random_series$wfHnnXcb.h>

link_internal void
Marshal(particle_system_0 *Stored, particle_system *Live)
{
  poof(default_marshal(particle_system_0))
#include <generated/default_marshal$particle_system_0$1JJgFwO0.h>
}

poof(serdes_struct(particle_system_0))
#include <generated/serdes_struct$particle_system_0$xZAMCf9E.h>

poof(serdes_struct(particle_system))
#include <generated/serdes_struct$particle_system$nMD11tMe.h>

/* poof(serdes_struct(vox_data)) */
/* #include <generated/serdes_struct_vox_data.h> */

poof(serdes_struct(physics))
#include <generated/serdes_struct$physics$sYmB6h7B.h>

poof(serdes_struct(model))
#include <generated/serdes_struct$model$X28x0VnA.h>

poof(serdes_struct(file_traversal_node))
#include <generated/serdes_struct$file_traversal_node$32I68SnX.h>

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
#include <generated/serdes_struct$entity_1$RXJvqHQm.h>

poof(serdes_struct(entity_0))
#include <generated/serdes_struct$entity_0$PuM7ah95.h>

/* poof(deserialize_struct(entity)) */
/* #include <generated/deserialize_struct_entity.h> */

/* poof(serialize_struct(entity)) */
/* #include <generated/serialize_struct_entity.h> */



poof( block_array(entity, {4}) )
#include <generated/block_array$entity.688856407$uIeB4JW7.h>

poof(serdes_struct(lighting_settings))
#include <generated/serdes_struct$lighting_settings$2iZ2ktHK.h>

/* poof(serdes_struct(render_settings)) */
/* #include <generated/serdes_struct_render_settings.h> */

poof(serdes_struct(bonsai_type_info))
#include <generated/serdes_struct$bonsai_type_info$7evMpcMH.h>
poof(serdes_struct(bonsai_type_info_buffer))
#include <generated/serdes_struct$bonsai_type_info_buffer$BCCBevJh.h>

/* poof(deserialize_struct(level_header)) */
/* #include <generated/deserialize_struct_level_header.h> */

/* poof(serialize_struct(level_header)) */
/* #include <generated/serialize_struct_level_header.h> */
















poof(serdes_struct(white_noise_params))
#include <generated/serdes_struct$white_noise_params$l5rtfRjF.h>
poof(serdes_struct(rectangular_lattice_params))
#include <generated/serdes_struct$rectangular_lattice_params$hEgcJfgy.h>
poof(serdes_struct(perlin_noise_params))
#include <generated/serdes_struct$perlin_noise_params$c6Oj10Ws.h>
poof(serdes_struct(voronoi_noise_params))
#include <generated/serdes_struct$voronoi_noise_params$9WxHPtf4.h>

poof(serdes_struct(noise_layer_0))
#include <generated/serdes_struct$noise_layer_0$qAJGSDVC.h>
poof(serdes_struct(noise_layer_1))
#include <generated/serdes_struct$noise_layer_1$PUxv76uV.h>
poof(serdes_struct(noise_layer_2))
#include <generated/serdes_struct$noise_layer_2$WjHPy5ll.h>
poof(serdes_struct(noise_layer))
#include <generated/serdes_struct$noise_layer$jpBLr3Y4.h>

poof(serdes_struct(world_update_op_shape_params_asset))
#include <generated/serdes_struct$world_update_op_shape_params_asset$P0hV83Ov.h>
poof(serdes_struct(world_update_op_shape_params_chunk_data))
#include <generated/serdes_struct$world_update_op_shape_params_chunk_data$mllhQ2vq.h>

poof(serdes_struct(world_update_op_shape_params_rect))
#include <generated/serdes_struct$world_update_op_shape_params_rect$MAEd7UGK.h>
poof(serdes_struct(world_update_op_shape_params_sphere))
#include <generated/serdes_struct$world_update_op_shape_params_sphere$bX9hNjgE.h>
poof(serdes_struct(world_update_op_shape_params_line))
#include <generated/serdes_struct$world_update_op_shape_params_line$WEB1RcTj.h>
poof(serdes_struct(world_update_op_shape_params_cylinder))
#include <generated/serdes_struct$world_update_op_shape_params_cylinder$VIq1EJ5W.h>
poof(serdes_struct(world_update_op_shape_params_plane))
#include <generated/serdes_struct$world_update_op_shape_params_plane$jYT8FNFi.h>
poof(serdes_struct(world_update_op_shape_params_torus))
#include <generated/serdes_struct$world_update_op_shape_params_torus$qMcWWKsv.h>
poof(serdes_struct(world_update_op_shape_params_pyramid))
#include <generated/serdes_struct$world_update_op_shape_params_pyramid$dI2Y9OYo.h>
// @sdf_shape_step(3): Add new asset struct here to generate serialization code
//

poof(serdes_struct(shape_layer_advanced_params))
#include <generated/serdes_struct$shape_layer_advanced_params$cjms2La5.h>

poof(serdes_struct(shape_layer))
#include <generated/serdes_struct$shape_layer$CiQ4n3LV.h>



poof(
  for_datatypes(struct)
    func (type)
    {
      type.has_tag(serdes)?
      {
        serialize_decl(type)
      }
    }
)
#include <generated/poof_builtin.for_datatypes$$r3De9RED.h>


poof(
  for_datatypes(struct)
    func (type)
    {
      type.has_tag(serdes)?
      {
        type.has_tag(default_marshal)?
        {
          link_internal void
          Marshal( type.name *Stored, type.tag_value(default_marshal) *Live)
          {
            default_marshal(type)
          }
        }
      }
    }
)
#include <generated/poof_builtin.for_datatypes$$6lLoiPf7.h>

poof(serdes_struct(world_edit_block_array_index))
#include <generated/serdes_struct$world_edit_block_array_index$JmYX4FCQ.h>

poof(serdes_collection(world_edit, {block_array}))
#include <generated/serdes_collection$world_edit.399783746$tBuNYhf1.h>

poof(serdes_collection(world_edit_block_array_index, {block_array}))
#include <generated/serdes_collection$world_edit_block_array_index.399783746$xiHSdvUk.h>

poof(serdes_collection(base_ptr_relative_edit, {block_array}))
#include <generated/serdes_collection$base_ptr_relative_edit.399783746$X1lFr0Fs.h>

/* poof(serdes_collection(edit_record, {block_array})) */
/* #include <generated/serdes_collection_CfYBKxLq.h> */


poof(
  for_datatypes(struct)
    func (type)
    {
      type.has_tag(serdes)?
      {
        type.has_tag(collection)?
        {
          /// serdes_collection(type, type.tag_value(collection))
          /* serdes_collection(type, type.tag_value(collection)) */
        }
        {
          serialize_struct(type)
          deserialize_struct(type)
        }
      }
    }
)
#include <generated/poof_builtin.for_datatypes$$Z6wqlo1W.h>

/* poof(serdes_struct(world_edit_layer)) */
/* #include <generated/serdes_struct_world_edit_layer.h> */
