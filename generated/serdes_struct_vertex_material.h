link_internal b32
Serialize(native_file *File, vertex_material *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Color);


  Result &= Serialize(File, &Element->Transparency);


  Result &= Serialize(File, &Element->Emission);



  

  u64 Tag = LEVEL_FILE_OBJECT_DELIM;
  Ensure( Serialize(File, &Tag) );
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, vertex_material *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->Color, Memory);



  Result &= Deserialize(Bytes, &Element->Transparency);



  Result &= Deserialize(Bytes, &Element->Emission);




  

  u64 Tag = Read_u64(Bytes);
  Ensure( Tag == LEVEL_FILE_OBJECT_DELIM );
  return Result;
}


