// callsite
// external/bonsai_stdlib/src/texture_cursor.cpp:1:0

// def (are_equal)
// external/bonsai_stdlib/src/poof_functions.h:638:0
link_internal b32
AreEqual(texture *Thing1, texture *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = True;
            Result &= AreEqual(Thing1->ID, Thing2->ID);

        Result &= AreEqual(Thing1->Dim, Thing2->Dim);

        Result &= AreEqual(Thing1->Slices, Thing2->Slices);

        Result &= AreEqual(Thing1->Channels, Thing2->Channels);

        Result &= AreEqual(Thing1->IsDepthTexture, Thing2->IsDepthTexture);

        Result &= AreEqual(Thing1->Format, Thing2->Format);

        Result &= AreEqual(Thing1->DebugName, Thing2->DebugName);

        Result &= AreEqual(Thing1->Queued, Thing2->Queued);



    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(texture Thing1, texture Thing2)
{
    b32 Result = True;
      Result &= AreEqual(Thing1.ID, Thing2.ID);

    Result &= AreEqual(Thing1.Dim, Thing2.Dim);

    Result &= AreEqual(Thing1.Slices, Thing2.Slices);

    Result &= AreEqual(Thing1.Channels, Thing2.Channels);

    Result &= AreEqual(Thing1.IsDepthTexture, Thing2.IsDepthTexture);

    Result &= AreEqual(Thing1.Format, Thing2.Format);

    Result &= AreEqual(Thing1.DebugName, Thing2.DebugName);

    Result &= AreEqual(Thing1.Queued, Thing2.Queued);



  return Result;
}

