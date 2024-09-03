// src/engine/editor.cpp:147:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, debug_state *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle debug_state", Element), Params))
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
        Cast(b8*,&Element->Initialized),
        CSz("Initialized"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*,&Element->UIType),
        CSz("UIType"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u64*,&Element->BytesBufferedToCard),
        CSz("BytesBufferedToCard"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DebugDoScopeProfiling),
        CSz("DebugDoScopeProfiling"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u64*,&Element->NumScopes),
        CSz("NumScopes"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*,&Element->DrawCallCountLastFrame),
        CSz("DrawCallCountLastFrame"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*,&Element->VertexCountLastFrame),
        CSz("VertexCountLastFrame"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(picked_world_chunk*, Element->PickedChunk),
        CSz("PickedChunk"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(picked_world_chunk*, Element->HoverChunk),
        CSz("HoverChunk"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(debug_thread_state*, Element->ThreadStates),
        CSz("ThreadStates"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(memory_arena*, Element->MetaTableNameStringsArena),
        CSz("MetaTableNameStringsArena"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(renderer_2d*, Element->UiGroup),
        CSz("UiGroup"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(untextured_3d_geometry_buffer*,&Element->LineMesh),
        CSz("LineMesh"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(selected_arenas*, Element->SelectedArenas),
        CSz("SelectedArenas"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DisplayDebugMenu),
        CSz("DisplayDebugMenu"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(debug_profile_scope*, Element->HotFunction),
        CSz("HotFunction"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u64_cursor*,&Element->HistogramSamples),
        CSz("HistogramSamples"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(debug_profile_scope*,&Element->FreeScopeSentinel),
        CSz("FreeScopeSentinel"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(umm*,&Element->MinCycles),
        CSz("MinCycles"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(umm*,&Element->MaxCycles),
        CSz("MaxCycles"),
        Params
        );







      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v Frames[(128)]"), CSz("> Frames[(128)]"), UiId(Window, "toggle debug_state frame_stats Frames", Element->Frames), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (128))
        {
          DoEditorUi(Ui, Window, Element->Frames+ArrayIndex, FSz("Frames[%d]", ArrayIndex), Params);
          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*,&Element->ReadScopeIndex),
        CSz("ReadScopeIndex"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s32*,&Element->FreeScopeCount),
        CSz("FreeScopeCount"),
        Params
        );







      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v RegisteredMemoryArenas[(1024)]"), CSz("> RegisteredMemoryArenas[(1024)]"), UiId(Window, "toggle debug_state registered_memory_arena RegisteredMemoryArenas", Element->RegisteredMemoryArenas), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (1024))
        {
          DoEditorUi(Ui, Window, Element->RegisteredMemoryArenas+ArrayIndex, FSz("RegisteredMemoryArenas[%d]", ArrayIndex), Params);
          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
      if (ToggleButton(Ui, CSz("v TrackedDrawCalls[(4096)]"), CSz("> TrackedDrawCalls[(4096)]"), UiId(Window, "toggle debug_state debug_draw_call TrackedDrawCalls", Element->TrackedDrawCalls), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (4096))
        {
          DoEditorUi(Ui, Window, Element->TrackedDrawCalls+ArrayIndex, FSz("TrackedDrawCalls[%d]", ArrayIndex), Params);
          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
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

