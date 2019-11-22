
struct input_event
{
  b32 Clicked;
  b32 Pressed;
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

