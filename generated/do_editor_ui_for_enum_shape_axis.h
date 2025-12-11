// callsite
// src/engine/editor.h:1008:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:705:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, shape_axis *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x29E6250C);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle shape_axis", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("InferFromMajorAxis"), UiId(Window, "enum ShapeAxis_InferFromMajorAxis", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeAxis_InferFromMajorAxis;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("PosX"), UiId(Window, "enum ShapeAxis_PosX", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeAxis_PosX;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("NegX"), UiId(Window, "enum ShapeAxis_NegX", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeAxis_NegX;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("PosY"), UiId(Window, "enum ShapeAxis_PosY", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeAxis_PosY;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("NegY"), UiId(Window, "enum ShapeAxis_NegY", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeAxis_NegY;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("PosZ"), UiId(Window, "enum ShapeAxis_PosZ", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeAxis_PosZ;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("NegZ"), UiId(Window, "enum ShapeAxis_NegZ", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeAxis_NegZ;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Count"), UiId(Window, "enum ShapeAxis_Count", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeAxis_Count;


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


