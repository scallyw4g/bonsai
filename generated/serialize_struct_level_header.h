link_internal bonsai_type_info
TypeInfo(level_header *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("level_header");
  Result.Version =2 ;

  /* type.map(member) */
  /* { */
  /*   { */
  /*     member_info Member = {CSz("member.name"), CSz("member.name"), 0x(member.hash)}; */
  /*     Push(&Result.Members, &Member); */
  /*   } */
  /* } */

  return Result;
}

link_internal b32
Serialize(native_file *File, level_header *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Upsert(TypeInfo(Element), &Global_SerializeTypeTable);
  u64 VersionNumber =2;
  Serialize(File, &VersionNumber);


  Result &= Serialize(File, &Element->Version);





  Result &= Serialize(File, &Element->ChunkCount);





  Result &= Serialize(File, &Element->EntityCount);





  Result &= Serialize(File, &Element->WorldFlags);





  Result &= Serialize(File, &Element->WorldCenter);





  Result &= Serialize(File, &Element->VisibleRegion);





  Result &= Serialize(File, &Element->Camera);





  Result &= Serialize(File, &Element->CameraTarget);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

