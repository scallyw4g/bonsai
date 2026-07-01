// callsite
// src/engine/serdes.cpp:427:0

// def (serdes_struct)
// src/engine/serdes.h:619:0
link_internal bonsai_type_info
TypeInfo(world_update_op_shape_params_asset *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_update_op_shape_params_asset");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_update_op_shape_params_asset *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_update_op_shape_params_asset *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->AssetId); // default







                                Result &= Serialize(Bytes, &Element->ModelIndex); // default







                                Result &= Serialize(Bytes, &Element->Origin); // default









            

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_update_op_shape_params_asset *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_asset *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_asset *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_update_op_shape_params_asset *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_update_op_shape_params_asset *Element, memory_arena *Memory)
{
  b32 Result = True;
  b32 ThisMember;

    ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->AssetId, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing asset_id AssetId on world_update_op_shape_params_asset");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ModelIndex, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u64 ModelIndex on world_update_op_shape_params_asset");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Origin, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing cp Origin on world_update_op_shape_params_asset");
  }
  Result &= ThisMember;


    
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_asset *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_asset *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


