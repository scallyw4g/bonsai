link_internal b32
Serialize(native_file *File, asset_slot *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Index);



  Result &= Serialize(File, &Element->Generation);




  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, asset_slot *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->Index);




  Result &= Deserialize(Bytes, &Element->Generation);





  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


