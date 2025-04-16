// src/engine/bonsai_type_info.h:24:0

link_internal b32
AreEqual(bonsai_type_info *Thing1, bonsai_type_info *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = True;
            Result &= AreEqual(Thing1->Name, Thing2->Name);

        Result &= AreEqual(Thing1->Version, Thing2->Version);

        Result &= AreEqual(Thing1->SizeOfInBytes, Thing2->SizeOfInBytes);

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(bonsai_type_info Thing1, bonsai_type_info Thing2)
{
    b32 Result = True;
      Result &= AreEqual(Thing1.Name, Thing2.Name);

    Result &= AreEqual(Thing1.Version, Thing2.Version);

    Result &= AreEqual(Thing1.SizeOfInBytes, Thing2.SizeOfInBytes);

  return Result;
}

