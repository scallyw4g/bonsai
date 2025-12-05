// callsite
// src/engine/serdes.cpp:301:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(physics *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("physics");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, physics *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    physics *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Velocity); // default







                                Result &= Serialize(Bytes, &Element->Force); // default







                                Result &= Serialize(Bytes, &Element->Delta); // default







                                Result &= Serialize(Bytes, &Element->Mass); // default







                                Result &= Serialize(Bytes, &Element->Speed); // default









            

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, physics *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, physics *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, physics *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, physics *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, physics *Element, memory_arena *Memory)
{
  b32 Result = True;
                    
  
  Result &= Deserialize(Bytes, &Element->Velocity, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->Force, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->Delta, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Mass, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Speed, Memory);









    
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, physics *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, physics *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


