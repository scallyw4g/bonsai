// callsite
// src/engine/editor.h:1123:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:751:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, brush_noise_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, primitive_value_changed_record_block_array *ChangeRecords = 0)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1B9D1792);


  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle brush_noise_type", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Perlin"), UiId(Window, "enum NoiseType_Perlin", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(ChangeRecords, Cast(u32*, Element));

            *Element = NoiseType_Perlin;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Voronoi"), UiId(Window, "enum NoiseType_Voronoi", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(ChangeRecords, Cast(u32*, Element));

            *Element = NoiseType_Voronoi;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("White"), UiId(Window, "enum NoiseType_White", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(ChangeRecords, Cast(u32*, Element));

            *Element = NoiseType_White;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("RectLattice"), UiId(Window, "enum NoiseType_RectLattice", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(ChangeRecords, Cast(u32*, Element));

            *Element = NoiseType_RectLattice;


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


