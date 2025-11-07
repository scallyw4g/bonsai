// src/engine/serdes.h:563:0
link_internal bonsai_type_info
TypeInfo(layered_brush *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("layered_brush");
  Result.Version =  0 ;

  /* type.map(member) */
  /* { */
  /*   { */
  /*     member_info Member = {CSz("member.name"), CSz("member.name"), 0x(member.hash)}; */
  /*     Push(&Result.Members, &Member); */
  /*   } */
  /* } */

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, layered_brush *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    layered_brush *Element = BaseElement + ElementIndex;
                                Result &= Serialize(Bytes, &Element->LayerCount); // default






                        {
            // TODO(Jesse): Should this really be a safe cast?
      umm ThisCount = umm(Element->LayerCount);

      Result &= Serialize(Bytes, Element->Layers, ThisCount);
    }





                            Result &= Serialize(Bytes, &Element->AffectExisting); // default








            

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, layered_brush *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, layered_brush *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layered_brush *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layered_brush *Element, memory_arena *Memory)
{
  b32 Result = True;
                
  
  Result &= Deserialize(Bytes, &Element->LayerCount, Memory);






            {
        // TODO(Jesse): Should this really be a safe cast?
    umm Count = umm(Element->LayerCount);

    Result &= Deserialize(Bytes, Element->Layers, Memory, Count);
  }





              
  
  Result &= Deserialize(Bytes, &Element->AffectExisting, Memory);








    
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, layered_brush *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


