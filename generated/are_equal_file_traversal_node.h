// external/bonsai_stdlib/src/file.h:50:0

link_internal b32
AreEqual(file_traversal_node *Thing1, file_traversal_node *Thing2)
{
  b32 Result = True;
  Result &= AreEqual(Thing1->Type, Thing2->Type);

  Result &= AreEqual(Thing1->Dir, Thing2->Dir);

  Result &= AreEqual(Thing1->Name, Thing2->Name);

  return Result;
}

link_internal b32
AreEqual(file_traversal_node Thing1, file_traversal_node Thing2)
{
  b32 Result = True;
  Result &= AreEqual(Thing1.Type, Thing2.Type);

  Result &= AreEqual(Thing1.Dir, Thing2.Dir);

  Result &= AreEqual(Thing1.Name, Thing2.Name);

  return Result;
}

