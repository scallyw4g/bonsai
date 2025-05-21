// src/engine/editor.h:852:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_color_blend_mode *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ThresholdPositive"), UiId(Window, "enum WorldEdit_ColorBlendMode_ThresholdPositive", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_ThresholdPositive;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ThresholdNegative"), UiId(Window, "enum WorldEdit_ColorBlendMode_ThresholdNegative", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_ThresholdNegative;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Additive"), UiId(Window, "enum WorldEdit_ColorBlendMode_Additive", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_Additive;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Subtractive"), UiId(Window, "enum WorldEdit_ColorBlendMode_Subtractive", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_Subtractive;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Multiply"), UiId(Window, "enum WorldEdit_ColorBlendMode_Multiply", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_Multiply;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Divide"), UiId(Window, "enum WorldEdit_ColorBlendMode_Divide", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_Divide;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Average"), UiId(Window, "enum WorldEdit_ColorBlendMode_Average", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_Average;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Disabled"), UiId(Window, "enum WorldEdit_ColorBlendMode_Disabled", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_Disabled;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}


