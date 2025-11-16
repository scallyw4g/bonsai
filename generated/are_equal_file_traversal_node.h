// callsite
// external/bonsai_stdlib/src/file.h:28:0

// def (are_equal)
// external/bonsai_stdlib/src/poof_functions.h:638:0
link_internal b32
AreEqual(file_traversal_node *Thing1, file_traversal_node *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = True;
            Result &= AreEqual(Thing1->Type, Thing2->Type);

        Result &= AreEqual(Thing1->Dir, Thing2->Dir);

        Result &= AreEqual(Thing1->Name, Thing2->Name);



    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
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

