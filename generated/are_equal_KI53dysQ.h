// external/bonsai_stdlib/src/poof_functions.h:626:0
link_internal b32
AreEqual(simple_brush *Thing1, simple_brush *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = True;
            Result &= AreEqual(Thing1->Name, Thing2->Name);

        Result &= AreEqual(Thing1->Outline, Thing2->Outline);

        Result &= AreEqual(Thing1->Texture, Thing2->Texture);



    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(simple_brush Thing1, simple_brush Thing2)
{
    b32 Result = True;
      Result &= AreEqual(Thing1.Name, Thing2.Name);

    Result &= AreEqual(Thing1.Outline, Thing2.Outline);

    Result &= AreEqual(Thing1.Texture, Thing2.Texture);



  return Result;
}

