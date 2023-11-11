link_internal b32
Serialize(native_file *File, voxel *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Flags);


  Result &= Serialize(File, &Element->Color);


  Result &= Serialize(File, &Element->Transparency);



  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, voxel *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->Flags);



  Result &= Deserialize(Bytes, &Element->Color);



  Result &= Deserialize(Bytes, &Element->Transparency);




  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


