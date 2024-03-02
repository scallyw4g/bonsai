// src/engine/editor.h:568:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, world_edit_tool Selection)
{
  NotImplemented;
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
  const char *ToggleGroupIdentifier,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None,
  ui_relative_position_reference *RelativePosition = 0,
  UI_FUNCTION_PROTO_DEFAULTS )
{
  cs ButtonNames[] =
  {
    CSz("Select"),
    CSz("Single"),
    CSz("Eyedropper"),
    CSz("Brush"),
    CSz("BlitEntity"),
    CSz("RecomputeStandingSpots"),
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



link_internal ui_toggle_button_group
DoEditorUi( renderer_2d *Ui,
  window_layout *Window,
  world_edit_tool *Element,
  cs Name,

  // TODO(Jesse): Should these be systemic in the DoEditorUi API?
  ui_relative_position_reference *RelativePosition = 0,

  EDITOR_UI_FUNCTION_PROTO_DEFAULTS )
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); PushNewRow(Ui); }
  ui_toggle_button_group RadioGroup = RadioButtonGroup_world_edit_tool(Ui, Window, "world_edit_tool radio group");
  GetRadioEnum(&RadioGroup, Element);
  return RadioGroup;
}

