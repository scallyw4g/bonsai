// src/engine/editor.cpp:569:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_debug_view_mode *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Level"), UiId(Window, "enum EngineDebugViewMode_Level", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("WorldEdit"), UiId(Window, "enum EngineDebugViewMode_WorldEdit", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Entities"), UiId(Window, "enum EngineDebugViewMode_Entities", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Assets"), UiId(Window, "enum EngineDebugViewMode_Assets", Element), Params))
    {
            if (EngineDebugViewMode_Assets == engine_debug_view_mode(0))
      {
        *Element = engine_debug_view_mode(0);
      }
      else
      {
        if ((*Element & EngineDebugViewMode_Assets) == EngineDebugViewMode_Assets)
        {
          *Element = engine_debug_view_mode(*Element&~EngineDebugViewMode_Assets);
        }
        else
        {
          *Element = engine_debug_view_mode(*Element|EngineDebugViewMode_Assets);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("WorldChunks"), UiId(Window, "enum EngineDebugViewMode_WorldChunks", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Textures"), UiId(Window, "enum EngineDebugViewMode_Textures", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("RenderSettings"), UiId(Window, "enum EngineDebugViewMode_RenderSettings", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("EngineDebug"), UiId(Window, "enum EngineDebugViewMode_EngineDebug", Element), Params))
    {
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


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}


