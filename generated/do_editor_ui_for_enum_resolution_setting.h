// src/engine/editor.cpp:432:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, resolution_setting *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ResolutionSetting_4096x2160"), UiId(Window, "enum ResolutionSetting_4096x2160", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ResolutionSetting_4096x2160;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ResolutionSetting_1920x1080"), UiId(Window, "enum ResolutionSetting_1920x1080", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ResolutionSetting_1920x1080;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ResolutionSetting_1280x720"), UiId(Window, "enum ResolutionSetting_1280x720", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ResolutionSetting_1280x720;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

