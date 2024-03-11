// src/engine/serdes.cpp:494:0

link_internal bonsai_type_info
TypeInfo(noise_layer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("noise_layer");
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
Serialize(u8_cursor_block_array *Bytes, noise_layer *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    noise_layer *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->Params);







    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

