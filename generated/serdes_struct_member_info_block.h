link_internal bonsai_type_info
TypeInfo(member_info_block *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("member_info_block");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("Index"), CSz("Index"), 0x336ECEB7};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("At"), CSz("At"), 0x290F211E};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Elements"), CSz("Elements"), 0x2E81212C};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Next"), CSz("Next"), 0x2938F360};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, member_info_block *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->Index);





  Result &= Serialize(File, &Element->At);





  if (Element->Elements) { Result &= WriteToFile(File, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
  else                        { Result &= WriteToFile(File, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }



  if (Element->Next) { Result &= WriteToFile(File, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
  else                        { Result &= WriteToFile(File, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }

  if (Element->Elements) { Result &= Serialize(File, Element->Elements); }


  if (Element->Next) { Result &= Serialize(File, Element->Next); }

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, member_info_block *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_stream *Bytes, member_info_block *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Index, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->At, Memory);





  b64 HadElementsPointer = Read_u64(Bytes);
  Assert(HadElementsPointer < 2); // Should be 0 or 1



  b64 HadNextPointer = Read_u64(Bytes);
  Assert(HadNextPointer < 2); // Should be 0 or 1

  if (HadElementsPointer)
  {
    if (Element->Elements == 0) { Element->Elements = Allocate(member_info, Memory, 1); }
    Result &= Deserialize(Bytes, Element->Elements, Memory);
  }


  if (HadNextPointer)
  {
    if (Element->Next == 0) { Element->Next = Allocate(member_info_block, Memory, 1); }
    Result &= Deserialize(Bytes, Element->Next, Memory);
  }
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, member_info_block *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


