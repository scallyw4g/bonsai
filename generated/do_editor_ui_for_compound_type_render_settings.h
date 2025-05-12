// src/engine/editor.cpp:255:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, render_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle render_settings", Element), Params))
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
        
        
        
        cs MemberName = CSz("UseSsao");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->UseSsao),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("UseShadowMapping");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->UseShadowMapping),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("UseLightingBloom");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->UseLightingBloom),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("BravoilMyersOIT");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->BravoilMyersOIT),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("BravoilMcGuireOIT");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->BravoilMcGuireOIT),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("DrawMajorGrid");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->DrawMajorGrid),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("DrawMinorGrid");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->DrawMinorGrid),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("MajorGridDim");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(r32*, &Element->MajorGridDim),
          MemberName,
          Params
          , 0, 32 );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("DrawCameraGhost");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->DrawCameraGhost),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );





      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("CameraGhostSize");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(r32*, &Element->CameraGhostSize),
          MemberName,
          Params
          , 1, 100 );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("Ignored");
        
      }

      
      

      { 
        
        
        
        cs MemberName = CSz("Headless");
        
      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("ToneMappingType");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(tone_mapping_type*, &Element->ToneMappingType),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("GameCameraFOV");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->GameCameraFOV),
          MemberName,
          Params
          , 5, 250 );







      }

            PushNewRow(Ui);

      

      { 
        
        
        
        cs MemberName = CSz("Lighting");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(lighting_settings*, &Element->Lighting),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("ApplicationResolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2*, &Element->ApplicationResolution),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("ShadowMapResolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2*, &Element->ShadowMapResolution),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("LuminanceMapResolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2*, &Element->LuminanceMapResolution),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("iApplicationResolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2i*, &Element->iApplicationResolution),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("iShadowMapResolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2i*, &Element->iShadowMapResolution),
          MemberName,
          Params
          );







      }

      
      

      { 
        
        
        
        cs MemberName = CSz("iLuminanceMapResolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2i*, &Element->iLuminanceMapResolution),
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

