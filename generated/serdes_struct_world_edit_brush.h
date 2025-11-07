// callsite
// src/engine/serdes.cpp:459:0

// def (serdes_struct)
// src/engine/serdes.h:563:0
link_internal bonsai_type_info
TypeInfo(world_edit_brush *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_edit_brush");
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
Serialize(u8_cursor_block_array *Bytes, world_edit_brush *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_edit_brush *Element = BaseElement + ElementIndex;
                            {
            umm ThisCount = (256) + 1;

      Result &= Serialize(Bytes, Element->NameBuf, ThisCount);
    }





                    Result &= Serialize(Bytes, (u32*)&Element->Mode); // enum




                    Result &= Serialize(Bytes, (u32*)&Element->Modifier); // enum




                            Result &= Serialize(Bytes, &Element->Layered); // default








            

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_edit_brush *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory)
{
  b32 Result = True;
              {
        umm Count = (256) + 1;

    Result &= Deserialize(Bytes, Element->NameBuf, Memory, Count);
  }





          Element->Mode = Cast(world_edit_blend_mode, Read_u32(Bytes));




          Element->Modifier = Cast(world_edit_blend_mode_modifier, Read_u32(Bytes));




              
  
  Result &= Deserialize(Bytes, &Element->Layered, Memory);








    
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


