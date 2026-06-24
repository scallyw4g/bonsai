
global_variable input_event NullInputEvent;

enum zoom_type
{
  ZoomType_ClutchDrag,
  ZoomType_Scrollwheel,
};

struct hotkey_chord
{
  u32 EventCount;
  input_event **Events;
};

struct hotkey_settings
poof(@do_editor_ui)
{
  input_event *Debug_ToggleMenu;
  input_event *Debug_ToggleProfiling;
  input_event *Debug_ToggleCamera;

  input_event *Left;
  input_event *Right;
  input_event *Forward;
  input_event *Backward;

  input_event *Up;
  input_event *Down;

  input_event *Primary;
  input_event *Secondary;
  input_event *Tertiary;

  input_event *Ctrl;
  input_event *Alt;
  input_event *Shift;

  zoom_type ZoomType;
  input_event *Zoom;


  input_event *DeleteEntity;
  input_event *MultiSelect;

  hotkey_chord CenterCamera;
  hotkey_chord NewSelection;

  hotkey_chord ResizeSelection_AllAxies;
  hotkey_chord ResizeSelection_BothLinearAxies;
  hotkey_chord ResizeSelection_SingleLinearAxis;
  hotkey_chord TranslateSelection_Linear;
  hotkey_chord TranslateSelection_Planar;
};

inline v3
GetOrthographicInputs(hotkey_settings *Hotkeys)
{
  v3 Right = V3(1,0,0);
  v3 Forward = V3(0,1,0);

  v3 UpdateDir = V3(0,0,0);

  if ( Hotkeys->Forward->Pressed )
    UpdateDir += Forward;

  if ( Hotkeys->Backward->Pressed )
    UpdateDir -= Forward;

  if ( Hotkeys->Right->Pressed )
    UpdateDir += Right;

  if ( Hotkeys->Left->Pressed )
    UpdateDir -= Right;

  UpdateDir = Normalize(UpdateDir);

  return UpdateDir;
}


link_internal b32
ChordPressed(hotkey_chord *Chord)
{
  b32 Result = False;
  NotImplemented;
  return Result;
}
  
link_internal b32
ChordClicked(hotkey_chord *Chord)
{
  b32 Result = False;
  NotImplemented;
  return Result;
}
  
