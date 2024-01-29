link_internal bonsai_type_info
TypeInfo(bonsai_type_info_buffer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("bonsai_type_info_buffer");
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
Serialize(u8_cursor_block_array *Bytes, bonsai_type_info_buffer *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  if (Element->Start) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
  else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }



  Result &= Serialize(Bytes, &Element->Count);

  if (Element->Start) { Result &= Serialize(Bytes, Element->Start); }




  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, bonsai_type_info_buffer *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_cursor *Bytes, bonsai_type_info_buffer *Element, memory_arena *Memory)
{
  b32 Result = True;
  b64 HadStartPointer = Read_u64(Bytes);
  Assert(HadStartPointer < 2); // Should be 0 or 1



  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Count, Memory);

  if (HadStartPointer)
  {
    if (Element->Start == 0) { Element->Start = Allocate(bonsai_type_info, Memory, 1); }
    Result &= Deserialize(Bytes, Element->Start, Memory);
  }



  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, bonsai_type_info_buffer *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


