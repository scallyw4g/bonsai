link_internal b32
AreEqual(file_traversal_node *Thing1, file_traversal_node *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( file_traversal_node ) );
  return Result;
}

