// callsite
// src/engine/editor.cpp:472:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:486:0
struct chunk_data;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, chunk_data *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x27DCE418);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle chunk_data", Element, ThisHash), Params))
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
            
            
            
            cs MemberName = CSz("Dim");

                                                                                                auto Member = Cast(v3i*, &Element->Dim);
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
            
            
            
            cs MemberName = CSz("Occupancy");

                                                                                                auto Member = Cast(u64*, Element->Occupancy);
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
            
            
            
            cs MemberName = CSz("xOccupancyBorder");

                                                                                                auto Member = Cast(u64*, Element->xOccupancyBorder);
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
            
            
            
            cs MemberName = CSz("FaceMasks");

                                                                                                auto Member = Cast(u64*, Element->FaceMasks);
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

