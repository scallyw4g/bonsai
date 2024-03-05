// src/engine/editor.cpp:450:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, resolution_setting *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("4096x2160"), UiId(Window, "enum ResolutionSetting_4096x2160", Element), Params))
    {
      *Element = ResolutionSetting_4096x2160;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("1920x1080"), UiId(Window, "enum ResolutionSetting_1920x1080", Element), Params))
    {
      *Element = ResolutionSetting_1920x1080;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("1280x720"), UiId(Window, "enum ResolutionSetting_1280x720", Element), Params))
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

