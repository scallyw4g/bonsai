
struct input_event
{
  b32 Clicked;
  b32 Pressed;
};

struct input
{
  input_event Escape;

  input_event Space;
  input_event Shift;
  input_event Ctrl;
  input_event Alt;

  input_event F12;
  input_event F11;
  input_event F10;
  input_event F9;
  input_event F8;
  input_event F7;
  input_event F6;
  input_event F5;
  input_event F4;
  input_event F3;
  input_event F2;
  input_event F1;

  input_event RMB;
  input_event LMB;
  input_event MMB;
  input_event W;
  input_event A;
  input_event S;
  input_event D;

  input_event Q;
  input_event E;
};

struct hotkeys
{
#if BONSAI_INTERNAL
  b32 Debug_RedrawEveryPush;
  b32 Debug_ToggleLoopedGamePlayback;
  b32 Debug_ToggleTriggeredRuntimeBreak;
  b32 Debug_Pause;
  b32 Debug_ToggleProfile;
  b32 Debug_ToggleMenu;
  b32 Debug_TriangulateIncrement;
  b32 Debug_TriangulateDecrement;
  b32 Debug_MousePick;
#endif

  b32 Left;
  b32 Right;
  b32 Forward;
  b32 Backward;

  b32 Player_Fire;
  b32 Player_Proton;
  b32 Player_Spawn;

};

inline v3
GetOrthographicInputs(hotkeys *Hotkeys)
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
