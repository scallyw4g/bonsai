// callsite
// src/engine/editor.cpp:419:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3585:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle_spawn_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, base_ptr_relative_edit_block_array *UiChangeEvents = 0, ui_id *OutClickedFieldId = 0)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x14DBD6B7);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id FieldId = UiId(Window, "toggle particle_spawn_type", Element, ThisHash);
  ui_id ToggleButtonId = FieldId;
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, &EditedParams))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum ParticleSpawnType_None", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = ParticleSpawnType_None;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Random"), UiId(Window, "enum ParticleSpawnType_Random", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = ParticleSpawnType_Random;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Expanding"), UiId(Window, "enum ParticleSpawnType_Expanding", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = ParticleSpawnType_Expanding;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Contracting"), UiId(Window, "enum ParticleSpawnType_Contracting", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = ParticleSpawnType_Contracting;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    if (OutClickedFieldId)
    {
      if (Button(Ui, ElementName, FieldId, &EditedParams))
      {
        Assert(!IsValid(OutClickedFieldId));
        *OutClickedFieldId = FieldId;
      }
    }
    PushNewRow(Ui);
  }
  return Result;
}


