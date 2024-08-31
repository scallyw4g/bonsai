// src/engine/serdes.cpp:265:0

link_internal bonsai_type_info
TypeInfo(particle_system *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("particle_system");
  Result.Version = 0 ;

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

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    particle_system *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->Entropy);





    Result &= Serialize(Bytes, (u32*)&Element->SpawnType);




    Result &= Serialize(Bytes, &Element->Drag);





    Result &= Serialize(Bytes, &Element->Lifetime);





    Result &= Serialize(Bytes, &Element->EmissionDelay);





    Result &= Serialize(Bytes, &Element->EmissionLifespan);





    Result &= Serialize(Bytes, &Element->ActiveParticles);





    Result &= Serialize(Bytes, &Element->LifespanMod);





    Result &= Serialize(Bytes, &Element->ParticleLifespan);





    Result &= Serialize(Bytes, &Element->ParticlesPerSecond);





    Result &= Serialize(Bytes, &Element->ParticleLightEmission);





    Result &= Serialize(Bytes, &Element->ParticleLightEmissionChance);





    Result &= Serialize(Bytes, &Element->ParticleStartingTransparency);





    Result &= Serialize(Bytes, &Element->ParticleEndingTransparency);





    Result &= Serialize(Bytes, &Element->ParticleStartingDim);





    Result &= Serialize(Bytes, &Element->ParticleEndingDim);





    Result &= Serialize(Bytes, &Element->ParticleTurbMin);





    Result &= Serialize(Bytes, &Element->ParticleTurbMax);





    Result &= Serialize(Bytes, &Element->SpawnRegion);





    Result &= Serialize(Bytes, &Element->SystemMovementCoefficient);





    Result &= Serialize(Bytes, &Element->ElapsedSinceLastEmission);





    {
      umm ThisCount = 6;

      Result &= Serialize(Bytes, Element->Colors, ThisCount);
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

    Result &= Deserialize(Bytes, Element->Colors, Memory, Count);
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
    Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


