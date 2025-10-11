// src/engine/editor.cpp:533:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, renderer_2d *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle renderer_2d", Element), Params))
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
        
        
        
        cs MemberName = CSz("TextGroup");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(render_buffers_2d*, Element->TextGroup),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("SolidQuadGeometryBuffer");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(gpu_mapped_ui_buffer*, &Element->SolidQuadGeometryBuffer),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("TexturedQuadRenderPass");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(textured_quad_render_pass*, &Element->TexturedQuadRenderPass),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("CustomQuadGeometryBuffer");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(gpu_mapped_ui_buffer*, &Element->CustomQuadGeometryBuffer),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("SpriteTextureArray");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, &Element->SpriteTextureArray),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("InteractionStackTop");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64*, &Element->InteractionStackTop),
          MemberName,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("MouseP");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2*, Element->MouseP),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("MouseDP");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2*, Element->MouseDP),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ScreenDim");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2*, Element->ScreenDim),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Input");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(input*, Element->Input),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ToggleTable");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(ui_toggle_hashtable*, &Element->ToggleTable),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("WindowTable");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(window_layout_hashtable*, &Element->WindowTable),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("MinimizedWindowBuffer");
                                

        if (ToggleButton(Ui, CSz("v MinimizedWindowBuffer[64]"), CSz("> MinimizedWindowBuffer[64]"), UiId(Window, "toggle renderer_2d window_layout MinimizedWindowBuffer", Element->MinimizedWindowBuffer), Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
          RangeIterator(ArrayIndex, 64)
          {
                        DoEditorUi(Ui, Window, Element->MinimizedWindowBuffer+ArrayIndex, FSz("MinimizedWindowBuffer[%d]", ArrayIndex), Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);




        
      }
      

      { 
        
        
        
        cs MemberName = CSz("HighestWindow");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(window_layout*, Element->HighestWindow),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Hover");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(interactable*, &Element->Hover),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Clicked");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(interactable*, &Element->Clicked),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Pressed");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(interactable*, &Element->Pressed),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("RequestedForceCapture");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->RequestedForceCapture),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("TextEdit");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(text_box_edit_state*, &Element->TextEdit),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("CommandBuffer");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(ui_render_command_buffer*, Element->CommandBuffer),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("RenderCommandArena");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(memory_arena*, &Element->RenderCommandArena),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("UiToggleArena");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(memory_arena*, &Element->UiToggleArena),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("WindowTableArena");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(memory_arena*, &Element->WindowTableArena),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("DebugColors");
                                

        if (ToggleButton(Ui, CSz("v DebugColors[128]"), CSz("> DebugColors[128]"), UiId(Window, "toggle renderer_2d v3 DebugColors", Element->DebugColors), Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
          RangeIterator(ArrayIndex, 128)
          {
                        DoEditorUi(Ui, Window, Element->DebugColors+ArrayIndex, FSz("DebugColors[%d]", ArrayIndex), Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);




        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ColorPalette");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3_cursor*, Element->ColorPalette),
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

