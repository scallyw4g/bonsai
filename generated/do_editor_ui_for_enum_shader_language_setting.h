// src/engine/editor.cpp:341:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shader_language_setting *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("330core"), UiId(Window, "enum ShaderLanguageSetting_330core", Element), Params))
    {
      *Element = ShaderLanguageSetting_330core;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("310es"), UiId(Window, "enum ShaderLanguageSetting_310es", Element), Params))
    {
      *Element = ShaderLanguageSetting_310es;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

