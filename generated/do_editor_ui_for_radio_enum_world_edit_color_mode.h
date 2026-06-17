// callsite
// src/engine/editor.h:1234:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:703:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_color_mode *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x551D41E);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle world_edit_color_mode", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Color"), UiId(Window, "enum WorldEditColorMode_Color", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEditColorMode_Color;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Texture"), UiId(Window, "enum WorldEditColorMode_Texture", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEditColorMode_Texture;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("TintedTexture"), UiId(Window, "enum WorldEditColorMode_TintedTexture", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEditColorMode_TintedTexture;


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


