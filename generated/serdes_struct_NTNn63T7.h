// callsite
// src/engine/serdes.cpp:464:0

// def (serdes_struct)
// src/engine/serdes.h:593:0
link_internal bonsai_type_info
TypeInfo(brush_layer_0 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("brush_layer_0");
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
Serialize(u8_cursor_block_array *Bytes, brush_layer_0 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    brush_layer_0 *Element = BaseElement + ElementIndex;
                                Result &= Serialize(Bytes, &Element->Settings); // default






        


            

        


    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, brush_layer_0 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, brush_layer_0 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, brush_layer_0 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, brush_layer_0 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, brush_layer_0 *Element, memory_arena *Memory)
{
  b32 Result = True;
                
  
  Result &= Deserialize(Bytes, &Element->Settings, Memory);






    


    
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, brush_layer_0 *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, brush_layer_0 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


