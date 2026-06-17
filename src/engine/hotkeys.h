struct hotkey_settings
poof(@do_editor_ui)
{
  b32 Debug_ToggleMenu;
  b32 Debug_ToggleProfiling;

  b32 Debug_TriangulateIncrement;
  b32 Debug_TriangulateDecrement;

  b32 Left;
  b32 Right;
  b32 Forward;
  b32 Backward;
};

inline v3
GetOrthographicInputs(hotkey_settings *Hotkeys)
{
  v3 Right = V3(1,0,0);
  v3 Forward = V3(0,1,0);

  v3 UpdateDir = V3(0,0,0);

  if ( Hotkeys->Forward )
    UpdateDir += Forward;

  if ( Hotkeys->Backward )
    UpdateDir -= Forward;

  if ( Hotkeys->Right )
    UpdateDir += Right;

  if ( Hotkeys->Left )
    UpdateDir -= Right;

  UpdateDir = Normalize(UpdateDir);

  return UpdateDir;
}

link_internal void
MarshallInputToHotkeys(input *Input, hotkey_settings *Hotkeys)
{
#if BONSAI_INTERNAL
  if (Input->F1.Clicked) { Hotkeys->Debug_ToggleMenu         = True; }
  if (Input->F2.Clicked) { Hotkeys->Debug_ToggleProfiling    = True; }
#endif

  Hotkeys->Left = Input->A.Pressed;
  Hotkeys->Right = Input->D.Pressed;
  Hotkeys->Forward = Input->W.Pressed;
  Hotkeys->Backward = Input->S.Pressed;
}

