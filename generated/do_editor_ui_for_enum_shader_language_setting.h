// src/engine/editor.cpp:365:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shader_language_setting *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ShaderLanguageSetting_330core"), UiId(Window, "enum ShaderLanguageSetting_330core", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ShaderLanguageSetting_330core;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ShaderLanguageSetting_310es"), UiId(Window, "enum ShaderLanguageSetting_310es", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

