// callsite
// src/engine/editor.cpp:530:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:495:0
struct model;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, model *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button, primitive_value_changed_record_block_array *ChangeRecords = 0)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x4BAE73F);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle model", Element, ThisHash), Params))
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
            
            
            
            cs MemberName = CSz("Node");

                                                                                                // Regular struct member
                        auto Member = Cast(octree_node*, Element->Node);
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
            
            
            
            cs MemberName = CSz("TransparentMesh");

                                                                                                // Regular struct member
                        auto Member = Cast(untextured_3d_geometry_buffer*, &Element->TransparentMesh);
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
            
            
            
            cs MemberName = CSz("Animation");

                                                                                                // Regular struct member
                        auto Member = Cast(animation*, &Element->Animation);
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

