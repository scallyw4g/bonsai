// src/engine/serdes.cpp:496:0

link_internal bonsai_type_info
TypeInfo(shape_layer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("shape_layer");
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
Serialize(u8_cursor_block_array *Bytes, shape_layer *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    shape_layer *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->TODO);

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

