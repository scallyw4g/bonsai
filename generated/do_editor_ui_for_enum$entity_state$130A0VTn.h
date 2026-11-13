// callsite
// src/engine/editor.cpp:395:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3591:0
link_internal b32
DoEditorUi(      renderer_2d *Ui,
  window_layout *Window,
  entity_state *Element,
  cs  Name,
  u32  ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
base_ptr_relative_edit_block_array *UiChangeEvents = 0,
  ui_id *OutClickedFieldId = 0 )
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x12A84A89);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle entity_state", Element, ThisHash);

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
    if (Button(Ui, CSz("Free"), UiId(Window, "enum EntityState_Free", Element, ThisHash), Params))
    {
      Result = True;
            *Element = EntityState_Free;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Spawned"), UiId(Window, "enum EntityState_Spawned", Element, ThisHash), Params))
    {
      Result = True;
            *Element = EntityState_Spawned;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Destroyed"), UiId(Window, "enum EntityState_Destroyed", Element, ThisHash), Params))
    {
      Result = True;
            *Element = EntityState_Destroyed;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Reserved"), UiId(Window, "enum EntityState_Reserved", Element, ThisHash), Params))
    {
      Result = True;
            *Element = EntityState_Reserved;


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


