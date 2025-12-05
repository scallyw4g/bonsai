// callsite
// src/engine/serdes.cpp:415:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(voronoi_noise_params *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("voronoi_noise_params");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, voronoi_noise_params *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    voronoi_noise_params *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Period); // default







                                Result &= Serialize(Bytes, &Element->Squareness); // default







                                Result &= Serialize(Bytes, &Element->MaskChance); // default









            

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, voronoi_noise_params *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, voronoi_noise_params *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, voronoi_noise_params *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, voronoi_noise_params *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, voronoi_noise_params *Element, memory_arena *Memory)
{
  b32 Result = True;
                    
  
  Result &= Deserialize(Bytes, &Element->Period, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Squareness, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->MaskChance, Memory);









    
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, voronoi_noise_params *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, voronoi_noise_params *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


