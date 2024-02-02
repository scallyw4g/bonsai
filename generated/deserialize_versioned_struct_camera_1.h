// src/engine/serdes.cpp:95:0

link_internal b32
DeserializeVersioned(u8_cursor *Bytes, camera *Element, bonsai_type_info *TypeInfo, u64 Version, memory_arena *Memory)
{
  Assert(Version <= 0);

  b32 Result = True;

  

  if (Version == 0)
  {
    Result &= Deserialize(Bytes, Element, Memory);
  }

  return Result;
}

