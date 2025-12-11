// callsite
// src/engine/editor.h:1246:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:705:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_color_blend_mode *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1E3E2BDD);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle world_edit_color_blend_mode", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ValuePositive"), UiId(Window, "enum WorldEdit_ColorBlendMode_ValuePositive", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_ColorBlendMode_ValuePositive;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ValueNegative"), UiId(Window, "enum WorldEdit_ColorBlendMode_ValueNegative", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_ColorBlendMode_ValueNegative;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Surface"), UiId(Window, "enum WorldEdit_ColorBlendMode_Surface", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_ColorBlendMode_Surface;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Always"), UiId(Window, "enum WorldEdit_ColorBlendMode_Always", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_ColorBlendMode_Always;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Disabled"), UiId(Window, "enum WorldEdit_ColorBlendMode_Disabled", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_ColorBlendMode_Disabled;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("FinalBlend"), UiId(Window, "enum WorldEdit_ColorBlendMode_FinalBlend", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_ColorBlendMode_FinalBlend;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
  return Result;
}


