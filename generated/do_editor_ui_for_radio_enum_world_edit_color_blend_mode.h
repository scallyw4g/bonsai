// src/engine/editor.h:909:0

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
    if (Button(Ui, CSz("ValuePositive"), UiId(Window, "enum WorldEdit_ColorBlendMode_ValuePositive", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_ValuePositive;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ValueNegative"), UiId(Window, "enum WorldEdit_ColorBlendMode_ValueNegative", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_ValueNegative;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Surface"), UiId(Window, "enum WorldEdit_ColorBlendMode_Surface", Element), Params))
    {
            *Element = WorldEdit_ColorBlendMode_Surface;


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


