// src/engine/editor.h:830:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_blend_mode_modifier *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Default"), UiId(Window, "enum WorldEdit_Modifier_Default", Element), Params))
    {
            if (WorldEdit_Modifier_Default == world_edit_blend_mode_modifier(0))
      {
        *Element = world_edit_blend_mode_modifier(0);
      }
      else
      {
        if ((*Element & WorldEdit_Modifier_Default) == WorldEdit_Modifier_Default)
        {
          *Element = world_edit_blend_mode_modifier(*Element&~WorldEdit_Modifier_Default);
        }
        else
        {
          *Element = world_edit_blend_mode_modifier(*Element|WorldEdit_Modifier_Default);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Surface"), UiId(Window, "enum WorldEdit_ValueModifier_Surface", Element), Params))
    {
            if (WorldEdit_ValueModifier_Surface == world_edit_blend_mode_modifier(0))
      {
        *Element = world_edit_blend_mode_modifier(0);
      }
      else
      {
        if ((*Element & WorldEdit_ValueModifier_Surface) == WorldEdit_ValueModifier_Surface)
        {
          *Element = world_edit_blend_mode_modifier(*Element&~WorldEdit_ValueModifier_Surface);
        }
        else
        {
          *Element = world_edit_blend_mode_modifier(*Element|WorldEdit_ValueModifier_Surface);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ClampPos"), UiId(Window, "enum WorldEdit_ValueModifier_ClampPos", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ClampNeg"), UiId(Window, "enum WorldEdit_ValueModifier_ClampNeg", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Discard"), UiId(Window, "enum WorldEdit_ColorModifier_Discard", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}


