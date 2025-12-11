// callsite
// src/engine/editor.h:1241:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:705:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_blend_mode *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1991C8B1);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle world_edit_blend_mode", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Union"), UiId(Window, "enum WorldEdit_Mode_Union", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Union;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Intersection"), UiId(Window, "enum WorldEdit_Mode_Intersection", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Intersection;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Difference"), UiId(Window, "enum WorldEdit_Mode_Difference", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Difference;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("SmoothUnion"), UiId(Window, "enum WorldEdit_Mode_SmoothUnion", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_SmoothUnion;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("SmoothIntersection"), UiId(Window, "enum WorldEdit_Mode_SmoothIntersection", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_SmoothIntersection;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("SmoothDifference"), UiId(Window, "enum WorldEdit_Mode_SmoothDifference", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_SmoothDifference;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Additive"), UiId(Window, "enum WorldEdit_Mode_Additive", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Additive;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Subtractive"), UiId(Window, "enum WorldEdit_Mode_Subtractive", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Subtractive;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Multiply"), UiId(Window, "enum WorldEdit_Mode_Multiply", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Multiply;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Disabled"), UiId(Window, "enum WorldEdit_Mode_Disabled", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Disabled;


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


