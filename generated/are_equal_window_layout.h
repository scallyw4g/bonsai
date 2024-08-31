// external/bonsai_stdlib/src/ui/ui.h:123:0

link_internal b32
AreEqual(window_layout *Thing1, window_layout *Thing2)
{
  if (Thing1 && Thing2)
  {
    b32 Result = True;
    Result &= AreEqual(Thing1->HashtableKey, Thing2->HashtableKey);

    Result &= AreEqual(Thing1->Title, Thing2->Title);

    Result &= AreEqual(Thing1->Minimized, Thing2->Minimized);

    Result &= AreEqual(Thing1->MinimizeIndex, Thing2->MinimizeIndex);

    Result &= AreEqual(Thing1->Flags, Thing2->Flags);

    Result &= AreEqual(Thing1->Basis, Thing2->Basis);

    Result &= AreEqual(Thing1->MaxClip, Thing2->MaxClip);

    Result &= AreEqual(Thing1->Scroll, Thing2->Scroll);

    Result &= AreEqual(Thing1->CachedFlags, Thing2->CachedFlags);

    Result &= AreEqual(Thing1->CachedBasis, Thing2->CachedBasis);

    Result &= AreEqual(Thing1->CachedMaxClip, Thing2->CachedMaxClip);

    Result &= AreEqual(Thing1->CachedScroll, Thing2->CachedScroll);

    Result &= AreEqual(Thing1->InteractionStackIndex, Thing2->InteractionStackIndex);

    Result &= AreEqual(Thing1->zBackground, Thing2->zBackground);

    Result &= AreEqual(Thing1->zText, Thing2->zText);

    Result &= AreEqual(Thing1->zBorder, Thing2->zBorder);

    Result &= AreEqual(Thing1->zTitleBar, Thing2->zTitleBar);

    Result &= AreEqual(Thing1->NextHotWindow, Thing2->NextHotWindow);

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(window_layout Thing1, window_layout Thing2)
{
  b32 Result = True;
  Result &= AreEqual(Thing1.HashtableKey, Thing2.HashtableKey);

  Result &= AreEqual(Thing1.Title, Thing2.Title);

  Result &= AreEqual(Thing1.Minimized, Thing2.Minimized);

  Result &= AreEqual(Thing1.MinimizeIndex, Thing2.MinimizeIndex);

  Result &= AreEqual(Thing1.Flags, Thing2.Flags);

  Result &= AreEqual(Thing1.Basis, Thing2.Basis);

  Result &= AreEqual(Thing1.MaxClip, Thing2.MaxClip);

  Result &= AreEqual(Thing1.Scroll, Thing2.Scroll);

  Result &= AreEqual(Thing1.CachedFlags, Thing2.CachedFlags);

  Result &= AreEqual(Thing1.CachedBasis, Thing2.CachedBasis);

  Result &= AreEqual(Thing1.CachedMaxClip, Thing2.CachedMaxClip);

  Result &= AreEqual(Thing1.CachedScroll, Thing2.CachedScroll);

  Result &= AreEqual(Thing1.InteractionStackIndex, Thing2.InteractionStackIndex);

  Result &= AreEqual(Thing1.zBackground, Thing2.zBackground);

  Result &= AreEqual(Thing1.zText, Thing2.zText);

  Result &= AreEqual(Thing1.zBorder, Thing2.zBorder);

  Result &= AreEqual(Thing1.zTitleBar, Thing2.zTitleBar);

  Result &= AreEqual(Thing1.NextHotWindow, Thing2.NextHotWindow);

  return Result;
}

