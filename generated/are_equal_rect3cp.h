// src/engine/canonical_position.h:254:0

link_internal b32
AreEqual(rect3cp *Thing1, rect3cp *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( rect3cp ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(rect3cp Thing1, rect3cp Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( rect3cp ) );

  return Result;
}

