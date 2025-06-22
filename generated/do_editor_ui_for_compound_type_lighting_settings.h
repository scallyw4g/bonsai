// src/engine/editor.cpp:306:0

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








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("tDaySpeed");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(r32*, &Element->tDaySpeed),
          MemberName,
          Params
          , 1.f, 30.f );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("tDay");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(r32*, &Element->tDay),
          MemberName,
          Params
          , -1.f, 1.f );








                PushNewRow(Ui);

      }
      

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








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("DawnHSV");
                                        PushColumn(Ui, CSz("DawnColor")); DoColorPickerToggle(Ui, Window, &Element->DawnHSV, False);





        
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








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("SunHSV");
                                        PushColumn(Ui, CSz("SunColor")); DoColorPickerToggle(Ui, Window, &Element->SunHSV, False);





        
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








                PushNewRow(Ui);

      }
      

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
        
        
        
        cs MemberName = CSz("DuskHSV");
                                        PushColumn(Ui, CSz("DuskColor")); DoColorPickerToggle(Ui, Window, &Element->DuskHSV, False);





        
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








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("MoonHSV");
                                        PushColumn(Ui, CSz("MoonColor")); DoColorPickerToggle(Ui, Window, &Element->MoonHSV, False);





        
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

