// callsite
// src/engine/serdes.cpp:377:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(lighting_settings *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("lighting_settings");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, lighting_settings *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    lighting_settings *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->AutoDayNightCycle); // default







                                Result &= Serialize(Bytes, &Element->tDaySpeed); // default







                                Result &= Serialize(Bytes, &Element->tDay); // default







                                Result &= Serialize(Bytes, &Element->SunP); // default







                                Result &= Serialize(Bytes, &Element->FogPower); // default







                                Result &= Serialize(Bytes, &Element->FogColor); // default







                                Result &= Serialize(Bytes, &Element->DawnIntensity); // default







                                Result &= Serialize(Bytes, &Element->DawnHSV); // default







                                Result &= Serialize(Bytes, &Element->SunIntensity); // default







                                Result &= Serialize(Bytes, &Element->SunHSV); // default







                                Result &= Serialize(Bytes, &Element->DuskIntensity); // default







                                Result &= Serialize(Bytes, &Element->DuskColor); // default







                                Result &= Serialize(Bytes, &Element->DuskHSV); // default







                                Result &= Serialize(Bytes, &Element->MoonIntensity); // default







                                Result &= Serialize(Bytes, &Element->MoonHSV); // default







                                Result &= Serialize(Bytes, &Element->CurrentSunColor); // default









            

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, lighting_settings *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, lighting_settings *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, lighting_settings *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, lighting_settings *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, lighting_settings *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->AutoDayNightCycle, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->tDaySpeed, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->tDay, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->SunP, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->FogPower, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->FogColor, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->DawnIntensity, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->DawnHSV, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->SunIntensity, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->SunHSV, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->DuskIntensity, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->DuskColor, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->DuskHSV, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->MoonIntensity, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->MoonHSV, Memory);








                  
  
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
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, lighting_settings *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


