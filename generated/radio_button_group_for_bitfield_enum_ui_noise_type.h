// src/engine/editor.h:505:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_noise_type Selection)
{
  Assert(CountBitsSet_Kernighan(u32(Selection)) == 1);
  u32 Index = GetIndexOfNthSetBit(u32(Selection), 1);
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Index;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal void
GetRadioEnum(ui_toggle_button_group *RadioGroup, ui_noise_type *Result)
{
  if (RadioGroup->ToggleBits)
  {
    Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1);
    // NOTE(Jesse): This is better; it asserts that we've actually got a bitfield
    Assert(((RadioGroup->ToggleBits == NoiseType_None||RadioGroup->ToggleBits == NoiseType_Perlin||RadioGroup->ToggleBits == NoiseType_Voronoi)));
    /* Assert((((enum_t.map(value).sep(|) {value.name})) & RadioGroup->ToggleBits) != 0); */
  }

  *Result = Cast(ui_noise_type, RadioGroup->ToggleBits);
}

link_internal b32
ToggledOn(ui_toggle_button_group *ButtonGroup, ui_noise_type Enum)
{
  b32 Result = ButtonGroup->ToggleBits & (1 << Enum);
  return Result;
}

// NOTE(Jesse): This could be implemented by reconstructing the button ID
// but I'm very unsure that's worth it.  Seems like just
link_internal b32
Clicked(ui_toggle_button_group *ButtonGroup, ui_noise_type Enum)
{
  b32 Result = False;
  NotImplemented;
  return Result;
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_noise_type(renderer_2d *Ui, window_layout *Window, const char *ToggleGroupIdentifier, ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None, UI_FUNCTION_PROTO_DEFAULTS)
{
  cs ButtonNames[] =
  {
    CSz("None"),
    CSz("Perlin"),
    CSz("Voronoi"),
  };

  u32 ButtonCount = ArrayCount(ButtonNames);

  ui_toggle_button_handle_buffer ButtonBuffer = UiToggleButtonHandleBuffer(ButtonCount, GetTranArena());
  IterateOver(&ButtonBuffer, Button, ButtonIndex)
  {
    cs Name = ButtonNames[ButtonIndex];
    *Button = UiToggle(Name, Window, ToggleGroupIdentifier, (void*)Name.Start);
  }

  ui_toggle_button_group Result = UiToggleButtonGroup(Ui, &ButtonBuffer, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons), UI_FUNCTION_INSTANCE_NAMES);

  return Result;
}


