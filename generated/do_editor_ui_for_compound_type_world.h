// callsite
// src/engine/editor.cpp:567:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:495:0
struct world;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, world *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button, primitive_value_changed_record_block_array *ChangeRecords = 0)


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
        {
          
          { 
            
            
            
            cs MemberName = CSz("Center");

                                                                                                auto Member = Cast(v3i*, &Element->Center);
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
            
            
            
            cs MemberName = CSz("VisibleRegionSize");

                                                                                                // Regular struct member
                        auto Member = Cast(visible_region_size*, &Element->VisibleRegionSize);
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
            
            
            
            cs MemberName = CSz("Root");

                                                                                                // Regular struct member
                        auto Member = Cast(octree_node*, &Element->Root);
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
            
            
            
            cs MemberName = CSz("OctreeMemory");

                                                                                                // Regular struct member
                        auto Member = Cast(memory_arena*, Element->OctreeMemory);
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
            
            
            
            cs MemberName = CSz("OctreeNodeFreelist");

                                                                                                // Regular struct member
                        auto Member = Cast(octree_node_freelist*, &Element->OctreeNodeFreelist);
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
            
            
            
            cs MemberName = CSz("OctreeNodeDeferFreelist");

                                                                                                // Regular struct member
                        auto Member = Cast(octree_node_freelist*, &Element->OctreeNodeDeferFreelist);
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
            
            
            
            cs MemberName = CSz("MaxOctreeNodesToQueuePerFrame");

                                                                                                // Regular struct member
                        auto Member = Cast(s32*, &Element->MaxOctreeNodesToQueuePerFrame);
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
            
            
            
            cs MemberName = CSz("ChunksPerResolutionStep");

                                                                                                // Regular struct member
                        auto Member = Cast(s32*, &Element->ChunksPerResolutionStep);
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
            
            
            
            cs MemberName = CSz("ChunkFreelistFutex");

                        

          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkFreelistSentinal");

                        

          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("FreeChunkCount");

                                                                                                // Regular struct member
                        auto Member = Cast(s32*, &Element->FreeChunkCount);
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
            
            
            
            cs MemberName = CSz("TotalChunksAllocated");

                                                                                                // Regular struct member
                        auto Member = Cast(s32*, &Element->TotalChunksAllocated);
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
            
            
            
            cs MemberName = CSz("TotalGenChunksAllocated");

                                                                                                // Regular struct member
                        auto Member = Cast(s32*, &Element->TotalGenChunksAllocated);
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
            
            
            
            cs MemberName = CSz("ChunkDim");

                        

          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkMemory");

                        

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

