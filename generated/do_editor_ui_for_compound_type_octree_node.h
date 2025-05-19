// src/engine/editor.cpp:440:0

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
      if (Name.Count) { PushTableStart(Ui); }

      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
            

      { 
        
        
        
        cs MemberName = CSz("Type");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(octree_node_type*, &Element->Type),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("HadNoVisibleSurface");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->HadNoVisibleSurface),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("Dirty");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->Dirty),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("WorldP");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3i*, &Element->WorldP),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Resolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3i*, &Element->Resolution),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Chunk");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_chunk*, Element->Chunk),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Children");
                                

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



      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Next");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(octree_node*, Element->Next),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Edits");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_ptr_block_array*, &Element->Edits),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Lock");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(bonsai_futex*, &Element->Lock),
          MemberName,
          Params
          );







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

