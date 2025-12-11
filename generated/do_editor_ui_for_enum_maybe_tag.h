// callsite
// src/engine/editor.h:1000:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:705:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, maybe_tag *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1FBAE2DF);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle maybe_tag", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("No"), UiId(Window, "enum Maybe_No", Element, ThisHash), Params))
    {
      Result = True;
            *Element = Maybe_No;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Yes"), UiId(Window, "enum Maybe_Yes", Element, ThisHash), Params))
    {
      Result = True;
            *Element = Maybe_Yes;


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


