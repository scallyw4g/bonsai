// callsite
// src/engine/editor.cpp:533:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:484:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shader_language_setting *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1C155A0B);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle shader_language_setting", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("330core"), UiId(Window, "enum ShaderLanguageSetting_330core", Element, ThisHash), Params))
    {
            *Element = ShaderLanguageSetting_330core;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("310es"), UiId(Window, "enum ShaderLanguageSetting_310es", Element, ThisHash), Params))
    {
            *Element = ShaderLanguageSetting_310es;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
}


