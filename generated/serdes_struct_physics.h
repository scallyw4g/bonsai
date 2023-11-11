link_internal b32
Serialize(native_file *File, physics *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Velocity);


  Result &= Serialize(File, &Element->Force);


  Result &= Serialize(File, &Element->Delta);


  Result &= Serialize(File, &Element->Mass);


  Result &= Serialize(File, &Element->Speed);



  

  u64 Tag = LEVEL_FILE_OBJECT_DELIM;
  Ensure( Serialize(File, &Tag) );
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, physics *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->Velocity, Memory);



  Result &= Deserialize(Bytes, &Element->Force, Memory);



  Result &= Deserialize(Bytes, &Element->Delta, Memory);



  Result &= Deserialize(Bytes, &Element->Mass);



  Result &= Deserialize(Bytes, &Element->Speed);




  

  u64 Tag = Read_u64(Bytes);
  Ensure( Tag == LEVEL_FILE_OBJECT_DELIM );
  return Result;
}


