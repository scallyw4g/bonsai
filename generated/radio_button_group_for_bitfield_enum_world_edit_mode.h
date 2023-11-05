link_internal void
GetRadioEnum(ui_toggle_button_group *RadioGroup, world_edit_mode *Result)
{
  if (RadioGroup->ToggleBits)
  {
    Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1);
    Assert(((WorldEditMode_Select|WorldEditMode_FillSelection|WorldEditMode_DeleteSelection|WorldEditMode_AddSingle|WorldEditMode_RemoveSingle|WorldEditMode_PaintSingle) & RadioGroup->ToggleBits) != 0);
  }

  *Result = Cast(world_edit_mode, RadioGroup->ToggleBits);
}

link_internal ui_toggle_button_group
RadioButtonGroup_world_edit_mode(renderer_2d *Ui, umm IdModifier, ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle Buttons[] =
  {
    UiToggle(CSz("Select"), IdModifier),
    UiToggle(CSz("FillSelection"), IdModifier),
    UiToggle(CSz("DeleteSelection"), IdModifier),
    UiToggle(CSz("AddSingle"), IdModifier),
    UiToggle(CSz("RemoveSingle"), IdModifier),
    UiToggle(CSz("PaintSingle"), IdModifier),
  };

  ui_toggle_button_group Result = UiToggleButtonGroup(Ui, Buttons, ArrayCount(Buttons),
    ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons));

  return Result;
}

