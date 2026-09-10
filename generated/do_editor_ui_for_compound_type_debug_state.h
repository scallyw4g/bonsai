// callsite
// src/engine/editor.cpp:357:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:495:0
struct debug_state;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, debug_state *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button, primitive_value_changed_record_block_array *ChangeRecords = 0)


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
        {
          
          { 
            
            
            
            cs MemberName = CSz("Initialized");

                                                                        auto Member = Cast(b32*, &Element->Initialized);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox,
              ChangeRecords
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("UIType");

                                                                                                // Regular struct member
                        auto Member = Cast(u32*, &Element->UIType);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("BytesBufferedToCard");

                                                                                                // Regular struct member
                        auto Member = Cast(u64*, &Element->BytesBufferedToCard);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugDoScopeProfiling");

                                                                        auto Member = Cast(b32*, &Element->DebugDoScopeProfiling);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox,
              ChangeRecords
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("NumScopes");

                                                                                                // Regular struct member
                        auto Member = Cast(u64*, &Element->NumScopes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawCallCountLastFrame");

                                                                                                // Regular struct member
                        auto Member = Cast(u32*, &Element->DrawCallCountLastFrame);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("VertexCountLastFrame");

                                                                                                // Regular struct member
                        auto Member = Cast(u32*, &Element->VertexCountLastFrame);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("FrameEnd");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("FrameBegin");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugRegisterArena");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("UnregisterArena");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("WorkerThreadAdvanceDebugSystem");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("MainThreadAdvanceDebugSystem");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("MutexWait");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("MutexAquired");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("MutexReleased");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("GetProfileScope");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Debug_Allocate");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("RegisterThread");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("WriteMemoryRecord");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("ClearMemoryRecordsFor");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("TrackDrawCall");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("GetThreadLocalState");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugValue_r32");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugValue_u32");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugValue_u64");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("DumpScopeTreeDataToConsole");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("OpenAndInitializeDebugWindow");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("ProcessInputAndRedrawWindow");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("(anonymous)");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("(anonymous)");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("(anonymous)");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("GetReadScopeTree");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("GetWriteScopeTree");

                                                                                                






            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("PickedChunk");

                                                                                                // Regular struct member
                        auto Member = Cast(picked_world_chunk*, Element->PickedChunk);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("HoverChunk");

                                                                                                // Regular struct member
                        auto Member = Cast(picked_world_chunk*, Element->HoverChunk);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("ThreadStates");

                                                                                                // Regular struct member
                        auto Member = Cast(debug_thread_state*, Element->ThreadStates);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("MetaTableNameStringsArena");

                                                                                                // Regular struct member
                        auto Member = Cast(memory_arena*, Element->MetaTableNameStringsArena);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("UiGroup");

                                                                                                // Regular struct member
                        auto Member = Cast(renderer_2d*, Element->UiGroup);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("LineMesh");

                                                                                                // Regular struct member
                        auto Member = Cast(untextured_3d_geometry_buffer*, &Element->LineMesh);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("SelectedArenas");

                                                                                                // Regular struct member
                        auto Member = Cast(selected_arenas*, Element->SelectedArenas);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("DisplayDebugMenu");

                                                                        auto Member = Cast(b32*, &Element->DisplayDebugMenu);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox,
              ChangeRecords
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("HotFunction");

                                                                                                // Regular struct member
                        auto Member = Cast(debug_profile_scope*, Element->HotFunction);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("HistogramSamples");

                                                                                                // Regular struct member
                        auto Member = Cast(u64_cursor*, &Element->HistogramSamples);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("HistogramFutex");

                                                                                                // Regular struct member
                        auto Member = Cast(bonsai_futex*, &Element->HistogramFutex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("FreeScopeSentinel");

                                                                                                // Regular struct member
                        auto Member = Cast(debug_profile_scope*, &Element->FreeScopeSentinel);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("MinCycles");

                                                                                                // Regular struct member
                        auto Member = Cast(umm*, &Element->MinCycles);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("MaxCycles");

                                                                                                // Regular struct member
                        auto Member = Cast(umm*, &Element->MaxCycles);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Frames");

                                                

            // NOTE(Jesse): Copypasta @array_display_code
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
                  Params,
                  ChangeRecords);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("ReadScopeIndex");

                                                                                                // Regular struct member
                        auto Member = Cast(u32*, &Element->ReadScopeIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("FreeScopeCount");

                                                                                                // Regular struct member
                        auto Member = Cast(s32*, &Element->FreeScopeCount);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("RegisteredMemoryArenas");

                                                

            // NOTE(Jesse): Copypasta @array_display_code
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
                  Params,
                  ChangeRecords);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("TrackedDrawCalls");

                                                

            // NOTE(Jesse): Copypasta @array_display_code
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
                  Params,
                  ChangeRecords);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
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

