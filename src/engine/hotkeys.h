
struct hotkey
{
  input_event *State;
};

global_variable input_event NullInputEvent;

struct hotkey_settings
poof(@do_editor_ui)
{
  b32 Debug_ToggleMenu;
  b32 Debug_ToggleProfiling;

  b32 Debug_TriangulateIncrement;
  b32 Debug_TriangulateDecrement;

  hotkey Left;
  hotkey Right;
  hotkey Forward;
  hotkey Backward;

  s32 ScrollDelta;
};

inline v3
GetOrthographicInputs(hotkey_settings *Hotkeys)
{
  v3 Right = V3(1,0,0);
  v3 Forward = V3(0,1,0);

  v3 UpdateDir = V3(0,0,0);

  if ( Hotkeys->Forward.State->Pressed )
    UpdateDir += Forward;

  if ( Hotkeys->Backward.State->Pressed )
    UpdateDir -= Forward;

  if ( Hotkeys->Right.State->Pressed )
    UpdateDir += Right;

  if ( Hotkeys->Left.State->Pressed )
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
}

