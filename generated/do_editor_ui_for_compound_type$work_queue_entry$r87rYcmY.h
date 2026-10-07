// callsite
// src/engine/editor.cpp:348:0

// def (do_editor_ui_for_compound_type)
// external/bonsai_stdlib/src/poof_functions.h:3253:0
struct work_queue_task;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, work_queue_task *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button, base_ptr_relative_edit_block_array *UiChangeEvents = 0)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x21A579B7);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle work_queue_task", Element, ThisHash), Params))
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
      PushTableStart(Ui);

      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
            {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Queue");

                                                                                                // Regular struct member
                        auto Member = Cast(work_queue_ptr*, &Element->Queue);
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
            
            
            
            cs MemberName = CSz("Type");

                                                                                                // Regular struct member
                        auto Member = Cast(work_queue_task_type*, &Element->Type);
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
            
            
            
            cs MemberName = CSz("anonymous");

                                                                                                
                        
            








            


          }
        }
      }

      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
      PushTableEnd(Ui);
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

