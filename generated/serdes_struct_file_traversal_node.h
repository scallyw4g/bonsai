link_internal b32
Serialize(native_file *File, file_traversal_node *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, (u32*)&Element->Type);


  Result &= Serialize(File, &Element->Name);



  Result &= Serialize(File, &Element->Dir);




  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, file_traversal_node *Element, memory_arena *Memory)
{
  b32 Result = True;
  Element->Type = Cast(file_traversal_type, Read_u32(Bytes));


  Result &= Deserialize(Bytes, &Element->Name);




  Result &= Deserialize(Bytes, &Element->Dir);





  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


