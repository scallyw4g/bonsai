// callsite
// src/engine/serdes.cpp:33:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(frustum *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("frustum");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, frustum *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    frustum *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->farClip); // default







                                Result &= Serialize(Bytes, &Element->nearClip); // default







                                Result &= Serialize(Bytes, &Element->FOV); // default







                                Result &= Serialize(Bytes, &Element->Top); // default







                                Result &= Serialize(Bytes, &Element->Bottom); // default







                                Result &= Serialize(Bytes, &Element->Left); // default







                                Result &= Serialize(Bytes, &Element->Right); // default









            

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, frustum *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, frustum *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, frustum *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, frustum *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, frustum *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->farClip, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->nearClip, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->FOV, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Top, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Bottom, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Left, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Right, Memory);









    
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, frustum *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, frustum *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


