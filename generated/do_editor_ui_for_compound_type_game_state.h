// callsite
// examples/terrain_gen/game.cpp:238:0

// def (do_editor_ui_for_compound_type)
// external/bonsai_stdlib/src/poof_functions.h:3203:0
struct game_state;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, game_state *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button, base_ptr_relative_edit_block_array *UiChangeEvents = 0)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x7825EE2);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle game_state", Element, ThisHash), Params))
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
            
            
            
            cs MemberName = CSz("EasingFunction");

                                                                                                // Regular struct member
                        auto Member = Cast(easing_function*, Element->EasingFunction);
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
            
            
            
            cs MemberName = CSz("VisibleRegionSize");

                                                                                                // Regular struct member
                        auto Member = Cast(visible_region_size*, Element->VisibleRegionSize);
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
            
            
            
            cs MemberName = CSz("EasingFunctionVisRP");

                        

          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Music");

                                                                                                // Regular struct member
                        auto Member = Cast(wav_sound*, &Element->Music);
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

