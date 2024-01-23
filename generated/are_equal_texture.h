link_internal b32
AreEqual(texture *Thing1, texture *Thing2)
{
  b32 Result = True;
  Result &= AreEqual(Thing1->ID, Thing2->ID);

  Result &= AreEqual(Thing1->Dim, Thing2->Dim);

  Result &= AreEqual(Thing1->Slices, Thing2->Slices);

  Result &= AreEqual(Thing1->Channels, Thing2->Channels);

  Result &= AreEqual(Thing1->IsDepthTexture, Thing2->IsDepthTexture);

  Result &= AreEqual(Thing1->DebugName, Thing2->DebugName);


  return Result;
}

