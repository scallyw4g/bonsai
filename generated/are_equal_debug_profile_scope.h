// external/bonsai_debug/src/api.h:110:0

link_internal b32
AreEqual(debug_profile_scope *Thing1, debug_profile_scope *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( debug_profile_scope ) );

  return Result;
}

link_internal b32
AreEqual(debug_profile_scope Thing1, debug_profile_scope Thing2)
{
  b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( debug_profile_scope ) );

  return Result;
}

