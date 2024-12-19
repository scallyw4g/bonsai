// src/engine/serdes.cpp:290:0

link_internal bonsai_type_info
TypeInfo(particle_system *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("particle_system");
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
Serialize(u8_cursor_block_array *Bytes, particle_system *BaseElement, umm Count = 1)
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
    particle_system *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->Entropy); // default






    Result &= Serialize(Bytes, (u32*)&Element->SpawnType); // enum




    Result &= Serialize(Bytes, &Element->Drag); // default






    Result &= Serialize(Bytes, &Element->Lifetime); // default






    Result &= Serialize(Bytes, &Element->EmissionDelay); // default






    Result &= Serialize(Bytes, &Element->EmissionLifespan); // default






    Result &= Serialize(Bytes, &Element->ActiveParticles); // default






    Result &= Serialize(Bytes, &Element->LifespanMod); // default






    Result &= Serialize(Bytes, &Element->ParticleLifespan); // default






    Result &= Serialize(Bytes, &Element->ParticlesPerSecond); // default






    Result &= Serialize(Bytes, &Element->ParticleLightEmission); // default






    Result &= Serialize(Bytes, &Element->ParticleLightEmissionChance); // default






    Result &= Serialize(Bytes, &Element->ParticleStartingTransparency); // default






    Result &= Serialize(Bytes, &Element->ParticleEndingTransparency); // default






    Result &= Serialize(Bytes, &Element->ParticleStartingDim); // default






    Result &= Serialize(Bytes, &Element->ParticleEndingDim); // default






    Result &= Serialize(Bytes, &Element->ParticleTurbMin); // default






    Result &= Serialize(Bytes, &Element->ParticleTurbMax); // default






    Result &= Serialize(Bytes, &Element->SpawnRegion); // default






    Result &= Serialize(Bytes, &Element->SystemMovementCoefficient); // default






    Result &= Serialize(Bytes, &Element->ElapsedSinceLastEmission); // default






    {
      umm ThisCount = 6;

      Result &= Serialize(Bytes, Element->PackedHSVColors, ThisCount);
    }







    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, particle_system *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, particle_system *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, particle_system *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <=1);

  b32 Result = True;

  if (TypeInfo->Version == 0)
  {
    particle_system_0 T0 = {};
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
DeserializeCurrentVersion(u8_cursor *Bytes, particle_system *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Entropy, Memory);





  Element->SpawnType = Cast(particle_spawn_type, Read_u32(Bytes));




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Drag, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Lifetime, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->EmissionDelay, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->EmissionLifespan, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ActiveParticles, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->LifespanMod, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleLifespan, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticlesPerSecond, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleLightEmission, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleLightEmissionChance, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleStartingTransparency, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleEndingTransparency, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleStartingDim, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleEndingDim, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleTurbMin, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleTurbMax, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->SpawnRegion, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->SystemMovementCoefficient, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ElapsedSinceLastEmission, Memory);





  {
    umm Count = 6;

    Result &= Deserialize(Bytes, Element->PackedHSVColors, Memory, Count);
  }







  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, particle_system *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
    maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("particle_system"));

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


