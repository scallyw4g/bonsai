

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

poof(serdes_struct(entity_id))
#include <generated/serdes_struct_entity_id.h>


poof(deserialize_struct(camera_0))
#include <generated/deserialize_struct_camera_0.h>

link_internal void
Marshal(camera_0 *C0, camera *C1)
{

  C1->Frust = C0->Frust;
  C1->CurrentP = C0->CurrentP;
  C1->RenderSpacePosition = C0->RenderSpacePosition;
  C1->Pitch = C0->Pitch;
  C1->Roll = C0->Roll;
  C1->Yaw = C0->Yaw;
  C1->DistanceFromTarget = C0->DistanceFromTarget;
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

poof(deserialize_versioned_struct(camera, 0))
#include <generated/deserialize_versioned_struct_camera_1.h>

poof(serdes_struct(camera))
#include <generated/serdes_struct_camera.h>

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

poof(serdes_struct(file_traversal_node))
#include <generated/serdes_struct_file_traversal_node.h>

/* poof(serdes_struct(asset_slot)) */
/* #include <generated/serdes_struct_asset_slot.h> */

poof(serdes_struct(asset_id))
#include <generated/serdes_struct_asset_id.h>

poof(deserialize_struct(entity))
#include <generated/deserialize_struct_entity.h>
poof(serialize_struct(entity))
#include <generated/serialize_struct_entity.h>
poof(serdes_struct(entity_1))
#include <generated/serdes_struct_entity_1.h>
poof(serdes_struct(entity_0))
#include <generated/serdes_struct_entity_0.h>

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

link_internal cp
GetEntityCenterP(world *World, entity *Entity)
{
  cp BaseP = Entity->P;
  BaseP.Offset += Entity->_CollisionVolumeRadius;
  BaseP = Canonicalize(World, BaseP);
  return BaseP;
}



link_internal void
Marshal(entity_1 *E1, entity *E)
{
  // NOTE(Jesse): Poor-mans stand-in for deep-comparing the types in poof
  CAssert(sizeof(entity_1) == sizeof(entity));

  CopyMemory(Cast(u8*, E1), Cast(u8*, E), sizeof(entity));
}

link_internal void
Marshal(entity_0 *E0, entity_1 *E1)
{
  poof(
    func (entity_0 e0_t)
    {
      e0_t.map_members(member)
      {
        member.is_named(Carrying)?
        {
          // E0 Doesn't have the Carrying member
        }
        {
          E1->member.name = E0->member.name;
        }
      }
    }
  )
#include <generated/anonymous_entity_0_AfIY0cwC.h>
}

#if 0
link_internal void
Marshal(entity_1 *E0, entity_2 *E1)
{
  E1->Id = E0->Id;
  E1->P = E0->P;
  E1->EulerAngles = E0->EulerAngles.xyz;
  E1->Scale = E0->Scale;
  E1->_CollisionVolumeRadius = E0->_CollisionVolumeRadius;
  E1->Physics = E0->Physics;

  E1->AssetId = E0->AssetId;
  E1->AssetId.Index = INVALID_ASSET_INDEX;

  E1->Emitter = E0->Emitter;
  E1->State = E0->State;
  E1->Behavior = E0->Behavior;
  E1->UserType = E0->UserType;
  E1->UserData = E0->UserData;
}
#endif

/* poof(serdes_struct(member_info)) */
/* #include <generated/serdes_struct_member_info.h> */
/* poof(serdes_struct(member_info_block)) */
/* #include <generated/serdes_struct_member_info_block.h> */
/* poof(serdes_struct(member_info_block_array)) */
/* #include <generated/serdes_struct_member_info_block_array.h> */
poof(serdes_struct(bonsai_type_info))
#include <generated/serdes_struct_bonsai_type_info.h>
poof(serdes_struct(bonsai_type_info_buffer))
#include <generated/serdes_struct_bonsai_type_info_buffer.h>

/* poof(serdes_struct(bonsai_type_info_linked_list_node)) */
/* #include <generated/serdes_struct_bonsai_type_info_linked_list_node.h> */
/* poof(serdes_struct(bonsai_type_info_hashtable)) */
/* #include <generated/serdes_struct_bonsai_type_info_hashtable.h> */

/* poof(serdes_struct(level_header)) */
/* #include <generated/serdes_struct_level_header.h> */


link_internal void
Marshal(level_header_0 *E0, level_header *E1)
{
  E1->ChunkCount = E0->ChunkCount;
  E1->EntityCount = E0->EntityCount;
  E1->WorldFlags = E0->WorldFlags;
  E1->WorldCenter = E0->WorldCenter;
  E1->VisibleRegion = E0->VisibleRegion;
  E1->Camera = E0->Camera;
  E1->CameraTarget = E0->CameraTarget;
}

link_internal void
Marshal(level_header_1 *E0, level_header *E1)
{
  E1->ChunkCount = E0->ChunkCount;
  E1->EntityCount = E0->EntityCount;
  E1->WorldFlags = E0->WorldFlags;
  E1->WorldCenter = E0->WorldCenter;
  E1->VisibleRegion = E0->VisibleRegion;
  E1->Camera = E0->Camera;
  E1->CameraTarget = E0->CameraTarget;
}



poof(deserialize_struct(level_header_0))
#include <generated/deserialize_struct_level_header_0.h>

poof(deserialize_struct(level_header_1))
#include <generated/deserialize_struct_level_header_1.h>

poof(deserialize_versioned_struct(level_header, 2))
#include <generated/deserialize_versioned_struct_level_header_0.h>

poof(deserialize_struct(level_header))
#include <generated/deserialize_struct_level_header.h>


poof(serialize_struct(level_header))
#include <generated/serialize_struct_level_header.h>



#if 0
link_internal b32
Deserialize(u8_stream *Bytes, entity *Element, memory_arena *Memory)
{
  b32 Result = True;

  u64 Version;
  Result &= Deserialize(Bytes, &Version, Memory);

  if (Result)
  {
    switch(Version)
    {
      case 0:
      {
        entity_0 E0 = {};
        entity_1 E1 = {};
        Deserialize(Bytes, &E0, Memory);
        Marshal(&E0, &E1);
        Marshal(&E1, Element);
      } break;

      case 1:
      {
        entity_1 E1 = {};
        Deserialize(Bytes, &E1, Memory);
        Marshal(&E1, Element);
      } break;

      default: { SoftError("Could not deserialize entity, got invalid version (%lu)", Version); }
    }


  }

  Element->AssetId.Index = INVALID_ASSET_INDEX;

  return Result;
}
#endif


link_internal b32
DeserializeVersioned(u8_stream *Bytes, entity *Element, bonsai_type_info *TypeInfo, u64 Version, memory_arena *Memory)
{
  b32 Result = True;

  if (Result)
  {
    switch(Version)
    {
      case 0:
      {
        entity_0 E0 = {};
        entity_1 E1 = {};
        Deserialize(Bytes, &E0, Memory);
        Marshal(&E0, &E1);
        Marshal(&E1, Element);
      } break;

      case 1:
      {
        entity_1 E1 = {};
        Deserialize(Bytes, &E1, Memory);
        Marshal(&E1, Element);
      } break;

      default: { SoftError("Could not deserialize entity, got invalid version (%lu)", Version); }
    }


  }

  Element->AssetId.Index = INVALID_ASSET_INDEX;

  return Result;
}

