link_internal b32
DeserializeVersioned(u8_stream *Bytes, camera *Element, bonsai_type_info *TypeInfo, u64 Version, memory_arena *Memory)
{
  Assert(Version <= 0);

  b32 Result = True;

  if (Version == 0)
  {
    camera_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }

  return Result;
}

