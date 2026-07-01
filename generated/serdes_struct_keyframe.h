// callsite
// src/engine/serdes.cpp:267:0

// def (serdes_struct)
// src/engine/serdes.h:619:0
link_internal bonsai_type_info
TypeInfo(keyframe *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("keyframe");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, keyframe *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    keyframe *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->tEnd); // default







                                Result &= Serialize(Bytes, &Element->Value); // default









            

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, keyframe *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, keyframe *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, keyframe *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, keyframe *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, keyframe *Element, memory_arena *Memory)
{
  b32 Result = True;
  b32 ThisMember;

    ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->tEnd, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 tEnd on keyframe");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Value, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 Value on keyframe");
  }
  Result &= ThisMember;


    
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, keyframe *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, keyframe *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


