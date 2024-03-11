// src/engine/serdes.cpp:492:0

link_internal bonsai_type_info
TypeInfo(noise_params *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("noise_params");
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
Serialize(u8_cursor_block_array *Bytes, noise_params *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    noise_params *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, (u32*)&Element->Type);




    Result &= Serialize(Bytes, &Element->EditParams);





    Result &= Serialize(Bytes, &Element->PerlinParams);





    Result &= Serialize(Bytes, &Element->VoronoiParams);





    Result &= Serialize(Bytes, &Element->Offset);





    Result &= Serialize(Bytes, &Element->Color);

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

