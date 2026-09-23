// callsite
// src/engine/editor.cpp:595:0

// def (do_editor_ui_for_compound_type)
// external/bonsai_stdlib/src/poof_functions.h:3175:0
struct entity_position_info;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_position_info *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button, base_ptr_relative_edit_block_array *UiChangeEvents = 0)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x35BC40CA);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle entity_position_info", Element, ThisHash), Params))
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
            
            
            
            cs MemberName = CSz("P");

                                                                                                // Regular struct member
                        auto Member = Cast(cp*, &Element->P);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              UiChangeEvents
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("_CollisionVolumeRadius");

                                                                                                auto Member = Cast(v3*, &Element->_CollisionVolumeRadius);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              UiChangeEvents
              );







            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Scale");

                                                                                                // Regular struct member
                        auto Member = Cast(r32*, &Element->Scale);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              UiChangeEvents
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("EulerAngles");

                                                                                                auto Member = Cast(v3*, &Element->EulerAngles);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              UiChangeEvents
              );







            


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

