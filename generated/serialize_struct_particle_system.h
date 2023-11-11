link_internal b32
Serialize(native_file *File, particle_system* Element)
{
  b32 Result = False;
  NotImplemented;
  Serialize(File, &Element->Entropy);


  Serialize(File, (u32*)&Element->SpawnType);


  Serialize(File, &Element->Drag);


  Serialize(File, &Element->Lifetime);


  Serialize(File, &Element->EmissionDelay);


  Serialize(File, &Element->EmissionLifespan);


  Serialize(File, &Element->ActiveParticles);


  Serialize(File, &Element->LifespanMod);


  Serialize(File, &Element->ParticleLifespan);


  Serialize(File, &Element->ParticlesPerSecond);


  Serialize(File, &Element->ParticleLightEmission);


  Serialize(File, &Element->ParticleLightEmissionChance);


  Serialize(File, &Element->ParticleStartingTransparency);


  Serialize(File, &Element->ParticleEndingTransparency);


  Serialize(File, &Element->ParticleStartingDim);


  Serialize(File, &Element->ParticleEndingDim);


  Serialize(File, &Element->ParticleTurbMin);


  Serialize(File, &Element->ParticleTurbMax);


  Serialize(File, &Element->SpawnRegion);


  Serialize(File, &Element->SystemMovementCoefficient);


  Serialize(File, &Element->Colors);


  Serialize(File, &Element->ElapsedSinceLastEmission);


  return Result;
}

