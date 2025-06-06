// external/bonsai_stdlib/src/primitive_containers.h:29:0

link_internal b32
AreEqual(u32_hashtable_wrapper *Thing1, u32_hashtable_wrapper *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( u32_hashtable_wrapper ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(u32_hashtable_wrapper Thing1, u32_hashtable_wrapper Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( u32_hashtable_wrapper ) );

  return Result;
}

