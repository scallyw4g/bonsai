// src/engine/serdes.h:495:0
link_internal bonsai_type_info
TypeInfo(entity_game_data *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("entity_game_data");
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
Serialize(u8_cursor_block_array *Bytes, entity_game_data *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    entity_game_data *Element = BaseElement + ElementIndex;
                                Result &= Serialize(Bytes, &Element->FireballChargeLevel); // default






                            Result &= Serialize(Bytes, &Element->FireballCharges); // default






                            Result &= Serialize(Bytes, &Element->IceBlockCharges); // default






                            Result &= Serialize(Bytes, &Element->HoldingItem); // default








            

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, entity_game_data *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, entity_game_data *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, entity_game_data *Element, memory_arena *Memory)
{
  b32 Result = True;
              // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->FireballChargeLevel, Memory);





            // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->FireballCharges, Memory);





            // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->IceBlockCharges, Memory);





            // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->HoldingItem, Memory);







    
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, entity_game_data *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


