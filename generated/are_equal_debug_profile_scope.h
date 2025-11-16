// callsite
// external/bonsai_debug/src/api.h:104:0

// def (are_equal)
// external/bonsai_stdlib/src/poof_functions.h:638:0
link_internal b32
AreEqual(debug_profile_scope *Thing1, debug_profile_scope *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( debug_profile_scope ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(debug_profile_scope Thing1, debug_profile_scope Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( debug_profile_scope ) );

  return Result;
}

