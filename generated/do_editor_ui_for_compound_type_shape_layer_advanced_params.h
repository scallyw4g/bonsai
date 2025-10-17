// src/engine/editor.cpp:173:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shape_layer_advanced_params *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle shape_layer_advanced_params", Element), Params))
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
        
        
        
        cs MemberName = CSz("Rounding");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(r32*, &Element->Rounding),
          MemberName,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("Stretch");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->Stretch),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Repeat");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->Repeat),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Axis");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->Axis),
          MemberName,
          Params
          , -1.f, 1.f );








        
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

