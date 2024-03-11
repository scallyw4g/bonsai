// src/engine/serdes.cpp:486:0

link_internal bonsai_type_info
TypeInfo(voronoi_noise_params *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("voronoi_noise_params");
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
Serialize(u8_cursor_block_array *Bytes, voronoi_noise_params *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    voronoi_noise_params *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, &Element->Threshold);





    Result &= Serialize(Bytes, &Element->Period);





    Result &= Serialize(Bytes, &Element->Amplitude);





    Result &= Serialize(Bytes, &Element->Squareness);





    Result &= Serialize(Bytes, &Element->MaskChance);

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

