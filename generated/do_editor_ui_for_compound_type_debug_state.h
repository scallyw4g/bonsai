// callsite
// src/engine/editor.cpp:262:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:305:0
struct debug_state;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, debug_state *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2186A9AB);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle debug_state", Element, ThisHash), Params))
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
        
        
        
        cs MemberName = CSz("Initialized");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->Initialized),
          MemberName,
          ThisHash,
          &DefaultUiRenderParams_Checkbox
          );





                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("UIType");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->UIType),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("BytesBufferedToCard");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64*, &Element->BytesBufferedToCard),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("DebugDoScopeProfiling");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->DebugDoScopeProfiling),
          MemberName,
          ThisHash,
          &DefaultUiRenderParams_Checkbox
          );





                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("NumScopes");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64*, &Element->NumScopes),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("DrawCallCountLastFrame");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->DrawCallCountLastFrame),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("VertexCountLastFrame");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->VertexCountLastFrame),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("FrameEnd");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("FrameBegin");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("DebugRegisterArena");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("UnregisterArena");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("WorkerThreadAdvanceDebugSystem");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("MainThreadAdvanceDebugSystem");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("MutexWait");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("MutexAquired");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("MutexReleased");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("GetProfileScope");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("Debug_Allocate");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("RegisterThread");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("WriteMemoryRecord");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("ClearMemoryRecordsFor");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("TrackDrawCall");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("GetThreadLocalState");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("DebugValue_r32");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("DebugValue_u32");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("DebugValue_u64");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("DumpScopeTreeDataToConsole");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("OpenAndInitializeDebugWindow");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("ProcessInputAndRedrawWindow");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("(anonymous)");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("(anonymous)");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("(anonymous)");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("GetReadScopeTree");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("GetWriteScopeTree");
                                                                






        


      }
      

      { 
        
        
        
        cs MemberName = CSz("PickedChunk");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(picked_world_chunk*, Element->PickedChunk),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("HoverChunk");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(picked_world_chunk*, Element->HoverChunk),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("ThreadStates");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(debug_thread_state*, Element->ThreadStates),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("MetaTableNameStringsArena");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(memory_arena*, Element->MetaTableNameStringsArena),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("UiGroup");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(renderer_2d*, Element->UiGroup),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("LineMesh");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(untextured_3d_geometry_buffer*, &Element->LineMesh),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("SelectedArenas");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(selected_arenas*, Element->SelectedArenas),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("DisplayDebugMenu");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->DisplayDebugMenu),
          MemberName,
          ThisHash,
          &DefaultUiRenderParams_Checkbox
          );





                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("HotFunction");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(debug_profile_scope*, Element->HotFunction),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("HistogramSamples");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64_cursor*, &Element->HistogramSamples),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("HistogramFutex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(bonsai_futex*, &Element->HistogramFutex),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("FreeScopeSentinel");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(debug_profile_scope*, &Element->FreeScopeSentinel),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("MinCycles");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(umm*, &Element->MinCycles),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("MaxCycles");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(umm*, &Element->MaxCycles),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("Frames");
                                

        if (ToggleButton(Ui,
            CSz("v Frames[(128)]"),
            CSz("> Frames[(128)]"),
            UiId(Window, "toggle debug_state frame_stats Frames", Element->Frames, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
                    s32 End = (128);

          RangeIterator(ArrayIndex, End)
          {
                        DoEditorUi(Ui,
              Window,
              Element->Frames+ArrayIndex,
              FSz("Frames[%d]", ArrayIndex),
              ThisHash,
              Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);



        


      }
      

      { 
        
        
        
        cs MemberName = CSz("ReadScopeIndex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->ReadScopeIndex),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("FreeScopeCount");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(s32*, &Element->FreeScopeCount),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("RegisteredMemoryArenas");
                                

        if (ToggleButton(Ui,
            CSz("v RegisteredMemoryArenas[(1024)]"),
            CSz("> RegisteredMemoryArenas[(1024)]"),
            UiId(Window, "toggle debug_state registered_memory_arena RegisteredMemoryArenas", Element->RegisteredMemoryArenas, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
                    s32 End = (1024);

          RangeIterator(ArrayIndex, End)
          {
                        DoEditorUi(Ui,
              Window,
              Element->RegisteredMemoryArenas+ArrayIndex,
              FSz("RegisteredMemoryArenas[%d]", ArrayIndex),
              ThisHash,
              Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);



        


      }
      

      { 
        
        
        
        cs MemberName = CSz("TrackedDrawCalls");
                                

        if (ToggleButton(Ui,
            CSz("v TrackedDrawCalls[(4096)]"),
            CSz("> TrackedDrawCalls[(4096)]"),
            UiId(Window, "toggle debug_state debug_draw_call TrackedDrawCalls", Element->TrackedDrawCalls, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
                    s32 End = (4096);

          RangeIterator(ArrayIndex, End)
          {
                        DoEditorUi(Ui,
              Window,
              Element->TrackedDrawCalls+ArrayIndex,
              FSz("TrackedDrawCalls[%d]", ArrayIndex),
              ThisHash,
              Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);



        


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

