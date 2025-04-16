// src/engine/serdes.cpp:364:0

link_internal bonsai_type_info
TypeInfo(entity *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("entity");
  Result.Version =  2 ;

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
Serialize(u8_cursor_block_array *Bytes, entity *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

    Upsert(TypeInfo(BaseElement), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber = 2;
  Serialize(Bytes, &VersionNumber);


  RangeIterator_t(umm, ElementIndex, Count)
  {
    entity *Element = BaseElement + ElementIndex;
                                Result &= Serialize(Bytes, &Element->Id); // default






                            Result &= Serialize(Bytes, &Element->P); // default






                            Result &= Serialize(Bytes, &Element->EulerAngles); // default






                            Result &= Serialize(Bytes, &Element->Scale); // default






                            Result &= Serialize(Bytes, &Element->_CollisionVolumeRadius); // default






                            Result &= Serialize(Bytes, &Element->Physics); // default






                            Result &= Serialize(Bytes, &Element->AssetId); // default






                            Result &= Serialize(Bytes, &Element->ModelIndex); // default






                if (Element->Emitter) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }



                    Result &= Serialize(Bytes, (u32*)&Element->State); // enum




                    Result &= Serialize(Bytes, (u32*)&Element->Behavior); // enum




                            Result &= Serialize(Bytes, &Element->Carrying); // default






                            Result &= Serialize(Bytes, &Element->UserType); // default






              if (EntityUserDataSerialize)   {Result &= EntityUserDataSerialize(Bytes, Element->UserType, Element->UserData);}

                    if (Element->Emitter) { Result &= Serialize(Bytes, Element->Emitter); }





    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

