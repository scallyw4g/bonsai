// src/engine/editor.cpp:445:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, level_editor *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle level_editor", Element), Params))
      {
        DidToggle = True;
        PushNewRow(Ui);
      }
      else
      {
        DrawChildren = False;
      }
    }

    if (DrawChildren)
    {
      PushTableStart(Ui);
      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(memory_arena*, Element->Memory),
        CSz("Memory"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_edit_tool*,&Element->Tool),
        CSz("Tool"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_edit_tool*,&Element->PreviousTool),
        CSz("PreviousTool"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_edit_brush_type*,&Element->BrushType),
        CSz("BrushType"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_edit_params*,&Element->Params),
        CSz("Params"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(b8*,&Element->SelectionFollowsCursor),
        CSz("SelectionFollowsCursor"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(layered_brush_editor*,&Element->LayeredBrushEditor),
        CSz("LayeredBrushEditor"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->RootChunkNeedsNewMesh),
        CSz("RootChunkNeedsNewMesh"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(cp*,&Element->MostRecentSelectionRegionMin),
        CSz("MostRecentSelectionRegionMin"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(cp*,&Element->NextSelectionRegionMin),
        CSz("NextSelectionRegionMin"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(cp*,&Element->EditorPreviewRegionMin),
        CSz("EditorPreviewRegionMin"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u64*,&Element->EngineDebugViewModeToggleBits),
        CSz("EngineDebugViewModeToggleBits"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u16*,&Element->SelectedColorIndex),
        CSz("SelectedColorIndex"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u16*,&Element->HoverColorIndex),
        CSz("HoverColorIndex"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->SelectionChanged),
        CSz("SelectionChanged"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*,&Element->SelectionClicks),
        CSz("SelectionClicks"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(cp*,&Element->SelectionBase),
        CSz("SelectionBase"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(rect3cp*,&Element->SelectionRegion),
        CSz("SelectionRegion"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(rect3cp*,&Element->PrevSelectionRegion),
        CSz("PrevSelectionRegion"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(rect3cp*,&Element->CopyRegion),
        CSz("CopyRegion"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(selection_modification_state*,&Element->Selection),
        CSz("Selection"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(selection_modification_state*,&Element->Entity),
        CSz("Entity"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(asset_thumbnail_block_array*,&Element->AssetThumbnails),
        CSz("AssetThumbnails"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->NewAssetFromSelection),
        CSz("NewAssetFromSelection"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v NewAssetFromSelectionFilename[512]"), CSz("> NewAssetFromSelectionFilename[512]"), UiId(Window, "toggle level_editor char  NewAssetFromSelectionFilename", Element->NewAssetFromSelectionFilename), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 512)
        {
          DoEditorUi(Ui, Window, Element->NewAssetFromSelectionFilename+ArrayIndex, FSz("NewAssetFromSelectionFilename[%d]", ArrayIndex), Params);
 PushNewRow(Ui); 
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      PushNewRow(Ui);
      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
      PushTableEnd(Ui);
    }
    else
    {
      PushNewRow(Ui);
    }

  }
  else
  {
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

