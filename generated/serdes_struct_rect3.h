// callsite
// src/engine/serdes.cpp:273:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(rect3 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("rect3");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, rect3 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    rect3 *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Min); // default







                                Result &= Serialize(Bytes, &Element->Max); // default









            

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, rect3 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, rect3 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, rect3 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, rect3 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, rect3 *Element, memory_arena *Memory)
{
  b32 Result = True;
                    
  
  Result &= Deserialize(Bytes, &Element->Min, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->Max, Memory);










    
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, rect3 *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, rect3 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


