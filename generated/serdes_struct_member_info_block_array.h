link_internal bonsai_type_info
TypeInfo(member_info_block_array *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("member_info_block_array");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("First"), CSz("First"), 0x31AD9C3B};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Current"), CSz("Current"), 0x293C01B2};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Memory"), CSz("Memory"), 0x19F638F4};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, member_info_block_array *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  if (Element->First) { Result &= WriteToFile(File, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
  else                        { Result &= WriteToFile(File, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }



  if (Element->Current) { Result &= WriteToFile(File, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
  else                        { Result &= WriteToFile(File, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }





  if (Element->First) { Result &= Serialize(File, Element->First); }


  if (Element->Current) { Result &= Serialize(File, Element->Current); }




  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, member_info_block_array *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_stream *Bytes, member_info_block_array *Element, memory_arena *Memory)
{
  b32 Result = True;
  b64 HadFirstPointer = Read_u64(Bytes);
  Assert(HadFirstPointer < 2); // Should be 0 or 1



  b64 HadCurrentPointer = Read_u64(Bytes);
  Assert(HadCurrentPointer < 2); // Should be 0 or 1





  if (HadFirstPointer)
  {
    if (Element->First == 0) { Element->First = Allocate(member_info_block, Memory, 1); }
    Result &= Deserialize(Bytes, Element->First, Memory);
  }


  if (HadCurrentPointer)
  {
    if (Element->Current == 0) { Element->Current = Allocate(member_info_block, Memory, 1); }
    Result &= Deserialize(Bytes, Element->Current, Memory);
  }



  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, member_info_block_array *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


