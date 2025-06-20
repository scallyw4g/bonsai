// src/engine/editor.cpp:540:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_render_group *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_render_group", Element), Params))
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
        
        
        
        cs MemberName = CSz("Program");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(shader*, &Element->Program),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Uniforms");
                                

        if (ToggleButton(Ui, CSz("v Uniforms[26]"), CSz("> Uniforms[26]"), UiId(Window, "toggle lighting_render_group shader_uniform Uniforms", Element->Uniforms), Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
          RangeIterator(ArrayIndex, 26)
          {
                        DoEditorUi(Ui, Window, Element->Uniforms+ArrayIndex, FSz("Uniforms[%d]", ArrayIndex), Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);




        
      }
      

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
        
        
        
        cs MemberName = CSz("gColor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, Element->gColor),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("gNormal");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, Element->gNormal),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("gDepth");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, Element->gDepth),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("shadowMap");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, Element->shadowMap),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Ssao");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, Element->Ssao),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("TransparencyAccumTex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, Element->TransparencyAccumTex),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("TransparencyCountTex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, Element->TransparencyCountTex),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("BravoilMyersOIT");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, Element->BravoilMyersOIT),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("BravoilMcGuireOIT");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, Element->BravoilMcGuireOIT),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("InverseViewMatrix");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(m4*, Element->InverseViewMatrix),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("InverseProjectionMatrix");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(m4*, Element->InverseProjectionMatrix),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ShadowMVP");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(m4*, Element->ShadowMVP),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("LightColors");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, Element->LightColors),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("LightPositions");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, Element->LightPositions),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("LightIndexToUV");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, Element->LightIndexToUV),
          MemberName,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("LightCount");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(s32*, Element->LightCount),
          MemberName,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("Camera");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(camera*, Element->Camera),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("SunPosition");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, Element->SunPosition),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("SunColor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, Element->SunColor),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("FogColor");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, Element->FogColor),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("FogPower");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, Element->FogPower),
          MemberName,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("UseSsao");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, Element->UseSsao),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("UseShadowMapping");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, Element->UseShadowMapping),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("UseLightingBloom");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, Element->UseLightingBloom),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("ApplicationResolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2*, Element->ApplicationResolution),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ShadowMapResolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v2*, Element->ShadowMapResolution),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("LuminanceTex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, &Element->LuminanceTex),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Lights");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(game_lights*, &Element->Lights),
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

