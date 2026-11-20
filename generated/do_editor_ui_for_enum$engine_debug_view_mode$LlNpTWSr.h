// callsite
// src/engine/editor.cpp:703:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3600:0
link_internal b32
DoEditorUi(      renderer_2d *Ui,
  window_layout *Window,
  engine_debug_view_mode *Element,
  cs  Name,
  u32  ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
base_ptr_relative_edit_block_array *UiChangeEvents = 0,
  ui_id *OutClickedFieldId = 0 )
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2D5EDA);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle engine_debug_view_mode", Element, ThisHash);

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
    if (Button(Ui, CSz("Level"), UiId(Window, "enum EngineDebugViewMode_Level", Element, ThisHash), Params))
    {
      Result = True;
            if (EngineDebugViewMode_Level == engine_debug_view_mode(0))
      {
        *Element = engine_debug_view_mode(0);
      }
      else
      {
        if ((*Element & EngineDebugViewMode_Level) == EngineDebugViewMode_Level)
        {
          *Element = engine_debug_view_mode(*Element&~EngineDebugViewMode_Level);
        }
        else
        {
          *Element = engine_debug_view_mode(*Element|EngineDebugViewMode_Level);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("WorldEdit"), UiId(Window, "enum EngineDebugViewMode_WorldEdit", Element, ThisHash), Params))
    {
      Result = True;
            if (EngineDebugViewMode_WorldEdit == engine_debug_view_mode(0))
      {
        *Element = engine_debug_view_mode(0);
      }
      else
      {
        if ((*Element & EngineDebugViewMode_WorldEdit) == EngineDebugViewMode_WorldEdit)
        {
          *Element = engine_debug_view_mode(*Element&~EngineDebugViewMode_WorldEdit);
        }
        else
        {
          *Element = engine_debug_view_mode(*Element|EngineDebugViewMode_WorldEdit);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Entities"), UiId(Window, "enum EngineDebugViewMode_Entities", Element, ThisHash), Params))
    {
      Result = True;
            if (EngineDebugViewMode_Entities == engine_debug_view_mode(0))
      {
        *Element = engine_debug_view_mode(0);
      }
      else
      {
        if ((*Element & EngineDebugViewMode_Entities) == EngineDebugViewMode_Entities)
        {
          *Element = engine_debug_view_mode(*Element&~EngineDebugViewMode_Entities);
        }
        else
        {
          *Element = engine_debug_view_mode(*Element|EngineDebugViewMode_Entities);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("WorldChunks"), UiId(Window, "enum EngineDebugViewMode_WorldChunks", Element, ThisHash), Params))
    {
      Result = True;
            if (EngineDebugViewMode_WorldChunks == engine_debug_view_mode(0))
      {
        *Element = engine_debug_view_mode(0);
      }
      else
      {
        if ((*Element & EngineDebugViewMode_WorldChunks) == EngineDebugViewMode_WorldChunks)
        {
          *Element = engine_debug_view_mode(*Element&~EngineDebugViewMode_WorldChunks);
        }
        else
        {
          *Element = engine_debug_view_mode(*Element|EngineDebugViewMode_WorldChunks);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Textures"), UiId(Window, "enum EngineDebugViewMode_Textures", Element, ThisHash), Params))
    {
      Result = True;
            if (EngineDebugViewMode_Textures == engine_debug_view_mode(0))
      {
        *Element = engine_debug_view_mode(0);
      }
      else
      {
        if ((*Element & EngineDebugViewMode_Textures) == EngineDebugViewMode_Textures)
        {
          *Element = engine_debug_view_mode(*Element&~EngineDebugViewMode_Textures);
        }
        else
        {
          *Element = engine_debug_view_mode(*Element|EngineDebugViewMode_Textures);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("RenderSettings"), UiId(Window, "enum EngineDebugViewMode_RenderSettings", Element, ThisHash), Params))
    {
      Result = True;
            if (EngineDebugViewMode_RenderSettings == engine_debug_view_mode(0))
      {
        *Element = engine_debug_view_mode(0);
      }
      else
      {
        if ((*Element & EngineDebugViewMode_RenderSettings) == EngineDebugViewMode_RenderSettings)
        {
          *Element = engine_debug_view_mode(*Element&~EngineDebugViewMode_RenderSettings);
        }
        else
        {
          *Element = engine_debug_view_mode(*Element|EngineDebugViewMode_RenderSettings);
        }
      }


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("EngineDebug"), UiId(Window, "enum EngineDebugViewMode_EngineDebug", Element, ThisHash), Params))
    {
      Result = True;
            if (EngineDebugViewMode_EngineDebug == engine_debug_view_mode(0))
      {
        *Element = engine_debug_view_mode(0);
      }
      else
      {
        if ((*Element & EngineDebugViewMode_EngineDebug) == EngineDebugViewMode_EngineDebug)
        {
          *Element = engine_debug_view_mode(*Element&~EngineDebugViewMode_EngineDebug);
        }
        else
        {
          *Element = engine_debug_view_mode(*Element|EngineDebugViewMode_EngineDebug);
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


