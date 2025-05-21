// src/engine/editor.cpp:374:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_layer_edit_actions Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_layer_edit_actions( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_layer_edit_actions *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("SetBrush"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_edit_actions UiLayerEditAction_SetBrush")), UiLayerEditAction_SetBrush },
    { CSz("Duplicate"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_edit_actions UiLayerEditAction_Duplicate")), UiLayerEditAction_Duplicate },
    { CSz("Delete"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_edit_actions UiLayerEditAction_Delete")), UiLayerEditAction_Delete },
  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}




link_internal ui_toggle_button_group
PushToolbar(     renderer_2d *Ui, 
  window_layout *Window,
  cs  GroupName,
  ui_layer_edit_actions *Element,
  u64  Index = 0,
  ui_render_params *Params     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
            {
      CSz("S"),
      CSz("SetBrush"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_layer_edit_actions UiLayerEditAction_SetBrush"),
        Cast(void*, Index)
      ),
      UiLayerEditAction_SetBrush,
    },

        {
      CSz("D"),
      CSz("Duplicate"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_layer_edit_actions UiLayerEditAction_Duplicate"),
        Cast(void*, Index)
      ),
      UiLayerEditAction_Duplicate,
    },

        {
      CSz("X"),
      CSz("Delete"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_layer_edit_actions UiLayerEditAction_Delete"),
        Cast(void*, Index)
      ),
      UiLayerEditAction_Delete,
    },
  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = {};
  Result.Ui = Ui;
  Result.Flags = ToggleButtonGroupFlags_TypeClickButton;
  Result.Buttons = ButtonBuffer;
  Result.EnumStorage = Cast(u32*, Element);

  DrawButtonGroup(&Result, GroupName);
  return Result;
}

