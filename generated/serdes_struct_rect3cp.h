// callsite
// src/engine/serdes.cpp:279:0

// def (serdes_struct)
// src/engine/serdes.h:619:0
link_internal bonsai_type_info
TypeInfo(rect3cp *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("rect3cp");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, rect3cp *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    rect3cp *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Min); // default







                                Result &= Serialize(Bytes, &Element->Max); // default









            

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, rect3cp *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, rect3cp *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, rect3cp *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, rect3cp *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, rect3cp *Element, memory_arena *Memory)
{
  b32 Result = True;
  b32 ThisMember;

    ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Min, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing cp Min on rect3cp");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Max, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing cp Max on rect3cp");
  }
  Result &= ThisMember;


    
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, rect3cp *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, rect3cp *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


