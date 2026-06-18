// callsite
// src/engine/settings.cpp:124:0

// def (anonymous)
// src/engine/settings.cpp:124:0




if (StringsMatch(CSz("Left"), TSettingName->Value))
{
  Assert(BindingSuccess == False);
  Hotkeys->Left.State = ResolveHotkeyNameToInputPointer(TSettingValue->Value, Input);
  BindingSuccess = True;
}

if (StringsMatch(CSz("Right"), TSettingName->Value))
{
  Assert(BindingSuccess == False);
  Hotkeys->Right.State = ResolveHotkeyNameToInputPointer(TSettingValue->Value, Input);
  BindingSuccess = True;
}

if (StringsMatch(CSz("Forward"), TSettingName->Value))
{
  Assert(BindingSuccess == False);
  Hotkeys->Forward.State = ResolveHotkeyNameToInputPointer(TSettingValue->Value, Input);
  BindingSuccess = True;
}

if (StringsMatch(CSz("Backward"), TSettingName->Value))
{
  Assert(BindingSuccess == False);
  Hotkeys->Backward.State = ResolveHotkeyNameToInputPointer(TSettingValue->Value, Input);
  BindingSuccess = True;
}




