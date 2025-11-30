// callsite
// src/engine/editor.h:1391:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:693:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, brush_layer_kind *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xCCE93B0);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle brush_layer_kind", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Noise"), UiId(Window, "enum BrushLayerKind_Noise", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerKind_Noise;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Shape"), UiId(Window, "enum BrushLayerKind_Shape", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerKind_Shape;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Brush"), UiId(Window, "enum BrushLayerKind_Brush", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerKind_Brush;


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


