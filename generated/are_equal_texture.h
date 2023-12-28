link_internal b32
AreEqual(texture *Thing1, texture *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( texture ) );


  return Result;
}

