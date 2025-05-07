// src/engine/editor.cpp:477:0

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
      if (Name.Count) { PushTableStart(Ui); }

      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
            

      { 
        
        
        
        cs MemberName = CSz("Memory");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(memory_arena*, Element->Memory),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Tool");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_tool*, &Element->Tool),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("PreviousTool");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_tool*, &Element->PreviousTool),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("SelectionFollowsCursor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(b8*, &Element->SelectionFollowsCursor),
          MemberName,
          Params
          );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("RootChunkNeedsNewMesh");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->RootChunkNeedsNewMesh),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("EngineDebugViewModeToggleBits");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64*, &Element->EngineDebugViewModeToggleBits),
          MemberName,
          Params
          );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("HoverColorIndex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u16*, &Element->HoverColorIndex),
          MemberName,
          Params
          );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("Selection");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(selection_region*, &Element->Selection),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("CopyRegion");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(rect3cp*, &Element->CopyRegion),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Entity");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(selection_modification_state*, &Element->Entity),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("AssetThumbnails");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(asset_thumbnail_block_array*, &Element->AssetThumbnails),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("NewAssetFromSelection");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->NewAssetFromSelection),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("NewAssetFromSelectionFilename");
                                

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



      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("MaskSelection");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->MaskSelection),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("NextEditOrdinal");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->NextEditOrdinal),
          MemberName,
          Params
          );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("Layers");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_layer_block_array*, &Element->Layers),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("LoadedBrushes");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_brush_hashtable*, &Element->LoadedBrushes),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("CurrentLayer");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_layer*, Element->CurrentLayer),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("HotLayer");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_layer*, Element->HotLayer),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("CurrentEdit");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit*, Element->CurrentEdit),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("HotEdit");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit*, Element->HotEdit),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("CurrentBrush");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_brush*, Element->CurrentBrush),
          MemberName,
          Params
          );







      }
      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
      if (Name.Count) { PushTableEnd(Ui); }
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

