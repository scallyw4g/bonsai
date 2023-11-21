link_internal b32
Serialize(native_file *File, aabb *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Min);



  Result &= Serialize(File, &Element->Max);




  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, aabb *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->Min, Memory);




  Result &= Deserialize(Bytes, &Element->Max, Memory);





  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


