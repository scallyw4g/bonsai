// callsite
// src/engine/editor.h:1094:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:495:0
struct maybe_ray;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, maybe_ray *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button, primitive_value_changed_record_block_array *ChangeRecords = 0)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x378E6FBD);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle maybe_ray", Element, ThisHash), Params))
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
            
            
            
            cs MemberName = CSz("Tag");

                                                                                                // Regular struct member
                        auto Member = Cast(maybe_tag*, &Element->Tag);
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
            
            
            
            cs MemberName = CSz("Ray");

                                                                                                // Regular struct member
                        auto Member = Cast(ray*, &Element->Ray);
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

