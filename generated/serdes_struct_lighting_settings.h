// src/engine/serdes.cpp:406:0

link_internal bonsai_type_info
TypeInfo(lighting_settings *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("lighting_settings");
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
Serialize(u8_cursor_block_array *Bytes, lighting_settings *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  Upsert(TypeInfo(BaseElement), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber =1;
  Serialize(Bytes, &VersionNumber);


  RangeIterator_t(umm, ElementIndex, Count)
  {
    lighting_settings *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->AutoDayNightCycle); // default






    Result &= Serialize(Bytes, &Element->tDay); // default






    Result &= Serialize(Bytes, &Element->SunP); // default






    Result &= Serialize(Bytes, &Element->DawnIntensity); // default






    Result &= Serialize(Bytes, &Element->DawnColor); // default






    Result &= Serialize(Bytes, &Element->SunIntensity); // default






    Result &= Serialize(Bytes, &Element->SunColor); // default






    Result &= Serialize(Bytes, &Element->DuskIntensity); // default






    Result &= Serialize(Bytes, &Element->DuskColor); // default






    Result &= Serialize(Bytes, &Element->MoonIntensity); // default






    Result &= Serialize(Bytes, &Element->MoonColor); // default






    Result &= Serialize(Bytes, &Element->CurrentSunColor); // default

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, lighting_settings *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, lighting_settings *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, lighting_settings *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <=1);

  b32 Result = True;

  if (TypeInfo->Version == 0)
  {
    lighting_settings_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }


  if (TypeInfo->Version ==1)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element, Memory);
  }

  return Result;
}


link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, lighting_settings *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->AutoDayNightCycle, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->tDay, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->SunP, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->DawnIntensity, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->DawnColor, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->SunIntensity, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->SunColor, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->DuskIntensity, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->DuskColor, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->MoonIntensity, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->MoonColor, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->CurrentSunColor, Memory);

  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, lighting_settings *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
    maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("lighting_settings"));

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


