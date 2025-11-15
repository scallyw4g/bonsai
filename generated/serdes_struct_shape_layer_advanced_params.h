// callsite
// src/engine/serdes.cpp:442:0

// def (serdes_struct)
// src/engine/serdes.h:572:0
link_internal bonsai_type_info
TypeInfo(shape_layer_advanced_params *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("shape_layer_advanced_params");
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
Serialize(u8_cursor_block_array *Bytes, shape_layer_advanced_params *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    shape_layer_advanced_params *Element = BaseElement + ElementIndex;
                                Result &= Serialize(Bytes, &Element->Rounding); // default






                            Result &= Serialize(Bytes, &Element->Stretch); // default






                            Result &= Serialize(Bytes, &Element->Repeat); // default






                            Result &= Serialize(Bytes, &Element->Axis); // default








            

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, shape_layer_advanced_params *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, shape_layer_advanced_params *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, shape_layer_advanced_params *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, shape_layer_advanced_params *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, shape_layer_advanced_params *Element, memory_arena *Memory)
{
  b32 Result = True;
                
  
  Result &= Deserialize(Bytes, &Element->Rounding, Memory);






                
  
  Result &= Deserialize(Bytes, &Element->Stretch, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Repeat, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Axis, Memory);









    
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, shape_layer_advanced_params *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, shape_layer_advanced_params *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


