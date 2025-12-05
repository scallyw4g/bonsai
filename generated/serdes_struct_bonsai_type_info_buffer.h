// callsite
// src/engine/serdes.cpp:385:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(bonsai_type_info_buffer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("bonsai_type_info_buffer");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, bonsai_type_info_buffer *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    bonsai_type_info_buffer *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Count); // default







                    if (Element->Start) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }






            

                if (Element->Start) { Result &= Serialize(Bytes, Element->Start, Element->Count ); }





    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, bonsai_type_info_buffer *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, bonsai_type_info_buffer *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, bonsai_type_info_buffer *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, bonsai_type_info_buffer *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, bonsai_type_info_buffer *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->Count, Memory);







          b64 HadStartPointer = Read_u64(Bytes);
  Assert(HadStartPointer < 2); // Should be 0 or 1






    
      if (HadStartPointer)
  {
        umm Count = Element->Count;


    if (Element->Start == 0)
    {
      Element->Start = Allocate(bonsai_type_info, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->Start, Memory, Count);
  }




  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, bonsai_type_info_buffer *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, bonsai_type_info_buffer *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


