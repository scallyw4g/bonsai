link_internal b32
Serialize(native_file *File, model *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Vox);



  Result &= Serialize(File, &Element->Mesh);



  Result &= Serialize(File, &Element->TransparentMesh);



  Result &= Serialize(File, &Element->Animation);



  Result &= Serialize(File, &Element->Dim);




  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, model *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->Vox, Memory);




  Result &= Deserialize(Bytes, &Element->Mesh, Memory);




  Result &= Deserialize(Bytes, &Element->TransparentMesh, Memory);




  Result &= Deserialize(Bytes, &Element->Animation, Memory);




  Result &= Deserialize(Bytes, &Element->Dim, Memory);





  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


