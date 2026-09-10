// callsite
// src/engine/editor.cpp:585:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:495:0
struct collision_event;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, collision_event *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button, primitive_value_changed_record_block_array *ChangeRecords = 0)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x29EED98A);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle collision_event", Element, ThisHash), Params))
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
            
            
            
            cs MemberName = CSz("FrameIndex");

                                                                                                // Regular struct member
                        auto Member = Cast(u32*, &Element->FrameIndex);
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
            
            
            
            cs MemberName = CSz("Count");

                                                                                                // Regular struct member
                        auto Member = Cast(u32*, &Element->Count);
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
            
            
            
            cs MemberName = CSz("MinP");

                                                                                                // Regular struct member
                        auto Member = Cast(cp*, &Element->MinP);
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
            
            
            
            cs MemberName = CSz("MaxP");

                                                                                                // Regular struct member
                        auto Member = Cast(cp*, &Element->MaxP);
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
            
            
            
            cs MemberName = CSz("Normal");

                                                                                                auto Member = Cast(v3*, &Element->Normal);
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
            
            
            
            cs MemberName = CSz("Ratio");

                                                                                                // Regular struct member
                        auto Member = Cast(r32*, &Element->Ratio);
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

