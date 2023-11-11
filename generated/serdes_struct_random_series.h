link_internal b32
Serialize(native_file *File, random_series *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Seed);



  

  u64 Tag = LEVEL_FILE_OBJECT_DELIM;
  Ensure( Serialize(File, &Tag) );
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, random_series *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->Seed);




  

  u64 Tag = Read_u64(Bytes);
  Ensure( Tag == LEVEL_FILE_OBJECT_DELIM );
  return Result;
}


