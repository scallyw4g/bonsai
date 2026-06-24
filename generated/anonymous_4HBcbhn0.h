// callsite
// src/engine/settings.cpp:245:0

// def (anonymous)
// src/engine/settings.cpp:245:0
if (Hotkeys->Debug_ToggleMenu == 0)
{
  SoftError("Hotkey (Debug_ToggleMenu) remains unbound!");
  Hotkeys->Debug_ToggleMenu = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Debug_ToggleProfiling == 0)
{
  SoftError("Hotkey (Debug_ToggleProfiling) remains unbound!");
  Hotkeys->Debug_ToggleProfiling = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Debug_ToggleCamera == 0)
{
  SoftError("Hotkey (Debug_ToggleCamera) remains unbound!");
  Hotkeys->Debug_ToggleCamera = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Left == 0)
{
  SoftError("Hotkey (Left) remains unbound!");
  Hotkeys->Left = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Right == 0)
{
  SoftError("Hotkey (Right) remains unbound!");
  Hotkeys->Right = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Forward == 0)
{
  SoftError("Hotkey (Forward) remains unbound!");
  Hotkeys->Forward = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Backward == 0)
{
  SoftError("Hotkey (Backward) remains unbound!");
  Hotkeys->Backward = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Up == 0)
{
  SoftError("Hotkey (Up) remains unbound!");
  Hotkeys->Up = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Down == 0)
{
  SoftError("Hotkey (Down) remains unbound!");
  Hotkeys->Down = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Primary == 0)
{
  SoftError("Hotkey (Primary) remains unbound!");
  Hotkeys->Primary = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Secondary == 0)
{
  SoftError("Hotkey (Secondary) remains unbound!");
  Hotkeys->Secondary = &NullInputEvent;
  Result = False;
}



if (Hotkeys->Tertiary == 0)
{
  SoftError("Hotkey (Tertiary) remains unbound!");
  Hotkeys->Tertiary = &NullInputEvent;
  Result = False;
}






if (Hotkeys->Zoom == 0)
{
  SoftError("Hotkey (Zoom) remains unbound!");
  Hotkeys->Zoom = &NullInputEvent;
  Result = False;
}



if (Hotkeys->DeleteEntity == 0)
{
  SoftError("Hotkey (DeleteEntity) remains unbound!");
  Hotkeys->DeleteEntity = &NullInputEvent;
  Result = False;
}



if (Hotkeys->MultiSelect == 0)
{
  SoftError("Hotkey (MultiSelect) remains unbound!");
  Hotkeys->MultiSelect = &NullInputEvent;
  Result = False;
}





if (Hotkeys->CenterCamera.EventCount == 0)
{
  SoftError("Chord (CenterCamera) remains unbound!");
  Result = False;
}



if (Hotkeys->NewSelection.EventCount == 0)
{
  SoftError("Chord (NewSelection) remains unbound!");
  Result = False;
}



if (Hotkeys->ResizeSelection_AllAxies.EventCount == 0)
{
  SoftError("Chord (ResizeSelection_AllAxies) remains unbound!");
  Result = False;
}



if (Hotkeys->ResizeSelection_BothLinearAxies.EventCount == 0)
{
  SoftError("Chord (ResizeSelection_BothLinearAxies) remains unbound!");
  Result = False;
}



if (Hotkeys->ResizeSelection_SingleLinearAxis.EventCount == 0)
{
  SoftError("Chord (ResizeSelection_SingleLinearAxis) remains unbound!");
  Result = False;
}



if (Hotkeys->TranslateSelection_Linear.EventCount == 0)
{
  SoftError("Chord (TranslateSelection_Linear) remains unbound!");
  Result = False;
}



if (Hotkeys->TranslateSelection_Planar.EventCount == 0)
{
  SoftError("Chord (TranslateSelection_Planar) remains unbound!");
  Result = False;
}



