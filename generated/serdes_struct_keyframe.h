link_internal b32
Serialize(native_file *File, keyframe *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->tEnd);



  Result &= Serialize(File, &Element->Value);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, keyframe *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->tEnd);




  Result &= Deserialize(Bytes, &Element->Value);

  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


