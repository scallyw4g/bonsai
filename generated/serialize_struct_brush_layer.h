// src/engine/serdes.cpp:498:0

link_internal bonsai_type_info
TypeInfo(brush_layer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("brush_layer");
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
Serialize(u8_cursor_block_array *Bytes, brush_layer *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    brush_layer *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, (u32*)&Element->Type);




    Result &= Serialize(Bytes, &Element->NoiseLayer);





    Result &= Serialize(Bytes, &Element->ShapeLayer);

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

