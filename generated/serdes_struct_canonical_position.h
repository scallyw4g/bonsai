// callsite
// src/engine/serdes.cpp:24:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(canonical_position *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("canonical_position");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, canonical_position *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    canonical_position *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Offset); // default







                                Result &= Serialize(Bytes, &Element->WorldP); // default









            

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, canonical_position *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, canonical_position *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, canonical_position *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, canonical_position *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, canonical_position *Element, memory_arena *Memory)
{
  b32 Result = True;
                    
  
  Result &= Deserialize(Bytes, &Element->Offset, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->WorldP, Memory);










    
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, canonical_position *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, canonical_position *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


