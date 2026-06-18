// callsite
// src/engine/settings.cpp:150:0

// def (anonymous)
// src/engine/settings.cpp:150:0




if (Hotkeys->Left.State == &NullInputEvent)
{
  SoftError("Hotkey (Left) remains unbound!");
  Result = False;
}

if (Hotkeys->Right.State == &NullInputEvent)
{
  SoftError("Hotkey (Right) remains unbound!");
  Result = False;
}

if (Hotkeys->Forward.State == &NullInputEvent)
{
  SoftError("Hotkey (Forward) remains unbound!");
  Result = False;
}

if (Hotkeys->Backward.State == &NullInputEvent)
{
  SoftError("Hotkey (Backward) remains unbound!");
  Result = False;
}




