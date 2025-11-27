// callsite
// src/engine/editor.h:981:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:660:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, ui_noise_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x56825CE);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle ui_noise_type", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Perlin"), UiId(Window, "enum NoiseType_Perlin", Element, ThisHash), Params))
    {
            *Element = NoiseType_Perlin;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Voronoi"), UiId(Window, "enum NoiseType_Voronoi", Element, ThisHash), Params))
    {
            *Element = NoiseType_Voronoi;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("White"), UiId(Window, "enum NoiseType_White", Element, ThisHash), Params))
    {
            *Element = NoiseType_White;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
}


