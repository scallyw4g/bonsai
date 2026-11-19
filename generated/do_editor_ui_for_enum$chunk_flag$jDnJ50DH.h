// callsite
// src/engine/editor.cpp:589:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3591:0
link_internal b32
DoEditorUi(      renderer_2d *Ui,
  window_layout *Window,
  chunk_flag *Element,
  cs  Name,
  u32  ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
base_ptr_relative_edit_block_array *UiChangeEvents = 0,
  ui_id *OutClickedFieldId = 0 )
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x516E95E);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle chunk_flag", Element, ThisHash);

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
    if (Button(Ui, CSz("Uninitialized"), UiId(Window, "enum Chunk_Uninitialized", Element, ThisHash), Params))
    {
      Result = True;
            if (Chunk_Uninitialized == chunk_flag(0))
      {
        *Element = chunk_flag(0);
      }
      else
      {
        if ((*Element & Chunk_Uninitialized) == Chunk_Uninitialized)
        {
          *Element = chunk_flag(*Element&~Chunk_Uninitialized);
        }
        else
        {
          *Element = chunk_flag(*Element|Chunk_Uninitialized);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Queued"), UiId(Window, "enum Chunk_Queued", Element, ThisHash), Params))
    {
      Result = True;
            if (Chunk_Queued == chunk_flag(0))
      {
        *Element = chunk_flag(0);
      }
      else
      {
        if ((*Element & Chunk_Queued) == Chunk_Queued)
        {
          *Element = chunk_flag(*Element&~Chunk_Queued);
        }
        else
        {
          *Element = chunk_flag(*Element|Chunk_Queued);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("VoxelsInitialized"), UiId(Window, "enum Chunk_VoxelsInitialized", Element, ThisHash), Params))
    {
      Result = True;
            if (Chunk_VoxelsInitialized == chunk_flag(0))
      {
        *Element = chunk_flag(0);
      }
      else
      {
        if ((*Element & Chunk_VoxelsInitialized) == Chunk_VoxelsInitialized)
        {
          *Element = chunk_flag(*Element&~Chunk_VoxelsInitialized);
        }
        else
        {
          *Element = chunk_flag(*Element|Chunk_VoxelsInitialized);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Garbage"), UiId(Window, "enum Chunk_Garbage", Element, ThisHash), Params))
    {
      Result = True;
            if (Chunk_Garbage == chunk_flag(0))
      {
        *Element = chunk_flag(0);
      }
      else
      {
        if ((*Element & Chunk_Garbage) == Chunk_Garbage)
        {
          *Element = chunk_flag(*Element&~Chunk_Garbage);
        }
        else
        {
          *Element = chunk_flag(*Element|Chunk_Garbage);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Deallocate"), UiId(Window, "enum Chunk_Deallocate", Element, ThisHash), Params))
    {
      Result = True;
            if (Chunk_Deallocate == chunk_flag(0))
      {
        *Element = chunk_flag(0);
      }
      else
      {
        if ((*Element & Chunk_Deallocate) == Chunk_Deallocate)
        {
          *Element = chunk_flag(*Element&~Chunk_Deallocate);
        }
        else
        {
          *Element = chunk_flag(*Element|Chunk_Deallocate);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Freelist"), UiId(Window, "enum Chunk_Freelist", Element, ThisHash), Params))
    {
      Result = True;
            if (Chunk_Freelist == chunk_flag(0))
      {
        *Element = chunk_flag(0);
      }
      else
      {
        if ((*Element & Chunk_Freelist) == Chunk_Freelist)
        {
          *Element = chunk_flag(*Element&~Chunk_Freelist);
        }
        else
        {
          *Element = chunk_flag(*Element|Chunk_Freelist);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("SpawnTriggersRun"), UiId(Window, "enum Chunk_SpawnTriggersRun", Element, ThisHash), Params))
    {
      Result = True;
            if (Chunk_SpawnTriggersRun == chunk_flag(0))
      {
        *Element = chunk_flag(0);
      }
      else
      {
        if ((*Element & Chunk_SpawnTriggersRun) == Chunk_SpawnTriggersRun)
        {
          *Element = chunk_flag(*Element&~Chunk_SpawnTriggersRun);
        }
        else
        {
          *Element = chunk_flag(*Element|Chunk_SpawnTriggersRun);
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


