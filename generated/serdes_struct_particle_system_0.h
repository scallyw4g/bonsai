// callsite
// src/engine/serdes.cpp:292:0

// def (serdes_struct)
// src/engine/serdes.h:619:0
link_internal bonsai_type_info
TypeInfo(particle_system_0 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("particle_system_0");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, particle_system_0 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    particle_system_0 *Element = BaseElement + ElementIndex;
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

      Result &= Serialize(Bytes, Element->MCVColors, ThisCount);
    }






        


            

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        


    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, particle_system_0 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, particle_system_0 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, particle_system_0 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, particle_system_0 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, particle_system_0 *Element, memory_arena *Memory)
{
  b32 Result = True;
  b32 ThisMember;

    ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Entropy, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing random_series Entropy on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
            Element->SpawnType = Cast(particle_spawn_type, Read_u32(Bytes));





  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing particle_spawn_type SpawnType on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Drag, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 Drag on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Lifetime, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 Lifetime on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->EmissionDelay, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 EmissionDelay on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->EmissionLifespan, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 EmissionLifespan on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ActiveParticles, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u32 ActiveParticles on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->LifespanMod, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 LifespanMod on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ParticleLifespan, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 ParticleLifespan on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ParticlesPerSecond, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 ParticlesPerSecond on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ParticleLightEmission, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 ParticleLightEmission on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ParticleLightEmissionChance, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 ParticleLightEmissionChance on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ParticleStartingTransparency, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 ParticleStartingTransparency on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ParticleEndingTransparency, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 ParticleEndingTransparency on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->ParticleStartingDim, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 ParticleStartingDim on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ParticleEndingDim, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing f32 ParticleEndingDim on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->ParticleTurbMin, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 ParticleTurbMin on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->ParticleTurbMax, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 ParticleTurbMax on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->SpawnRegion, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing aabb SpawnRegion on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->SystemMovementCoefficient, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 SystemMovementCoefficient on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ElapsedSinceLastEmission, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 ElapsedSinceLastEmission on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
              {
        umm Count = 6;

    ThisMember = Deserialize(Bytes, Element->MCVColors, Memory, Count);
  }






  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u8 MCVColors on particle_system_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing particle Particles on particle_system_0");
  }
  Result &= ThisMember;


    
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, particle_system_0 *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, particle_system_0 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


