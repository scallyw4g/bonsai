// src/engine/editor.h:295:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_settings *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x4B67FED);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_settings", Element, ThisHash), Params))
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
          ThisHash,
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
          ThisHash,
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
          ThisHash,
          Params
          , -PI32, PI32 );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("SunP");
                

        
      }
      

      { 
        
        
        
        cs MemberName = CSz("FogPower");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(r32*, &Element->FogPower),
          MemberName,
          ThisHash,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("FogColor");
                                        PushColumn(Ui, CSz("FogColor")); DoColorPickerToggle(Ui, Window, &Element->FogColor, False, ThisHash);





        
      }
      

      { 
        
        
        
        cs MemberName = CSz("DawnIntensity");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->DawnIntensity),
          MemberName,
          ThisHash,
          Params
          , 0.f, 3.f );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("DawnHSV");
                                        PushColumn(Ui, CSz("DawnColor")); DoColorPickerToggle(Ui, Window, &Element->DawnHSV, False, ThisHash);





        
      }
      

      { 
        
        
        
        cs MemberName = CSz("SunIntensity");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->SunIntensity),
          MemberName,
          ThisHash,
          Params
          , 0.f, 3.f );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("SunHSV");
                                        PushColumn(Ui, CSz("SunColor")); DoColorPickerToggle(Ui, Window, &Element->SunHSV, False, ThisHash);





        
      }
      

      { 
        
        
        
        cs MemberName = CSz("DuskIntensity");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->DuskIntensity),
          MemberName,
          ThisHash,
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
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("DuskHSV");
                                        PushColumn(Ui, CSz("DuskColor")); DoColorPickerToggle(Ui, Window, &Element->DuskHSV, False, ThisHash);





        
      }
      

      { 
        
        
        
        cs MemberName = CSz("MoonIntensity");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->MoonIntensity),
          MemberName,
          ThisHash,
          Params
          , 0.f, 3.f );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("MoonHSV");
                                        PushColumn(Ui, CSz("MoonColor")); DoColorPickerToggle(Ui, Window, &Element->MoonHSV, False, ThisHash);





        
      }
      

      { 
        
        
        
        cs MemberName = CSz("CurrentSunColor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->CurrentSunColor),
          MemberName,
          ThisHash,
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

