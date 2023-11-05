link_internal void
GetRadioEnum(ui_toggle_button_group *RadioGroup, asset_spawn_mode *Result)
{
  if (RadioGroup->ToggleBits)
  {
    Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1);
    Assert(((AssetSpawnMode_BlitIntoWorld|AssetSpawnMode_Entity) & RadioGroup->ToggleBits) != 0);
  }

  *Result = Cast(asset_spawn_mode, RadioGroup->ToggleBits);
}

link_internal ui_toggle_button_group
RadioButtonGroup_asset_spawn_mode(renderer_2d *Ui, umm IdModifier, ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None, UI_FUNCTION_PROTO_DEFAULTS)
{
  ui_toggle_button_handle Buttons[] =
  {
    UiToggle(CSz("BlitIntoWorld"), IdModifier),
    UiToggle(CSz("Entity"), IdModifier),
  };

  ui_toggle_button_group Result = UiToggleButtonGroup(Ui, Buttons, ArrayCount(Buttons),
    ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons), UI_FUNCTION_INSTANCE_NAMES);

  return Result;
}

