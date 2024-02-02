// src/engine/serdes.cpp:394:0

link_internal b32
DeserializeVersioned(u8_cursor *Bytes, level_header *Element, bonsai_type_info *TypeInfo, u64 Version, memory_arena *Memory)
{
  Assert(Version <= 2);

  b32 Result = True;

  if (Version == 0)
  {
    level_header_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }
  if (Version == 1)
  {
    level_header_1 T1 = {};
    Result &= Deserialize(Bytes, &T1, Memory);
    Marshal(&T1, Element);
  }


  if (Version == 2)
  {
    Result &= Deserialize(Bytes, Element, Memory);
  }

  return Result;
}

