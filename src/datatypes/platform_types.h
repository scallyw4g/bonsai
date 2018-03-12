
struct input_event
{
  b32 WasPressed;
  b32 IsDown;
};

struct hotkeys
{
  b32 Debug_RedrawEveryPush;
  b32 Debug_ToggleLoopedGamePlayback;
  b32 Debug_Pause;
  b32 Debug_ToggleProfile;
  b32 Debug_NextUiState;

  b32 Left;
  b32 Right;
  b32 Forward;
  b32 Backward;

  b32 Player_Fire;
  b32 Player_Proton;
  b32 Player_Spawn;
};

