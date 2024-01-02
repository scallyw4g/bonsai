link_internal b32
Serialize(native_file *File, particle_system *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Entropy);




  Result &= Serialize(File, (u32*)&Element->SpawnType);



  Result &= Serialize(File, &Element->Drag);




  Result &= Serialize(File, &Element->Lifetime);




  Result &= Serialize(File, &Element->EmissionDelay);




  Result &= Serialize(File, &Element->EmissionLifespan);




  Result &= Serialize(File, &Element->ActiveParticles);




  Result &= Serialize(File, &Element->LifespanMod);




  Result &= Serialize(File, &Element->ParticleLifespan);




  Result &= Serialize(File, &Element->ParticlesPerSecond);




  Result &= Serialize(File, &Element->ParticleLightEmission);




  Result &= Serialize(File, &Element->ParticleLightEmissionChance);




  Result &= Serialize(File, &Element->ParticleStartingTransparency);




  Result &= Serialize(File, &Element->ParticleEndingTransparency);




  Result &= Serialize(File, &Element->ParticleStartingDim);




  Result &= Serialize(File, &Element->ParticleEndingDim);




  Result &= Serialize(File, &Element->ParticleTurbMin);




  Result &= Serialize(File, &Element->ParticleTurbMax);




  Result &= Serialize(File, &Element->SpawnRegion);




  Result &= Serialize(File, &Element->SystemMovementCoefficient);




  Result &= Serialize(File, &Element->ElapsedSinceLastEmission);




  Result &= SerializeArray(File, Element->Colors, 6);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, particle_system *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Entropy, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  Element->SpawnType = Cast(particle_spawn_type, Read_u32(Bytes));



  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Drag, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Lifetime, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->EmissionDelay, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->EmissionLifespan, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ActiveParticles, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->LifespanMod, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleLifespan, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticlesPerSecond, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleLightEmission, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleLightEmissionChance, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleStartingTransparency, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleEndingTransparency, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleStartingDim, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleEndingDim, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleTurbMin, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ParticleTurbMax, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->SpawnRegion, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->SystemMovementCoefficient, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ElapsedSinceLastEmission, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */




  RangeIterator(ElementIndex, 6)
  {
    Result &= Deserialize(Bytes, &Element->Colors[ElementIndex]);
  }

  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


