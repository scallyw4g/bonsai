link_internal b32
Serialize(native_file *File, canonical_position *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Offset);



  Result &= Serialize(File, &Element->WorldP);




  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, canonical_position *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->Offset, Memory);




  Result &= Deserialize(Bytes, &Element->WorldP, Memory);





  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

