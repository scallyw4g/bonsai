link_internal b32
Serialize(native_file *File, asset_id *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->FileNode);



  Result &= Serialize(File, &Element->SlotIndex);




  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, asset_id *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->FileNode, Memory);




  Result &= Deserialize(Bytes, &Element->SlotIndex);





  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


