// callsite
// src/engine/serdes.cpp:139:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(vertex_material *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("vertex_material");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, vertex_material *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    vertex_material *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->ColorIndex); // default







                                Result &= Serialize(Bytes, &Element->Transparency); // default







                                Result &= Serialize(Bytes, &Element->Emission); // default









            

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, vertex_material *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, vertex_material *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, vertex_material *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, vertex_material *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, vertex_material *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->ColorIndex, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Transparency, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Emission, Memory);









    
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, vertex_material *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, vertex_material *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


