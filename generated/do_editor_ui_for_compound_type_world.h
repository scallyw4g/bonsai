// src/engine/editor.h:295:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x108363A);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world", Element, ThisHash), Params))
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
        
        
        
        cs MemberName = CSz("Center");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3i*, &Element->Center),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("VisibleRegionSize");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(visible_region_size*, &Element->VisibleRegionSize),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Root");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(octree_node*, &Element->Root),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("OctreeMemory");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(memory_arena*, Element->OctreeMemory),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("OctreeNodeFreelist");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(octree_node_freelist*, &Element->OctreeNodeFreelist),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("OctreeNodeDeferFreelist");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(octree_node_freelist*, &Element->OctreeNodeDeferFreelist),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("MaxOctreeNodesToQueuePerFrame");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(s32*, &Element->MaxOctreeNodesToQueuePerFrame),
          MemberName,
          ThisHash,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("ChunkFreelistFutex");
                

        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ChunkFreelistSentinal");
                

        
      }
      

      { 
        
        
        
        cs MemberName = CSz("FreeChunkCount");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(s32*, &Element->FreeChunkCount),
          MemberName,
          ThisHash,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("ChunkDim");
                

        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ChunkMemory");
                

        
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

