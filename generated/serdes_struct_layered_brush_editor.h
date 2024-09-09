// src/engine/serdes.cpp:547:0

link_internal bonsai_type_info
TypeInfo(layered_brush_editor *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("layered_brush_editor");
  Result.Version =3 ;

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
Serialize(u8_cursor_block_array *Bytes, layered_brush_editor *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  Upsert(TypeInfo(BaseElement), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber =3;
  Serialize(Bytes, &VersionNumber);


  RangeIterator_t(umm, ElementIndex, Count)
  {
    layered_brush_editor *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->LayerCount);





    {
      // TODO(Jesse): Should this really be a safe cast?
      umm ThisCount = umm(Element->LayerCount);

      Result &= Serialize(Bytes, Element->Layers, ThisCount);
    }





    Result &= Serialize(Bytes, &Element->SeedBrushWithSelection);





    Result &= Serialize(Bytes, &Element->BrushFollowsCursor);





    Result &= Serialize(Bytes, (u32*)&Element->Mode);




    Result &= Serialize(Bytes, (u32*)&Element->Modifier);






    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, layered_brush_editor *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layered_brush_editor *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, layered_brush_editor *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <=3);

  b32 Result = True;

  if (TypeInfo->Version == 0)
  {
    layered_brush_editor_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }
  if (TypeInfo->Version == 1)
  {
    layered_brush_editor_1 T1 = {};
    Result &= Deserialize(Bytes, &T1, Memory);
    Marshal(&T1, Element);
  }
  if (TypeInfo->Version == 2)
  {
    layered_brush_editor_2 T2 = {};
    Result &= Deserialize(Bytes, &T2, Memory);
    Marshal(&T2, Element);
  }


  if (TypeInfo->Version ==3)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element, Memory);
  }

  return Result;
}


link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layered_brush_editor *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->LayerCount, Memory);





  {
    // TODO(Jesse): Should this really be a safe cast?
    umm Count = umm(Element->LayerCount);

    Result &= Deserialize(Bytes, Element->Layers, Memory, Count);
  }





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->SeedBrushWithSelection, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->BrushFollowsCursor, Memory);





  Element->Mode = Cast(world_edit_mode, Read_u32(Bytes));




  Element->Modifier = Cast(world_edit_mode_modifier, Read_u32(Bytes));






  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, layered_brush_editor *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
    maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("layered_brush_editor"));

    if (MaybeSerializedType.Tag)
    {
      u64 OldIgnoredVersionNumber;
      if (MaybeSerializedType.Value.Version > 0)
      {
        Deserialize(Bytes, &OldIgnoredVersionNumber, Memory);
      }
      Result &= DeserializeVersioned(Bytes, Element+ElementIndex, &MaybeSerializedType.Value, Memory);
    }
    else
    {
      bonsai_type_info T0TypeInfo = {};
      Result &= DeserializeVersioned(Bytes, Element+ElementIndex, &T0TypeInfo, Memory);
    }

  }

  return Result;
}


