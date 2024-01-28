link_internal bonsai_type_info
TypeInfo(bonsai_type_info_linked_list_node *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("bonsai_type_info_linked_list_node");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("Element"), CSz("Element"), 0x29301571};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Next"), CSz("Next"), 0x2938F360};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, bonsai_type_info_linked_list_node *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->Element);





  if (Element->Next) { Result &= WriteToFile(File, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
  else                        { Result &= WriteToFile(File, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }

  if (Element->Next) { Result &= Serialize(File, Element->Next); }

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, bonsai_type_info_linked_list_node *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_stream *Bytes, bonsai_type_info_linked_list_node *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Element, Memory);





  b64 HadNextPointer = Read_u64(Bytes);
  Assert(HadNextPointer < 2); // Should be 0 or 1

  if (HadNextPointer)
  {
    if (Element->Next == 0) { Element->Next = Allocate(bonsai_type_info_linked_list_node, Memory, 1); }
    Result &= Deserialize(Bytes, Element->Next, Memory);
  }
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, bonsai_type_info_linked_list_node *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


