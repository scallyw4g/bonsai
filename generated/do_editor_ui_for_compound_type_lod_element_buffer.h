// callsite
// src/engine/editor.cpp:484:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:486:0
struct lod_element_buffer;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, lod_element_buffer *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x229F06E8);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lod_element_buffer", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MeshMask");

                                                                                                auto Member = Cast(u32*, &Element->MeshMask);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GpuBufferHandles");

                                                

            if (ToggleButton(Ui,
                CSz("v GpuBufferHandles[MeshIndex_Count]"),
                CSz("> GpuBufferHandles[MeshIndex_Count]"),
                UiId(Window, "toggle lod_element_buffer gpu_element_buffer_handles GpuBufferHandles", Element->GpuBufferHandles, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = MeshIndex_Count;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->GpuBufferHandles+ArrayIndex,
                  FSz("GpuBufferHandles[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("E");

                                                

            if (ToggleButton(Ui,
                CSz("v E[MeshIndex_Count]"),
                CSz("> E[MeshIndex_Count]"),
                UiId(Window, "toggle lod_element_buffer geo_u3d_ptr E", Element->E, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = MeshIndex_Count;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->E+ArrayIndex,
                  FSz("E[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Locks");

                                                

            if (ToggleButton(Ui,
                CSz("v Locks[MeshIndex_Count]"),
                CSz("> Locks[MeshIndex_Count]"),
                UiId(Window, "toggle lod_element_buffer bonsai_futex Locks", Element->Locks, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = MeshIndex_Count;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Locks+ArrayIndex,
                  FSz("Locks[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
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

