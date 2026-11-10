// callsite
// src/engine/editor.h:587:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3583:0
link_internal b32
DoEditorUi(      renderer_2d *Ui,
  window_layout *Window,
  world_edit_blend_mode_modifier *Element,
  cs  Name,
  u32  ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
base_ptr_relative_edit_block_array *UiChangeEvents = 0,
  ui_id *OutClickedFieldId = 0 )
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1AAA4276);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle world_edit_blend_mode_modifier", Element, ThisHash);

  ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, &EditedParams);

  if (AnyClicked(Ui, &ToggleButtonId))
  {
    if (OutClickedFieldId)
    {
      Assert(!IsValid(OutClickedFieldId));
      *OutClickedFieldId = ToggleButtonId;
    }
  }

  // Set the toggle off if we rclicked on it
  if (GetToggleState(Ui, &ToggleButtonId) && RClicked(Ui, &ToggleButtonId))
  {
    SetToggleState(Ui, &ToggleButtonId, False);
  }

  if (GetToggleState(Ui, &ToggleButtonId))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum WorldEdit_Modifier_None", Element, ThisHash), Params))
    {
      Result = True;
            if (WorldEdit_Modifier_None == world_edit_blend_mode_modifier(0))
      {
        *Element = world_edit_blend_mode_modifier(0);
      }
      else
      {
        if ((*Element & WorldEdit_Modifier_None) == WorldEdit_Modifier_None)
        {
          *Element = world_edit_blend_mode_modifier(*Element&~WorldEdit_Modifier_None);
        }
        else
        {
          *Element = world_edit_blend_mode_modifier(*Element|WorldEdit_Modifier_None);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ClampPos"), UiId(Window, "enum WorldEdit_ValueModifier_ClampPos", Element, ThisHash), Params))
    {
      Result = True;
            if (WorldEdit_ValueModifier_ClampPos == world_edit_blend_mode_modifier(0))
      {
        *Element = world_edit_blend_mode_modifier(0);
      }
      else
      {
        if ((*Element & WorldEdit_ValueModifier_ClampPos) == WorldEdit_ValueModifier_ClampPos)
        {
          *Element = world_edit_blend_mode_modifier(*Element&~WorldEdit_ValueModifier_ClampPos);
        }
        else
        {
          *Element = world_edit_blend_mode_modifier(*Element|WorldEdit_ValueModifier_ClampPos);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ClampNeg"), UiId(Window, "enum WorldEdit_ValueModifier_ClampNeg", Element, ThisHash), Params))
    {
      Result = True;
            if (WorldEdit_ValueModifier_ClampNeg == world_edit_blend_mode_modifier(0))
      {
        *Element = world_edit_blend_mode_modifier(0);
      }
      else
      {
        if ((*Element & WorldEdit_ValueModifier_ClampNeg) == WorldEdit_ValueModifier_ClampNeg)
        {
          *Element = world_edit_blend_mode_modifier(*Element&~WorldEdit_ValueModifier_ClampNeg);
        }
        else
        {
          *Element = world_edit_blend_mode_modifier(*Element|WorldEdit_ValueModifier_ClampNeg);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Threshold"), UiId(Window, "enum WorldEdit_ValueModifier_Threshold", Element, ThisHash), Params))
    {
      Result = True;
            if (WorldEdit_ValueModifier_Threshold == world_edit_blend_mode_modifier(0))
      {
        *Element = world_edit_blend_mode_modifier(0);
      }
      else
      {
        if ((*Element & WorldEdit_ValueModifier_Threshold) == WorldEdit_ValueModifier_Threshold)
        {
          *Element = world_edit_blend_mode_modifier(*Element&~WorldEdit_ValueModifier_Threshold);
        }
        else
        {
          *Element = world_edit_blend_mode_modifier(*Element|WorldEdit_ValueModifier_Threshold);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Discard"), UiId(Window, "enum WorldEdit_ColorModifier_Discard", Element, ThisHash), Params))
    {
      Result = True;
            if (WorldEdit_ColorModifier_Discard == world_edit_blend_mode_modifier(0))
      {
        *Element = world_edit_blend_mode_modifier(0);
      }
      else
      {
        if ((*Element & WorldEdit_ColorModifier_Discard) == WorldEdit_ColorModifier_Discard)
        {
          *Element = world_edit_blend_mode_modifier(*Element&~WorldEdit_ColorModifier_Discard);
        }
        else
        {
          *Element = world_edit_blend_mode_modifier(*Element|WorldEdit_ColorModifier_Discard);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
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


