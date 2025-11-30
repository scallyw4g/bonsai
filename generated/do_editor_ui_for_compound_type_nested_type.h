// callsite
// examples/ui_test/game.cpp:71:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:476:0
struct nested_type;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, nested_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x10BA4CBE);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle nested_type", Element, ThisHash), Params))
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
            
            
            
            cs MemberName = CSz("NestedFoo");

                                                                                                auto Member = Cast(s32*, &Element->NestedFoo);
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
            
            
            
            cs MemberName = CSz("NestedBar");

                                                                                                auto Member = Cast(f32*, &Element->NestedBar);
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
            
            
            
            cs MemberName = CSz("Array");

                                                

            if (ToggleButton(Ui,
                CSz("v Array[4]"),
                CSz("> Array[4]"),
                UiId(Window, "toggle nested_type u32 Array", Element->Array, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 4;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Array+ArrayIndex,
                  FSz("Array[%d]", ArrayIndex),
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

