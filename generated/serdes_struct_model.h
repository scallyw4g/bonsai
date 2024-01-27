link_internal bonsai_type_info
TypeInfo(model *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("model");

  {
    member_info Member = {CSz("Vox"), CSz("Vox"), 0x29101190};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Meshes"), CSz("Meshes"), 0x310AD0AF};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("TransparentMesh"), CSz("TransparentMesh"), 0x321C682D};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Animation"), CSz("Animation"), 0x34AA15C5};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Dim"), CSz("Dim"), 0x290F978D};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, model *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->Vox);





  Result &= Serialize(File, &Element->TransparentMesh);





  Result &= Serialize(File, &Element->Animation);





  Result &= Serialize(File, &Element->Dim);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}


link_internal b32
DeserializeUnversioned(u8_stream *Bytes, model *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Vox, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->TransparentMesh, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Animation, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Dim, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, model *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


