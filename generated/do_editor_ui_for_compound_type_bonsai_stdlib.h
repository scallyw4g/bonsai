// src/engine/editor.cpp:240:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, bonsai_stdlib *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle bonsai_stdlib", Element), Params))
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
        
        
        
        cs MemberName = CSz("Os");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(os*, &Element->Os),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Plat");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(platform*, &Element->Plat),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("ThreadStates");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(thread_local_state*, Element->ThreadStates),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("AllTextures");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture_block_array*, &Element->AllTextures),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("AllShaders");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(shader_ptr_block_array*, &Element->AllShaders),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("DebugState");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(debug_state*, &Element->DebugState),
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

