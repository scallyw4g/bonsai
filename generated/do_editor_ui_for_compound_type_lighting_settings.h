// src/engine/editor.cpp:258:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_settings", Element), Params))
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
        
        
        
        cs MemberName = CSz("AutoDayNightCycle");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(b8*, &Element->AutoDayNightCycle),
          MemberName,
          Params
          );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("tDay");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(r32*, &Element->tDay),
          MemberName,
          Params
          , -PI32, PI32 );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("SunP");
        
      }

      
      

      { 
        
        
        
        cs MemberName = CSz("DawnIntensity");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->DawnIntensity),
          MemberName,
          Params
          , 0.f, 3.f );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("DawnColor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->DawnColor),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("SunIntensity");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->SunIntensity),
          MemberName,
          Params
          , 0.f, 3.f );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("SunColor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->SunColor),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("DuskIntensity");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->DuskIntensity),
          MemberName,
          Params
          , 0.f, 3.f );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("DuskColor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->DuskColor),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("MoonIntensity");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->MoonIntensity),
          MemberName,
          Params
          , 0.f, 3.f );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("MoonColor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->MoonColor),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("CurrentSunColor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->CurrentSunColor),
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

