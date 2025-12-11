// callsite
// src/engine/editor.h:1011:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:705:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, voxel_rule_direction *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x77DD4C3);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle voxel_rule_direction", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("PosX"), UiId(Window, "enum VoxelRuleDir_PosX", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VoxelRuleDir_PosX;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("NegX"), UiId(Window, "enum VoxelRuleDir_NegX", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VoxelRuleDir_NegX;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("PosY"), UiId(Window, "enum VoxelRuleDir_PosY", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VoxelRuleDir_PosY;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("NegY"), UiId(Window, "enum VoxelRuleDir_NegY", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VoxelRuleDir_NegY;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("PosZ"), UiId(Window, "enum VoxelRuleDir_PosZ", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VoxelRuleDir_PosZ;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("NegZ"), UiId(Window, "enum VoxelRuleDir_NegZ", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VoxelRuleDir_NegZ;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Count"), UiId(Window, "enum VoxelRuleDir_Count", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VoxelRuleDir_Count;


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


