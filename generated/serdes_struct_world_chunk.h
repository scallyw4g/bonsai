// src/engine/serdes.cpp:49:0

link_internal bonsai_type_info
TypeInfo(world_chunk *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_chunk");
  Result.Version = 0 ;

  /* type.map(member) */
  /* { */
  /*   { */
  /*     member_info Member = {CSz("member.name"), CSz("member.name"), 0x(member.hash)}; */
  /*     Push(&Result.Members, &Member); */
  /*   } */
  /* } */

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_chunk *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_chunk *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->Dim);





    if (Element->Voxels) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }



    if (Element->VoxelLighting) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }



    Result &= Serialize(Bytes, &Element->WorldP);







    if (Element->Voxels) { Result &= Serialize(Bytes, Element->Voxels, Cast(umm, Volume(Element->Dim)) ); }



    if (Element->VoxelLighting) { Result &= Serialize(Bytes, Element->VoxelLighting, Cast(umm, Volume(Element->Dim)) ); }





    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Dim, Memory);





  b64 HadVoxelsPointer = Read_u64(Bytes);
  Assert(HadVoxelsPointer < 2); // Should be 0 or 1



  b64 HadVoxelLightingPointer = Read_u64(Bytes);
  Assert(HadVoxelLightingPointer < 2); // Should be 0 or 1



  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->WorldP, Memory);







  if (HadVoxelsPointer)
  {
    umm Count = Cast(umm, Volume(Element->Dim));


    if (Element->Voxels == 0)
    {
      Element->Voxels = Allocate(voxel, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->Voxels, Memory, Count);
  }


  if (HadVoxelLightingPointer)
  {
    umm Count = Cast(umm, Volume(Element->Dim));


    if (Element->VoxelLighting == 0)
    {
      Element->VoxelLighting = Allocate(voxel_lighting, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->VoxelLighting, Memory, Count);
  }




  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


