link_internal bonsai_type_info
TypeInfo(particle_system *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("particle_system");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("Entropy"), CSz("Entropy"), 0x19908EC4};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("SpawnType"), CSz("SpawnType"), 0x196B1D85};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Drag"), CSz("Drag"), 0x291A717F};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Lifetime"), CSz("Lifetime"), 0x29BA3789};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("EmissionDelay"), CSz("EmissionDelay"), 0x3393ED36};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("EmissionLifespan"), CSz("EmissionLifespan"), 0x19AAA94C};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ActiveParticles"), CSz("ActiveParticles"), 0x21FB4F2E};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("LifespanMod"), CSz("LifespanMod"), 0x854C01E};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticleLifespan"), CSz("ParticleLifespan"), 0x3897ADA3};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticlesPerSecond"), CSz("ParticlesPerSecond"), 0x38A35F56};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticleLightEmission"), CSz("ParticleLightEmission"), 0xED1A616};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticleLightEmissionChance"), CSz("ParticleLightEmissionChance"), 0x29B1BD12};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticleStartingTransparency"), CSz("ParticleStartingTransparency"), 0x11C89978};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticleEndingTransparency"), CSz("ParticleEndingTransparency"), 0x4846C5};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticleStartingDim"), CSz("ParticleStartingDim"), 0xFD0A292};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticleEndingDim"), CSz("ParticleEndingDim"), 0x111BC84C};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticleTurbMin"), CSz("ParticleTurbMin"), 0x34C97463};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ParticleTurbMax"), CSz("ParticleTurbMax"), 0x3372BD06};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("SpawnRegion"), CSz("SpawnRegion"), 0x1BE13FF5};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("SystemMovementCoefficient"), CSz("SystemMovementCoefficient"), 0x8A80525};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ElapsedSinceLastEmission"), CSz("ElapsedSinceLastEmission"), 0x345D5A75};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Colors"), CSz("Colors"), 0x37379D40};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Particles"), CSz("Particles"), 0x27E75E9D};
    Push(&Result.Members, &Member);
  }

  return Result;
}

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
DeserializeUnversioned(u8_stream *Bytes, particle_system *Element, memory_arena *Memory)
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





  RangeIterator(ElementIndex, 6)
  {
    Result &= Deserialize(Bytes, &Element->Colors[ElementIndex]);
  }







  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, particle_system *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


