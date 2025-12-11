// callsite
// src/engine/editor.cpp:478:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:486:0
struct gpu_element_buffer_handles;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, gpu_element_buffer_handles *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xA286F82);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle gpu_element_buffer_handles", Element, ThisHash), Params))
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
            
            
            
            cs MemberName = CSz("VAO");

                                                                                                auto Member = Cast(u32*, &Element->VAO);
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
            
            
            
            cs MemberName = CSz("Handles");

                                                

            if (ToggleButton(Ui,
                CSz("v Handles[3]"),
                CSz("> Handles[3]"),
                UiId(Window, "toggle gpu_element_buffer_handles u32 Handles", Element->Handles, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 3;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Handles+ArrayIndex,
                  FSz("Handles[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                 PushNewRow(Ui); 
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



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
            
            
            
            cs MemberName = CSz("ElementCount");

                                                                                                auto Member = Cast(u32*, &Element->ElementCount);
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
            
            
            
            cs MemberName = CSz("ElementType");

                                                                                                auto Member = Cast(data_type*, &Element->ElementType);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


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
            
            
            
            cs MemberName = CSz("Mapped");

                                                                                                auto Member = Cast(b8*, &Element->Mapped);
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
            
            
            
            cs MemberName = CSz("Pad");

                                                                                                auto Member = Cast(b8*, &Element->Pad);
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
            
            
            
            cs MemberName = CSz("Flags");

                                                                                                auto Member = Cast(u16*, &Element->Flags);
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

