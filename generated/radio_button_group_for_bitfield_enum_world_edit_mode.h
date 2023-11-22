link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, world_edit_mode Selection)
{
  Assert(CountBitsSet_Kernighan(u32(Selection)) == 1);
  u32 Index = GetIndexOfNthSetBit(u32(Selection), 1);
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Index;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal void
GetRadioEnum(ui_toggle_button_group *RadioGroup, world_edit_mode *Result)
{
  if (RadioGroup->ToggleBits)
  {
    Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1);
    // NOTE(Jesse): This is better; it asserts that we've actually got a bitfield
    Assert(((RadioGroup->ToggleBits == WorldEditMode_Select||RadioGroup->ToggleBits == WorldEditMode_FillSelection||RadioGroup->ToggleBits == WorldEditMode_DeleteSelection||RadioGroup->ToggleBits == WorldEditMode_AddSingle||RadioGroup->ToggleBits == WorldEditMode_RemoveSingle||RadioGroup->ToggleBits == WorldEditMode_PaintSingle)));
    /* Assert((((enum_t.map(value).sep(|) {value.name})) & RadioGroup->ToggleBits) != 0); */
  }

  *Result = Cast(world_edit_mode, RadioGroup->ToggleBits);
}

link_internal b32
ToggledOn(ui_toggle_button_group *ButtonGroup, world_edit_mode Enum)
{
  b32 Result = ButtonGroup->ToggleBits & (1 << Enum);
  return Result;
}

// NOTE(Jesse): This could be implemented by reconstructing the button ID
// but I'm very unsure that's worth it.  Seems like just
link_internal b32
Clicked(ui_toggle_button_group *ButtonGroup, world_edit_mode Enum)
{
  b32 Result = False;
  NotImplemented;
  return Result;
}

link_internal ui_toggle_button_group
RadioButtonGroup_world_edit_mode(renderer_2d *Ui, umm IdModifier, ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None, UI_FUNCTION_PROTO_DEFAULTS)
{
  cs ButtonNames[] =
  {
    CSz("Select"),
    CSz("FillSelection"),
    CSz("DeleteSelection"),
    CSz("AddSingle"),
    CSz("RemoveSingle"),
    CSz("PaintSingle"),
  };

  u32 ButtonCount = ArrayCount(ButtonNames);

  ui_toggle_button_handle_buffer ButtonBuffer = UiToggleButtonHandleBuffer(ButtonCount, GetTranArena());
  IterateOver(&ButtonBuffer, Button, ButtonIndex)
  {
    *Button = UiToggle(ButtonNames[ButtonIndex], IdModifier+ButtonIndex);
  }

  ui_toggle_button_group Result = UiToggleButtonGroup(Ui, &ButtonBuffer, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons), UI_FUNCTION_INSTANCE_NAMES);

  return Result;
}


