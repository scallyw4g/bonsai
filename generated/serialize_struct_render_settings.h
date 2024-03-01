// src/engine/serdes.cpp:436:0

link_internal bonsai_type_info
TypeInfo(render_settings *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("render_settings");
  Result.Version =1 ;

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
Serialize(u8_cursor_block_array *Bytes, render_settings *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  Upsert(TypeInfo(BaseElement), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber =1;
  Serialize(Bytes, &VersionNumber);


  RangeIterator_t(umm, ElementIndex, Count)
  {
    render_settings *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->UseSsao);





    Result &= Serialize(Bytes, &Element->UseShadowMapping);





    Result &= Serialize(Bytes, &Element->UseLightingBloom);





    Result &= Serialize(Bytes, &Element->BravoilMyersOIT);





    Result &= Serialize(Bytes, &Element->BravoilMcGuireOIT);





    Result &= Serialize(Bytes, &Element->DrawMajorGrid);





    Result &= Serialize(Bytes, &Element->DrawMinorGrid);





    Result &= Serialize(Bytes, &Element->MajorGridDim);





    Result &= Serialize(Bytes, &Element->OffsetOfWorldCenterToGrid);





    Result &= Serialize(Bytes, &Element->Headless);





    Result &= Serialize(Bytes, (u32*)&Element->ToneMappingType);




    Result &= Serialize(Bytes, &Element->Lighting);





    Result &= Serialize(Bytes, &Element->ApplicationResolution);





    Result &= Serialize(Bytes, &Element->ShadowMapResolution);





    Result &= Serialize(Bytes, &Element->LuminanceMapResolution);





    Result &= Serialize(Bytes, &Element->iApplicationResolution);





    Result &= Serialize(Bytes, &Element->iShadowMapResolution);





    Result &= Serialize(Bytes, &Element->iLuminanceMapResolution);

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

