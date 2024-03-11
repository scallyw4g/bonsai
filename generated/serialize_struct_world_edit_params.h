// src/engine/serdes.cpp:490:0

link_internal bonsai_type_info
TypeInfo(world_edit_params *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_edit_params");
  Result.Version = 0 ;

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
Serialize(u8_cursor_block_array *Bytes, world_edit_params *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_edit_params *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, (u32*)&Element->Mode);




    Result &= Serialize(Bytes, (u32*)&Element->Modifier);

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

