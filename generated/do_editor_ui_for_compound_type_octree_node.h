// src/engine/editor.cpp:329:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, octree_node *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle octree_node", Element), Params))
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
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(octree_node_type*, &Element->Type),
        CSz("Type"),
        Params
        );







      
                              DoEditorUi(Ui,
        Window,
        Cast(b8*, &Element->HadNoVisibleSurface),
        CSz("HadNoVisibleSurface"),
        &DefaultUiRenderParams_Checkbox
        );





            PushNewRow(Ui);

                              DoEditorUi(Ui,
        Window,
        Cast(b8*, &Element->Dirty),
        CSz("Dirty"),
        &DefaultUiRenderParams_Checkbox
        );





            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3i*, &Element->WorldP),
        CSz("WorldP"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3i*, &Element->Resolution),
        CSz("Resolution"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_chunk*, Element->Chunk),
        CSz("Chunk"),
        Params
        );







      
                  if (ToggleButton(Ui, CSz("v Children[8]"), CSz("> Children[8]"), UiId(Window, "toggle octree_node octree_node Children", Element->Children), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 8)
        {
                    if (Element->Children[ArrayIndex]) {DoEditorUi(Ui, Window, Element->Children[ArrayIndex], CSz("Child"), Params);};

          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(octree_node*, Element->Next),
        CSz("Next"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_edit_ptr_block_array*, &Element->Edits),
        CSz("Edits"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(bonsai_futex*, &Element->Lock),
        CSz("Lock"),
        Params
        );
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

