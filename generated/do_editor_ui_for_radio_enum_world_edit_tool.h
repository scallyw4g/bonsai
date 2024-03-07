// src/engine/editor.h:650:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, world_edit_tool Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal void
GetRadioEnum(ui_toggle_button_group *RadioGroup, world_edit_tool *Result)
{
  if (RadioGroup->ToggleBits)
  {
    Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1); // Radio group can 
  }

  s32 Index = s32(GetIndexOfNthSetBit(u32(RadioGroup->ToggleBits), 1));
  *Result = world_edit_tool(Max(0, Index));
}

link_internal b32
ToggledOn(ui_toggle_button_group *ButtonGroup, world_edit_tool Enum)
{
  b32 Result = ButtonGroup->ToggleBits & (1 << Enum);
  return Result;
}

// NOTE(Jesse): This could be implemented by reconstructing the button ID
// but I'm very unsure that's worth it.  Seems like just
link_internal b32
Clicked(ui_toggle_button_group *ButtonGroup, world_edit_tool Enum)
{
  b32 Result = False;
  NotImplemented;
  return Result;
}

link_internal ui_toggle_button_group
RadioButtonGroup_world_edit_tool( renderer_2d *Ui,
  window_layout *Window,
  cs GroupName,
  world_edit_tool *Element,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  cs ButtonNames[] =
  {
    CSz("Disabled"),
    CSz("Select"),
    CSz("Brush"),
    CSz("Eyedropper"),
    CSz("BlitEntity"),
    CSz("StandingSpots"),
  };

  u32 ButtonCount = ArrayCount(ButtonNames);

  ui_toggle_button_handle_buffer ButtonBuffer = UiToggleButtonHandleBuffer(ButtonCount, GetTranArena());
  IterateOver(&ButtonBuffer, Button, ButtonIndex)
  {
    cs ButtonName = ButtonNames[ButtonIndex];
    *Button = UiToggle(ButtonName, UiId(Window, Cast(void*, Element), Cast(void*, ButtonName.Start)));
  }

  ui_toggle_button_group Result = UiToggleButtonGroup(Ui, &ButtonBuffer, GroupName, Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons));

  return Result;
}



link_internal ui_toggle_button_group
DoEditorUi( renderer_2d *Ui,
  window_layout *Window,
  world_edit_tool *Element,
  cs GroupName,
  ui_render_params *Params,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); PushNewRow(Ui); } */
  ui_toggle_button_group RadioGroup = RadioButtonGroup_world_edit_tool(Ui, Window, GroupName, Element, Params, ExtraFlags);
  GetRadioEnum(&RadioGroup, Element);
  return RadioGroup;
}

