// callsite
// external/bonsai_stdlib/src/primitives.h:320:0

// def (are_equal)
// external/bonsai_stdlib/src/poof_functions.h:638:0
link_internal b32
AreEqual(primitive_value_changed_record *Thing1, primitive_value_changed_record *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( primitive_value_changed_record ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(primitive_value_changed_record Thing1, primitive_value_changed_record Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( primitive_value_changed_record ) );

  return Result;
}

