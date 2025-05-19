// src/engine/serdes.cpp:449:0

link_internal bonsai_type_info
TypeInfo(world_edit *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_edit");
  Result.Version =  0 ;

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
Serialize(u8_cursor_block_array *Bytes, world_edit *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_edit *Element = BaseElement + ElementIndex;
                                Result &= Serialize(Bytes, &Element->Region); // default






                if (Element->Brush) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }



                            Result &= Serialize(Bytes, &Element->Ordinal); // default

                    if (Element->Brush) { Result &= Serialize(Bytes, Element->Brush); }





    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory)
{
  b32 Result = True;
              // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Region, Memory);





        b64 HadBrushPointer = Read_u64(Bytes);
  Assert(HadBrushPointer < 2); // Should be 0 or 1



            // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Ordinal, Memory);

        if (HadBrushPointer)
  {
        umm Count = 1;


    if (Element->Brush == 0)
    {
      Element->Brush = Allocate(world_edit_brush, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->Brush, Memory, Count);
  }




  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


