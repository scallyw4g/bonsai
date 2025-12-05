// callsite
// src/engine/serdes.cpp:411:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(rectangular_lattice_params *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("rectangular_lattice_params");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, rectangular_lattice_params *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    rectangular_lattice_params *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Radius); // default







                                Result &= Serialize(Bytes, &Element->Jitter); // default







                                Result &= Serialize(Bytes, &Element->Period); // default









            

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, rectangular_lattice_params *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, rectangular_lattice_params *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, rectangular_lattice_params *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, rectangular_lattice_params *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, rectangular_lattice_params *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->Radius, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Jitter, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->Period, Memory);










    
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, rectangular_lattice_params *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, rectangular_lattice_params *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


