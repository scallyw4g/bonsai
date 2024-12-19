// src/engine/serdes.cpp:443:0

link_internal bonsai_type_info
TypeInfo(render_settings *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("render_settings");
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
Serialize(u8_cursor_block_array *Bytes, render_settings *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  Upsert(TypeInfo(BaseElement), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber =2;
  Serialize(Bytes, &VersionNumber);


  RangeIterator_t(umm, ElementIndex, Count)
  {
    render_settings *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->UseSsao); // default






    Result &= Serialize(Bytes, &Element->UseShadowMapping); // default






    Result &= Serialize(Bytes, &Element->UseLightingBloom); // default






    Result &= Serialize(Bytes, &Element->BravoilMyersOIT); // default






    Result &= Serialize(Bytes, &Element->BravoilMcGuireOIT); // default






    Result &= Serialize(Bytes, &Element->DrawMajorGrid); // default






    Result &= Serialize(Bytes, &Element->DrawMinorGrid); // default






    Result &= Serialize(Bytes, &Element->MajorGridDim); // default






    Result &= Serialize(Bytes, &Element->DrawCameraGhost); // default






    Result &= Serialize(Bytes, &Element->CameraGhostSize); // default






    Result &= Serialize(Bytes, &Element->OffsetOfWorldCenterToGrid); // default






    Result &= Serialize(Bytes, &Element->Headless); // default






    Result &= Serialize(Bytes, (u32*)&Element->ToneMappingType); // enum




    Result &= Serialize(Bytes, &Element->GameCameraFOV); // default






    Result &= Serialize(Bytes, &Element->Lighting); // default






    Result &= Serialize(Bytes, &Element->ApplicationResolution); // default






    Result &= Serialize(Bytes, &Element->ShadowMapResolution); // default






    Result &= Serialize(Bytes, &Element->LuminanceMapResolution); // default






    Result &= Serialize(Bytes, &Element->iApplicationResolution); // default






    Result &= Serialize(Bytes, &Element->iShadowMapResolution); // default






    Result &= Serialize(Bytes, &Element->iLuminanceMapResolution); // default

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, render_settings *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <=2);

  b32 Result = True;

  if (TypeInfo->Version == 0)
  {
    render_settings_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }
  if (TypeInfo->Version == 1)
  {
    render_settings_1 T1 = {};
    Result &= Deserialize(Bytes, &T1, Memory);
    Marshal(&T1, Element);
  }


  if (TypeInfo->Version ==2)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element, Memory);
  }

  return Result;
}


link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->UseSsao, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->UseShadowMapping, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->UseLightingBloom, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->BravoilMyersOIT, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->BravoilMcGuireOIT, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->DrawMajorGrid, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->DrawMinorGrid, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->MajorGridDim, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->DrawCameraGhost, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->CameraGhostSize, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->OffsetOfWorldCenterToGrid, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Headless, Memory);





  Element->ToneMappingType = Cast(tone_mapping_type, Read_u32(Bytes));




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->GameCameraFOV, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Lighting, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ApplicationResolution, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ShadowMapResolution, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->LuminanceMapResolution, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->iApplicationResolution, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->iShadowMapResolution, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->iLuminanceMapResolution, Memory);

  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
    maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("render_settings"));

    if (MaybeSerializedType.Tag)
    {
      u64 OldIgnoredVersionNumber;
      if (MaybeSerializedType.Value.Version > 0)
      {
        Deserialize(Bytes, &OldIgnoredVersionNumber, Memory);
      }
      Result &= DeserializeVersioned(Bytes, Element+ElementIndex, &MaybeSerializedType.Value, Memory);
    }
    else
    {
      bonsai_type_info T0TypeInfo = {};
      Result &= DeserializeVersioned(Bytes, Element+ElementIndex, &T0TypeInfo, Memory);
    }

  }

  return Result;
}


