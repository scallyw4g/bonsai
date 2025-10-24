// external/bonsai_stdlib/src/poof_functions.h:624:0
link_internal b32
AreEqual(octree_node *Thing1, octree_node *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( octree_node ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(octree_node Thing1, octree_node Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( octree_node ) );

  return Result;
}

