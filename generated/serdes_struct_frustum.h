link_internal bonsai_type_info
TypeInfo(frustum *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("frustum");

  {
    member_info Member = {CSz("farClip"), CSz("farClip"), 0x3B1ACDFB};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("nearClip"), CSz("nearClip"), 0x2BCE053A};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("width"), CSz("width"), 0x2FB608A};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("FOV"), CSz("FOV"), 0x290E95CE};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Top"), CSz("Top"), 0x290FB9C6};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Bot"), CSz("Bot"), 0x290FE598};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Left"), CSz("Left"), 0x29382DDC};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Right"), CSz("Right"), 0x31942685};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, frustum *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->farClip);





  Result &= Serialize(File, &Element->nearClip);





  Result &= Serialize(File, &Element->width);





  Result &= Serialize(File, &Element->FOV);





  Result &= Serialize(File, &Element->Top);





  Result &= Serialize(File, &Element->Bot);





  Result &= Serialize(File, &Element->Left);





  Result &= Serialize(File, &Element->Right);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}


link_internal b32
DeserializeUnversioned(u8_stream *Bytes, frustum *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->farClip, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->nearClip, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->width, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->FOV, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Top, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Bot, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Left, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Right, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, frustum *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


