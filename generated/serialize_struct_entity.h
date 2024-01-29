link_internal bonsai_type_info
TypeInfo(entity *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("entity");
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
Serialize(native_file *File, entity *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Upsert(TypeInfo(Element), &Global_SerializeTypeTable);
  u64 VersionNumber =2;
  Serialize(File, &VersionNumber);


  Result &= Serialize(File, &Element->Id);





  Result &= Serialize(File, &Element->P);





  Result &= Serialize(File, &Element->EulerAngles);





  Result &= Serialize(File, &Element->Scale);





  Result &= Serialize(File, &Element->_CollisionVolumeRadius);





  Result &= Serialize(File, &Element->Physics);





  Result &= Serialize(File, &Element->AssetId);





  Result &= Serialize(File, &Element->ModelIndex);





  if (Element->Emitter) { Result &= WriteToFile(File, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
  else                        { Result &= WriteToFile(File, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }



  Result &= Serialize(File, (u32*)&Element->State);




  Result &= Serialize(File, (u32*)&Element->Behavior);




  Result &= Serialize(File, &Element->Carrying);





  Result &= Serialize(File, &Element->UserType);





  if (EntityUserDataSerialize)   {Result &= EntityUserDataSerialize(File, Element->UserType, Element->UserData);}

  if (Element->Emitter) { Result &= Serialize(File, Element->Emitter); }




  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

