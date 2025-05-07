// src/engine/editor.cpp:447:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, g_buffer_render_group *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle g_buffer_render_group", Element), Params))
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
        
        
        
        cs MemberName = CSz("FBO");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(framebuffer*, &Element->FBO),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Textures");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(g_buffer_textures*, &Element->Textures),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("gBufferShader");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(shader*, &Element->gBufferShader),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("InverseViewMatrix");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(m4*, &Element->InverseViewMatrix),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("InverseProjectionMatrix");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(m4*, &Element->InverseProjectionMatrix),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("ViewProjection");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(m4*, &Element->ViewProjection),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("GlTimerObject");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->GlTimerObject),
          MemberName,
          Params
          );







      }

            PushNewRow(Ui);
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

