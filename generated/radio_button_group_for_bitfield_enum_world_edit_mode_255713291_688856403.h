ui_toggle_button_handle WorldEditModeButtons[] =
{
  UiToggle(CSz("Select"), 0),
  UiToggle(CSz("FillSelection"), 0),
  UiToggle(CSz("DeleteSelection"), 0),
  UiToggle(CSz("AddSingle"), 0),
  UiToggle(CSz("RemoveSingle"), 0),
  UiToggle(CSz("PaintSingle"), 0),
};

ui_toggle_button_group WorldEditModeRadioGroup = UiToggleButtonGroup(Ui, WorldEditModeButtons, ArrayCount(WorldEditModeButtons), ToggleButtonGroupFlags_RadioButtons);

/* Assert(CountBitsSet_Kernighan((EnumVarName)Group.ToggleBits) <= 1); */

world_edit_mode WorldEditMode = Cast(world_edit_mode, WorldEditModeRadioGroup.ToggleBits);

