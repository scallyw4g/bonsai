// callsite
// src/engine/serdes.cpp:292:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
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
                  
  
  Result &= Deserialize(Bytes, &Element->Entropy, Memory);







            Element->SpawnType = Cast(particle_spawn_type, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Drag, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Lifetime, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->EmissionDelay, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->EmissionLifespan, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ActiveParticles, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->LifespanMod, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ParticleLifespan, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ParticlesPerSecond, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ParticleLightEmission, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ParticleLightEmissionChance, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ParticleStartingTransparency, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ParticleEndingTransparency, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->ParticleStartingDim, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->ParticleEndingDim, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->ParticleTurbMin, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->ParticleTurbMax, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->SpawnRegion, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->SystemMovementCoefficient, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ElapsedSinceLastEmission, Memory);







              {
        umm Count = 6;

    Result &= Deserialize(Bytes, Element->MCVColors, Memory, Count);
  }






    


    
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  


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


