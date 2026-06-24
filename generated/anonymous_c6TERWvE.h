// callsite
// src/engine/settings.cpp:68:0

// def (anonymous)
// src/engine/settings.cpp:68:0



















if (StringsMatch(CSz("CenterCamera"), Name))
{
  Assert(Result == 0);
  Result = &Hotkeys->CenterCamera;
}

if (StringsMatch(CSz("NewSelection"), Name))
{
  Assert(Result == 0);
  Result = &Hotkeys->NewSelection;
}

if (StringsMatch(CSz("ResizeSelection_AllAxies"), Name))
{
  Assert(Result == 0);
  Result = &Hotkeys->ResizeSelection_AllAxies;
}

if (StringsMatch(CSz("ResizeSelection_BothLinearAxies"), Name))
{
  Assert(Result == 0);
  Result = &Hotkeys->ResizeSelection_BothLinearAxies;
}

if (StringsMatch(CSz("ResizeSelection_SingleLinearAxis"), Name))
{
  Assert(Result == 0);
  Result = &Hotkeys->ResizeSelection_SingleLinearAxis;
}

if (StringsMatch(CSz("TranslateSelection_Linear"), Name))
{
  Assert(Result == 0);
  Result = &Hotkeys->TranslateSelection_Linear;
}

if (StringsMatch(CSz("TranslateSelection_Planar"), Name))
{
  Assert(Result == 0);
  Result = &Hotkeys->TranslateSelection_Planar;
}



