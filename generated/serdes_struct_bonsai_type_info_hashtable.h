link_internal bonsai_type_info
TypeInfo(bonsai_type_info_hashtable *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("bonsai_type_info_hashtable");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("Size"), CSz("Size"), 0x2916F6E8};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Elements"), CSz("Elements"), 0x2E81212C};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, bonsai_type_info_hashtable *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->Size);





  if (Element->Elements) { Result &= WriteToFile(File, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
  else                        { Result &= WriteToFile(File, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }

  if (Element->Elements) { Result &= Serialize(File, Element->Elements); }

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, bonsai_type_info_hashtable *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_stream *Bytes, bonsai_type_info_hashtable *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Size, Memory);





  b64 HadElementsPointer = Read_u64(Bytes);
  Assert(HadElementsPointer < 2); // Should be 0 or 1

  if (HadElementsPointer)
  {
    if (Element->Elements == 0) { Element->Elements = Allocate(bonsai_type_info_linked_list_node, Memory, 1); }
    Result &= Deserialize(Bytes, Element->Elements, Memory);
  }
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, bonsai_type_info_hashtable *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


