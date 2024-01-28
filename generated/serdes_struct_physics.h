link_internal bonsai_type_info
TypeInfo(physics *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("physics");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("Velocity"), CSz("Velocity"), 0x380FA2D0};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Force"), CSz("Force"), 0x2A7B47CA};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Delta"), CSz("Delta"), 0x28C00979};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Mass"), CSz("Mass"), 0x29367621};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Speed"), CSz("Speed"), 0x2A06DE50};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, physics *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->Velocity);





  Result &= Serialize(File, &Element->Force);





  Result &= Serialize(File, &Element->Delta);





  Result &= Serialize(File, &Element->Mass);





  Result &= Serialize(File, &Element->Speed);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
DeserializeUnversioned(u8_stream *Bytes, physics *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Velocity, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Force, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Delta, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Mass, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Speed, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, physics *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


