// callsite
// src/engine/editor.cpp:555:0

// def ((builtin.for_datatypes))
// src/engine/editor.cpp:555:0






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, easing_function *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x35BB500A);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle easing_function", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Name");

                                                                                                auto Member = Cast(cs*, &Element->Name);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Points");

                                                                                                auto Member = Cast(v2_static_cursor_16*, &Element->Points);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_render_group *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1933080D);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_render_group", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[26]"),
                CSz("> Uniforms[26]"),
                UiId(Window, "toggle lighting_render_group shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 26;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FBO");

                                                                                                auto Member = Cast(framebuffer*, &Element->FBO);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("gColor");

                                                                                                auto Member = Cast(texture*, Element->gColor);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("gNormal");

                                                                                                auto Member = Cast(texture*, Element->gNormal);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("gDepth");

                                                                                                auto Member = Cast(texture*, Element->gDepth);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("shadowMap");

                                                                                                auto Member = Cast(texture*, Element->shadowMap);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Ssao");

                                                                                                auto Member = Cast(texture*, Element->Ssao);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TransparencyAccumTex");

                                                                                                auto Member = Cast(texture*, Element->TransparencyAccumTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TransparencyCountTex");

                                                                                                auto Member = Cast(texture*, Element->TransparencyCountTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BravoilMyersOIT");

                                                                        auto Member = Cast(b32*, Element->BravoilMyersOIT);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BravoilMcGuireOIT");

                                                                        auto Member = Cast(b32*, Element->BravoilMcGuireOIT);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("InverseViewMatrix");

                                                                                                auto Member = Cast(m4*, Element->InverseViewMatrix);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("InverseProjectionMatrix");

                                                                                                auto Member = Cast(m4*, Element->InverseProjectionMatrix);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ShadowMVP");

                                                                                                auto Member = Cast(m4*, Element->ShadowMVP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LightColors");

                                                                                                auto Member = Cast(texture*, Element->LightColors);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LightPositions");

                                                                                                auto Member = Cast(texture*, Element->LightPositions);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LightIndexToUV");

                                                                                                auto Member = Cast(f32*, Element->LightIndexToUV);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LightCount");

                                                                                                auto Member = Cast(s32*, Element->LightCount);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Camera");

                                                                                                auto Member = Cast(camera*, Element->Camera);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SunPosition");

                                                                                                auto Member = Cast(v3*, Element->SunPosition);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SunColor");

                                                                                                auto Member = Cast(v3*, Element->SunColor);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FogColor");

                                                                                                auto Member = Cast(v3*, Element->FogColor);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FogPower");

                                                                                                auto Member = Cast(f32*, Element->FogPower);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UseSsao");

                                                                        auto Member = Cast(b32*, Element->UseSsao);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UseShadowMapping");

                                                                        auto Member = Cast(b32*, Element->UseShadowMapping);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UseLightingBloom");

                                                                        auto Member = Cast(b32*, Element->UseLightingBloom);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ApplicationResolution");

                                                                                                auto Member = Cast(v2*, Element->ApplicationResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ShadowMapResolution");

                                                                                                auto Member = Cast(v2*, Element->ShadowMapResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LuminanceTex");

                                                                                                auto Member = Cast(texture*, &Element->LuminanceTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Lights");

                                                                                                auto Member = Cast(game_lights*, &Element->Lights);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, render_debug *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1E3B3011);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle render_debug", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BytesSolidGeoLastFrame");

                                                                                                auto Member = Cast(u32*, &Element->BytesSolidGeoLastFrame);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BytesTransGeoLastFrame");

                                                                                                auto Member = Cast(u32*, &Element->BytesTransGeoLastFrame);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("VertsDrawnLastFrame");

                                                                                                auto Member = Cast(u32*, &Element->VertsDrawnLastFrame);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawCallsLastFrame");

                                                                                                auto Member = Cast(u32*, &Element->DrawCallsLastFrame);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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











link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, ui_toggle *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x34E9B82B);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle ui_toggle", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Id");

                                                                                                auto Member = Cast(ui_id*, &Element->Id);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ToggledOn");

                                                                        auto Member = Cast(b32*, &Element->ToggledOn);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, bonsai_futex *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1CACA02D);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle bonsai_futex", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SignalValue");

                                                                                                auto Member = Cast(u32*, &Element->SignalValue);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ThreadsWaiting");

                                                                                                auto Member = Cast(u32*, &Element->ThreadsWaiting);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_settings *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x11FDB034);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle engine_settings", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Graphics");

                                                                                                auto Member = Cast(graphics_settings*, &Element->Graphics);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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











link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, layer_settings *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1D3D585A);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle layer_settings", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Type");

                                                            DoBrushTypePicker(Ui, Window, Element, ThisHash);




            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == BrushLayerType_Noise) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(noise_layer*, &Element->Noise);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == BrushLayerType_Shape) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(shape_layer*, &Element->Shape);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == BrushLayerType_Brush) 
          { 
            
            
            
            cs MemberName = {};

                                                             DoBrushBrushPicker(Ui, Window, Element, ThisHash) ;




            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Offset");

                                                                                                auto Member = Cast(v3*, &Element->Offset);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Rotation");

                                                                                                auto Member = Cast(v3*, &Element->Rotation);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Invert");

                                                                                                auto Member = Cast(b8*, &Element->Invert);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Normalized");

                                                                                                auto Member = Cast(b8*, &Element->Normalized);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Reserved");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ValueBias");

                                                                                                auto Member = Cast(r32*, &Element->ValueBias);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , -1.f,  1.f );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Power");

                                                                                                auto Member = Cast(r32*, &Element->Power);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ValueFunc");

                                                                                                auto Member = Cast(world_edit_blend_mode_modifier*, &Element->ValueFunc);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BlendMode");

                                                                                                auto Member = Cast(world_edit_blend_mode*, &Element->BlendMode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Smoothing");

                                                                                                auto Member = Cast(smooth_blend_params*, &Element->Smoothing);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ColorMode");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BasisOffset");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HSVColor");

                                                            
      DoColorPickerToggle(Ui, Window, &Element->HSVColor, False, ThisHash)
    ;




            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Disabled");

                        

          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, texture *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1414405E);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle texture", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ID");

                                                                                                auto Member = Cast(u32*, &Element->ID);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Dim");

                                                                                                auto Member = Cast(v2i*, &Element->Dim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Slices");

                                                                                                auto Member = Cast(u32*, &Element->Slices);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Channels");

                                                                                                auto Member = Cast(u32*, &Element->Channels);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("IsDepthTexture");

                                                                        auto Member = Cast(b32*, &Element->IsDepthTexture);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Format");

                                                                                                auto Member = Cast(texture_storage_format*, &Element->Format);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugName");

                                                                                                auto Member = Cast(cs*, &Element->DebugName);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Queued");

                                                                        auto Member = Cast(b32*, &Element->Queued);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, asset_thumbnail *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x26039060);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle asset_thumbnail", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Texture");

                                                                                                auto Member = Cast(texture*, &Element->Texture);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Camera");

                                                                                                auto Member = Cast(camera*, &Element->Camera);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_update_op_shape_params_cylinder *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x21B74063);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_update_op_shape_params_cylinder", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Radius");

                                                                                                auto Member = Cast(r32*, &Element->Radius);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Height");

                                                                                                auto Member = Cast(r32*, &Element->Height);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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













link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, vertex_material *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x25F8008E);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle vertex_material", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ColorIndex");

                                                                                                auto Member = Cast(u16*, &Element->ColorIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Transparency");

                                                                                                auto Member = Cast(u8*, &Element->Transparency);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Emission");

                                                                                                auto Member = Cast(u8*, &Element->Emission);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, white_noise_params *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1F5D090);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle white_noise_params", Element, ThisHash), Params))
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_update_op_shape_params_rect *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1D0F48B7);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_update_op_shape_params_rect", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Dim");

                                                                                                auto Member = Cast(v3*, &Element->Dim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, platform *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2477D8B8);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle platform", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LowPriority");

                                                                                                auto Member = Cast(work_queue*, &Element->LowPriority);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HighPriority");

                                                                                                auto Member = Cast(work_queue*, &Element->HighPriority);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LoRenderQ");

                                                                                                auto Member = Cast(work_queue*, &Element->LoRenderQ);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HiRenderQ");

                                                                                                auto Member = Cast(work_queue*, &Element->HiRenderQ);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HighPriorityWorkerCount");

                                                                                                auto Member = Cast(u32*, &Element->HighPriorityWorkerCount);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HighPriorityModeFutex");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->HighPriorityModeFutex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WorkerThreadsSuspendFutex");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->WorkerThreadsSuspendFutex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WorkerThreadsExitFutex");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->WorkerThreadsExitFutex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MouseP");

                                                                                                auto Member = Cast(v2*, &Element->MouseP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MouseDP");

                                                                                                auto Member = Cast(v2*, &Element->MouseDP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Memory");

                                                                                                auto Member = Cast(memory_arena*, Element->Memory);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GameTime");

                                                                                                auto Member = Cast(r32*, &Element->GameTime);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("dt");

                                                                                                auto Member = Cast(r32*, &Element->dt);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ScreenDim");

                                                                                                auto Member = Cast(v2*, &Element->ScreenDim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Input");

                                                                                                auto Member = Cast(input*, &Element->Input);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ServerState");

                                                                                                auto Member = Cast(server_state*, Element->ServerState);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, hotkeys *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1A16B0BB);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle hotkeys", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Debug_ToggleMenu");

                                                                        auto Member = Cast(b32*, &Element->Debug_ToggleMenu);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Debug_ToggleProfiling");

                                                                        auto Member = Cast(b32*, &Element->Debug_ToggleProfiling);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Debug_TriangulateIncrement");

                                                                        auto Member = Cast(b32*, &Element->Debug_TriangulateIncrement);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Debug_TriangulateDecrement");

                                                                        auto Member = Cast(b32*, &Element->Debug_TriangulateDecrement);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Left");

                                                                        auto Member = Cast(b32*, &Element->Left);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Right");

                                                                        auto Member = Cast(b32*, &Element->Right);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Forward");

                                                                        auto Member = Cast(b32*, &Element->Forward);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Backward");

                                                                        auto Member = Cast(b32*, &Element->Backward);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shadow_render_group *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x4F8F8BF);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle shadow_render_group", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FramebufferName");

                                                                                                auto Member = Cast(u32*, &Element->FramebufferName);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ShadowMap");

                                                                                                auto Member = Cast(texture*, &Element->ShadowMap);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Shader");

                                                                                                auto Member = Cast(shadow_map_shader*, &Element->Shader);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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










link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle_system *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x137DC8CC);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle particle_system", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Entropy");

                                                                                                auto Member = Cast(random_series*, &Element->Entropy);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SpawnType");

                                                                                                auto Member = Cast(particle_spawn_type*, &Element->SpawnType);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Drag");

                                                                                                auto Member = Cast(r32*, &Element->Drag);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Lifetime");

                                                                                                auto Member = Cast(r32*, &Element->Lifetime);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("EmissionDelay");

                                                                                                auto Member = Cast(r32*, &Element->EmissionDelay);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("EmissionLifespan");

                                                                                                auto Member = Cast(r32*, &Element->EmissionLifespan);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ActiveParticles");

                                                                                                auto Member = Cast(u32*, &Element->ActiveParticles);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LifespanMod");

                                                                                                auto Member = Cast(r32*, &Element->LifespanMod);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticleLifespan");

                                                                                                auto Member = Cast(r32*, &Element->ParticleLifespan);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticlesPerSecond");

                                                                                                auto Member = Cast(r32*, &Element->ParticlesPerSecond);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticleLightEmission");

                                                                                                auto Member = Cast(r32*, &Element->ParticleLightEmission);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticleLightEmissionChance");

                                                                                                auto Member = Cast(r32*, &Element->ParticleLightEmissionChance);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticleStartingTransparency");

                                                                                                auto Member = Cast(r32*, &Element->ParticleStartingTransparency);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticleEndingTransparency");

                                                                                                auto Member = Cast(r32*, &Element->ParticleEndingTransparency);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticleStartingDim");

                                                                                                auto Member = Cast(v3*, &Element->ParticleStartingDim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticleEndingDim");

                                                                                                auto Member = Cast(f32*, &Element->ParticleEndingDim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticleTurbMin");

                                                                                                auto Member = Cast(v3*, &Element->ParticleTurbMin);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ParticleTurbMax");

                                                                                                auto Member = Cast(v3*, &Element->ParticleTurbMax);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SpawnRegion");

                                                                                                auto Member = Cast(aabb*, &Element->SpawnRegion);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SystemMovementCoefficient");

                                                                                                auto Member = Cast(r32*, &Element->SystemMovementCoefficient);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ElapsedSinceLastEmission");

                                                                                                auto Member = Cast(r32*, &Element->ElapsedSinceLastEmission);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("PackedHSVColors");

                                                

            if (ToggleButton(Ui,
                CSz("v PackedHSVColors[6]"),
                CSz("> PackedHSVColors[6]"),
                UiId(Window, "toggle particle_system u16 PackedHSVColors", Element->PackedHSVColors, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 6;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->PackedHSVColors+ArrayIndex,
                  FSz("PackedHSVColors[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                 PushNewRow(Ui); 
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Particles");

                                                

            if (ToggleButton(Ui,
                CSz("v Particles[(4096)]"),
                CSz("> Particles[(4096)]"),
                UiId(Window, "toggle particle_system particle Particles", Element->Particles, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = (4096);

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Particles+ArrayIndex,
                  FSz("Particles[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, octree_node *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1C6C60D9);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle octree_node", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Flags");

                                                                                                auto Member = Cast(chunk_flag*, &Element->Flags);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Type");

                                                                                                auto Member = Cast(octree_node_type*, &Element->Type);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Dirty");

                                                                        auto Member = Cast(b32*, &Element->Dirty);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WorldP");

                                                                                                auto Member = Cast(v3i*, &Element->WorldP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Resolution");

                                                                                                auto Member = Cast(v3i*, &Element->Resolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Chunk");

                                                                                                auto Member = Cast(world_chunk*, Element->Chunk);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Children");

                                                

            if (ToggleButton(Ui,
                CSz("v Children[8]"),
                CSz("> Children[8]"),
                UiId(Window, "toggle octree_node octree_node Children", Element->Children, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 8;

              RangeIterator(ArrayIndex, End)
              {
                                if (Element->Children[ArrayIndex]) {DoEditorUi(Ui, Window, Element->Children[ArrayIndex], CSz("Child"), ThisHash, Params);};

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Next");

                                                                                                auto Member = Cast(octree_node*, Element->Next);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Edits");

                                                                                                auto Member = Cast(world_edit_ptr_block_array*, &Element->Edits);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Lock");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->Lock);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, voronoi_noise_params *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x38DB8E3);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle voronoi_noise_params", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Period");

                                                                                                auto Member = Cast(v3*, &Element->Period);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 0.1f, 20.f );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Squareness");

                                                                                                auto Member = Cast(r32*, &Element->Squareness);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MaskChance");

                                                                                                auto Member = Cast(r32*, &Element->MaskChance);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, interactable *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x7AFD8F6);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle interactable", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Id");

                                                                                                auto Member = Cast(ui_id*, &Element->Id);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MinP");

                                                                                                auto Member = Cast(v2*, &Element->MinP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MaxP");

                                                                                                auto Member = Cast(v2*, &Element->MaxP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Window");

                                                                                                auto Member = Cast(window_layout*, Element->Window);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, texture_block_array *Container, cs Name, u32 ParentHash, UI_FUNCTION_PROTO_NAMES)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x179A010D);

  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);
      IterateOver(Container, Element, ElementIndex)
      {
        if (Element)
        {
          DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          PushNewRow(Ui);
        }
      }
    }
    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S", Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}










link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, terrain_decoration_render_context *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x231A7121);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle terrain_decoration_render_context", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[4]"),
                CSz("> Uniforms[4]"),
                UiId(Window, "toggle terrain_decoration_render_context shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 4;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DestTex");

                                                                                                auto Member = Cast(texture*, Element->DestTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DestFBO");

                                                                                                auto Member = Cast(framebuffer*, Element->DestFBO);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DerivsTex");

                                                                                                auto Member = Cast(texture*, Element->DerivsTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkDim");

                                                                                                auto Member = Cast(v3*, &Element->ChunkDim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WorldspaceChunkBasis");

                                                                                                auto Member = Cast(v3*, &Element->WorldspaceChunkBasis);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkResolution");

                                                                                                auto Member = Cast(v3*, &Element->ChunkResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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

















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, terrain_finalize_render_context *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1AC2947);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle terrain_finalize_render_context", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[1]"),
                CSz("> Uniforms[1]"),
                UiId(Window, "toggle terrain_finalize_render_context shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 1;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DestTex");

                                                                                                auto Member = Cast(texture*, &Element->DestTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FBO");

                                                                                                auto Member = Cast(framebuffer*, &Element->FBO);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Ignored");

                                                                                                auto Member = Cast(u32*, &Element->Ignored);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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





















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, camera *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x17AFA196);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle camera", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Frust");

                                                                                                auto Member = Cast(frustum*, &Element->Frust);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CurrentP");

                                                                                                auto Member = Cast(cp*, &Element->CurrentP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("RenderSpacePosition");

                                                                                                auto Member = Cast(v3*, &Element->RenderSpacePosition);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Pitch");

                                                                                                auto Member = Cast(r32*, &Element->Pitch);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Roll");

                                                                                                auto Member = Cast(r32*, &Element->Roll);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Yaw");

                                                                                                auto Member = Cast(r32*, &Element->Yaw);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DistanceFromTarget");

                                                                                                auto Member = Cast(r32*, &Element->DistanceFromTarget);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TargetPitch");

                                                                                                auto Member = Cast(r32*, &Element->TargetPitch);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TargetRoll");

                                                                                                auto Member = Cast(r32*, &Element->TargetRoll);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TargetYaw");

                                                                                                auto Member = Cast(r32*, &Element->TargetYaw);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TargetDistanceFromTarget");

                                                                                                auto Member = Cast(r32*, &Element->TargetDistanceFromTarget);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Blend");

                                                                                                auto Member = Cast(r32*, &Element->Blend);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Speed");

                                                                                                auto Member = Cast(r32*, &Element->Speed);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Front");

                                                                                                auto Member = Cast(v3*, &Element->Front);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Right");

                                                                                                auto Member = Cast(v3*, &Element->Right);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Up");

                                                                                                auto Member = Cast(v3*, &Element->Up);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GhostId");

                                                                                                auto Member = Cast(entity_id*, &Element->GhostId);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ViewProjection");

                                                                                                auto Member = Cast(m4*, &Element->ViewProjection);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("InverseViewMatrix");

                                                                                                auto Member = Cast(m4*, &Element->InverseViewMatrix);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("InverseProjectionMatrix");

                                                                                                auto Member = Cast(m4*, &Element->InverseProjectionMatrix);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, window_layout_hashtable *Container, cs Name, u32 ParentHash, UI_FUNCTION_PROTO_NAMES)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x24ABD9A5);

  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);
      IterateOver(Container, Element, ElementIndex)
      {
        if (Element)
        {
          DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          PushNewRow(Ui);
        }
      }
    }
    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S", Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, prefab_hashtable *Container, cs Name, u32 ParentHash, UI_FUNCTION_PROTO_NAMES)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x25FC21AE);

  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);
      IterateOver(Container, Element, ElementIndex)
      {
        if (Element)
        {
          DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          PushNewRow(Ui);
        }
      }
    }
    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S", Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, renderer_2d *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1D97D1BB);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle renderer_2d", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TextGroup");

                                                                                                auto Member = Cast(render_buffers_2d*, Element->TextGroup);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SolidQuadGeometryBuffer");

                                                                                                auto Member = Cast(triple_buffered_gpu_mapped_ui_buffer*, &Element->SolidQuadGeometryBuffer);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TexturedQuadRenderPass");

                                                                                                auto Member = Cast(textured_quad_render_pass*, &Element->TexturedQuadRenderPass);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CustomQuadGeometryBuffer");

                                                                                                auto Member = Cast(triple_buffered_gpu_mapped_ui_buffer*, &Element->CustomQuadGeometryBuffer);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SpriteTextureArray");

                                                                                                auto Member = Cast(texture*, &Element->SpriteTextureArray);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("IconTextureArray");

                                                                                                auto Member = Cast(texture*, &Element->IconTextureArray);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MouseP");

                                                                                                auto Member = Cast(v2*, Element->MouseP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MouseDP");

                                                                                                auto Member = Cast(v2*, Element->MouseDP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ScreenDim");

                                                                                                auto Member = Cast(v2*, Element->ScreenDim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Input");

                                                                                                auto Member = Cast(input*, Element->Input);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ToggleTable");

                                                                                                auto Member = Cast(ui_toggle_hashtable*, &Element->ToggleTable);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WindowTable");

                                                                                                auto Member = Cast(window_layout_hashtable*, &Element->WindowTable);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MinimizedWindowBuffer");

                                                

            if (ToggleButton(Ui,
                CSz("v MinimizedWindowBuffer[64]"),
                CSz("> MinimizedWindowBuffer[64]"),
                UiId(Window, "toggle renderer_2d window_layout MinimizedWindowBuffer", Element->MinimizedWindowBuffer, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 64;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->MinimizedWindowBuffer+ArrayIndex,
                  FSz("MinimizedWindowBuffer[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HighestWindow");

                                                                                                auto Member = Cast(window_layout*, Element->HighestWindow);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Active");

                                                                                                auto Member = Cast(interactable*, &Element->Active);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Hover");

                                                                                                auto Member = Cast(interactable*, &Element->Hover);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Clicked");

                                                                                                auto Member = Cast(interactable*, &Element->Clicked);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Pressed");

                                                                                                auto Member = Cast(interactable*, &Element->Pressed);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("RequestedForceCapture");

                                                                        auto Member = Cast(b32*, &Element->RequestedForceCapture);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TextEdit");

                                                                                                auto Member = Cast(text_box_edit_state*, &Element->TextEdit);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CommandBuffer");

                                                                                                auto Member = Cast(ui_render_command_buffer*, Element->CommandBuffer);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("RenderCommandArena");

                                                                                                auto Member = Cast(memory_arena*, &Element->RenderCommandArena);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UiToggleArena");

                                                                                                auto Member = Cast(memory_arena*, &Element->UiToggleArena);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WindowTableArena");

                                                                                                auto Member = Cast(memory_arena*, &Element->WindowTableArena);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugColors");

                                                

            if (ToggleButton(Ui,
                CSz("v DebugColors[128]"),
                CSz("> DebugColors[128]"),
                UiId(Window, "toggle renderer_2d v3 DebugColors", Element->DebugColors, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 128;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->DebugColors+ArrayIndex,
                  FSz("DebugColors[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ColorPalette");

                                                                                                auto Member = Cast(v3_cursor*, Element->ColorPalette);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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














link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, frustum *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x358AF1CD);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle frustum", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("farClip");

                                                                                                auto Member = Cast(f32*, &Element->farClip);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("nearClip");

                                                                                                auto Member = Cast(f32*, &Element->nearClip);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FOV");

                                                                                                auto Member = Cast(f32*, &Element->FOV);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Top");

                                                                                                auto Member = Cast(plane*, &Element->Top);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Bottom");

                                                                                                auto Member = Cast(plane*, &Element->Bottom);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Left");

                                                                                                auto Member = Cast(plane*, &Element->Left);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Right");

                                                                                                auto Member = Cast(plane*, &Element->Right);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, prefab *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x289FF1D1);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle prefab", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Name");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Edits");

                                                                                                auto Member = Cast(world_edit_paged_list*, &Element->Edits);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SpawnCallback");

                                                                                                auto Member = Cast(prefab_spawn_callback*, &Element->SpawnCallback);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, plane *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2A9389DB);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle plane", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Normal");

                                                                                                auto Member = Cast(v3*, &Element->Normal);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DistanceToOrigin");

                                                                                                auto Member = Cast(f32*, &Element->DistanceToOrigin);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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














link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_render_context *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x21E7B9F7);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_edit_render_context", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[4]"),
                CSz("> Uniforms[4]"),
                UiId(Window, "toggle world_edit_render_context shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 4;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Framebuffers");

                                                

            if (ToggleButton(Ui,
                CSz("v Framebuffers[3]"),
                CSz("> Framebuffers[3]"),
                UiId(Window, "toggle world_edit_render_context rtt_framebuffer Framebuffers", Element->Framebuffers, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 3;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Framebuffers+ArrayIndex,
                  FSz("Framebuffers[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkDim");

                                                                                                auto Member = Cast(v3*, Element->ChunkDim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WorldspaceChunkBasis");

                                                                                                auto Member = Cast(v3*, Element->WorldspaceChunkBasis);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkResolution");

                                                                                                auto Member = Cast(v3*, Element->ChunkResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Type");

                                                                                                auto Member = Cast(s32*, &Element->Type);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BrushTextureFramebufferFreelist");

                                                                                                auto Member = Cast(rtt_framebuffer_paged_list*, &Element->BrushTextureFramebufferFreelist);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shape_layer_advanced_params *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x31853A01);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle shape_layer_advanced_params", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Hollow");

                                                                                                auto Member = Cast(r32*, &Element->Hollow);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Rounding");

                                                                                                auto Member = Cast(r32*, &Element->Rounding);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Stretch");

                                                                                                auto Member = Cast(v3*, &Element->Stretch);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Repeat");

                                                                                                auto Member = Cast(v3*, &Element->Repeat);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Rotation");

                        

          }
        }
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








link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_update_op_shape_params_sphere *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xD209A19);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_update_op_shape_params_sphere", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Location");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Radius");

                                                                                                auto Member = Cast(f32*, &Element->Radius);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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








link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, graphics_settings *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x17D77224);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle graphics_settings", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Resolution");

                                                                                                auto Member = Cast(resolution_setting*, &Element->Resolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ShadowQuality");

                                                                                                auto Member = Cast(shadow_quality_setting*, &Element->ShadowQuality);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LightingQuality");

                                                                                                auto Member = Cast(lighting_quality_setting*, &Element->LightingQuality);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ShaderLanguage");

                                                                                                auto Member = Cast(shader_language_setting*, &Element->ShaderLanguage);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WindowStartingSize");

                                                                                                auto Member = Cast(resolution_setting*, &Element->WindowStartingSize);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, graphics *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x36CA0243);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle graphics", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Initialized");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->Initialized);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("RenderGate");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->RenderGate);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Settings");

                                                                                                auto Member = Cast(render_settings*, &Element->Settings);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("PrevSettings");

                                                                                                auto Member = Cast(render_settings*, &Element->PrevSettings);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("OffsetOfWorldCenterToGrid");

                                                                                                auto Member = Cast(v3*, &Element->OffsetOfWorldCenterToGrid);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SunBasis");

                                                                                                auto Member = Cast(v3*, &Element->SunBasis);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SkyColor");

                                                                                                auto Member = Cast(v3*, &Element->SkyColor);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GameCamera");

                                                                                                auto Member = Cast(camera*, &Element->GameCamera);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugCamera");

                                                                                                auto Member = Cast(camera*, &Element->DebugCamera);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Camera");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Exposure");

                                                                                                auto Member = Cast(r32*, &Element->Exposure);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MainDrawList");

                                                                                                auto Member = Cast(octree_node_ptr_paged_list*, &Element->MainDrawList);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ShadowMapDrawList");

                                                                                                auto Member = Cast(octree_node_ptr_paged_list*, &Element->ShadowMapDrawList);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MinClipP_worldspace");

                                                                                                auto Member = Cast(v3*, &Element->MinClipP_worldspace);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MaxClipP_worldspace");

                                                                                                auto Member = Cast(v3*, &Element->MaxClipP_worldspace);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("gBuffer");

                                                                                                auto Member = Cast(g_buffer_render_group*, Element->gBuffer);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("AoGroup");

                                                                                                auto Member = Cast(ao_render_group*, Element->AoGroup);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SG");

                                                                                                auto Member = Cast(shadow_render_group*, Element->SG);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ColorPalette");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ColorPaletteTexture");

                                                                                                auto Member = Cast(texture*, &Element->ColorPaletteTexture);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Transparency");

                                                                                                auto Member = Cast(transparency_render_group*, &Element->Transparency);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Lighting");

                                                                                                auto Member = Cast(lighting_render_group*, &Element->Lighting);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Bloom");

                                                                                                auto Member = Cast(bloom_render_group*, &Element->Bloom);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Gaussian");

                                                                                                auto Member = Cast(gaussian_render_group*, &Element->Gaussian);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CompositeGroup");

                                                                                                auto Member = Cast(composite_render_group*, &Element->CompositeGroup);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TerrainShapingRC");

                                                                                                auto Member = Cast(terrain_shaping_render_context*, &Element->TerrainShapingRC);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TerrainDecorationRC");

                                                                                                auto Member = Cast(terrain_decoration_render_context*, &Element->TerrainDecorationRC);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TerrainDerivsRC");

                                                                                                auto Member = Cast(terrain_derivs_render_context*, &Element->TerrainDerivsRC);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TerrainFinalizeRC");

                                                                                                auto Member = Cast(terrain_finalize_render_context*, &Element->TerrainFinalizeRC);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WorldEditRC");

                                                                                                auto Member = Cast(world_edit_render_context*, &Element->WorldEditRC);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("NoiseFinalizeJobsPending");

                                                                                                auto Member = Cast(u32*, &Element->NoiseFinalizeJobsPending);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TotalChunkJobsActive");

                                                                                                auto Member = Cast(u32*, &Element->TotalChunkJobsActive);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("NoiseReadbackJobs");

                                                                                                auto Member = Cast(dummy_work_queue_entry_build_chunk_mesh_block_array*, &Element->NoiseReadbackJobs);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ImmediateGeometry");

                                                                                                auto Member = Cast(triple_buffered_gpu_mapped_element_buffer*, &Element->ImmediateGeometry);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GpuTimers");

                                                                                                auto Member = Cast(gpu_timer_block_array*, &Element->GpuTimers);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Memory");

                                                                                                auto Member = Cast(memory_arena*, Element->Memory);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, perlin_noise_params *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3926BA4A);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle perlin_noise_params", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Period");

                                                                                                auto Member = Cast(v3*, &Element->Period);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 0.1f, 20.f );







            


          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_block_array *Container, cs Name, u32 ParentHash, UI_FUNCTION_PROTO_NAMES)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3935224D);

  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);
      IterateOver(Container, Element, ElementIndex)
      {
        if (Element)
        {
          DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          PushNewRow(Ui);
        }
      }
    }
    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S", Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}














link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, physics *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x101A527C);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle physics", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Velocity");

                                                                                                auto Member = Cast(v3*, &Element->Velocity);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Force");

                                                                                                auto Member = Cast(v3*, &Element->Force);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Delta");

                                                                                                auto Member = Cast(v3*, &Element->Delta);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Mass");

                                                                                                auto Member = Cast(r32*, &Element->Mass);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Speed");

                                                                                                auto Member = Cast(r32*, &Element->Speed);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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





















































link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, smooth_blend_params *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2E4E4323);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle smooth_blend_params", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ValueBlend");

                                                                                                auto Member = Cast(r32*, &Element->ValueBlend);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , -1.f,  1.f );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ColorBlend");

                                                                                                auto Member = Cast(r32*, &Element->ColorBlend);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , -1.f,  1.f );







                        PushNewRow(Ui);



          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x12B39B25);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_edit", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Region");

                                                                                                auto Member = Cast(rect3cp*, &Element->Region);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Brush");

                                                                                                auto Member = Cast(world_edit_brush*, Element->Brush);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Rotation");

                                                                                                auto Member = Cast(v3*, &Element->Rotation);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , -180.f, 180.f );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Flags");

                                                                                                auto Member = Cast(u32*, &Element->Flags);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Dirty");

                                                                        auto Member = Cast(b32*, &Element->Dirty);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Selected");

                                                                        auto Member = Cast(b32*, &Element->Selected);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Ordinal");

                                                                                                auto Member = Cast(u32*, &Element->Ordinal);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3825E333);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle particle", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Velocity");

                                                                                                auto Member = Cast(v3*, &Element->Velocity);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Offset");

                                                                                                auto Member = Cast(v3*, &Element->Offset);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Color");

                                                                                                auto Member = Cast(u8*, &Element->Color);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("IsLight");

                                                                                                auto Member = Cast(b8*, &Element->IsLight);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("RemainingLifespan");

                                                                                                auto Member = Cast(r32*, &Element->RemainingLifespan);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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











link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, easing_function_visualizer_render_pass *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x8D1934E);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle easing_function_visualizer_render_pass", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[2]"),
                CSz("> Uniforms[2]"),
                UiId(Window, "toggle easing_function_visualizer_render_pass shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 2;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Func");

                                                                                                auto Member = Cast(easing_function*, Element->Func);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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













link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, input_event *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1548FB6C);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle input_event", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Clicked");

                                                                        auto Member = Cast(b32*, &Element->Clicked);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Pressed");

                                                                        auto Member = Cast(b32*, &Element->Pressed);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Released");

                                                                        auto Member = Cast(b32*, &Element->Released);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
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














link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_update_op_shape_params_plane *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2BB91B80);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_update_op_shape_params_plane", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Orientation");

                                                                                                auto Member = Cast(shape_axis*, &Element->Orientation);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Thickness");

                                                                                                auto Member = Cast(f32*, &Element->Thickness);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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





















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, selection_region *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xBA6A3C1);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle selection_region", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Clicks");

                                                                                                auto Member = Cast(u32*, &Element->Clicks);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Base");

                                                                                                auto Member = Cast(cp*, &Element->Base);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Region");

                                                                                                auto Member = Cast(rect3cp*, &Element->Region);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Diff");

                                                                                                auto Member = Cast(v3*, &Element->Diff);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("InitialSelect");

                                                                        auto Member = Cast(b32*, &Element->InitialSelect);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ModMode");

                                                                                                auto Member = Cast(selection_modification_mode*, &Element->ModMode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ModState");

                                                                                                auto Member = Cast(selection_modification_state*, &Element->ModState);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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












link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shadow_map_shader *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x21B57BD7);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle shadow_map_shader", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[4]"),
                CSz("> Uniforms[4]"),
                UiId(Window, "toggle shadow_map_shader shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 4;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ModelMatrix");

                                                                                                auto Member = Cast(m4*, &Element->ModelMatrix);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ViewProjection");

                                                                                                auto Member = Cast(m4*, &Element->ViewProjection);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MinClipP_worldspace");

                                                                                                auto Member = Cast(v3*, Element->MinClipP_worldspace);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MaxClipP_worldspace");

                                                                                                auto Member = Cast(v3*, Element->MaxClipP_worldspace);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, bloom_upsample_shader *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x35DFEBD8);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle bloom_upsample_shader", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[1]"),
                CSz("> Uniforms[1]"),
                UiId(Window, "toggle bloom_upsample_shader shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 1;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FilterRadius");

                                                                                                auto Member = Cast(f32*, Element->FilterRadius);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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














link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, render_settings *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3A47CBF1);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle render_settings", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UseSsao");

                                                                        auto Member = Cast(b32*, &Element->UseSsao);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UseShadowMapping");

                                                                        auto Member = Cast(b32*, &Element->UseShadowMapping);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UseLightingBloom");

                                                                        auto Member = Cast(b32*, &Element->UseLightingBloom);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BravoilMyersOIT");

                                                                        auto Member = Cast(b32*, &Element->BravoilMyersOIT);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BravoilMcGuireOIT");

                                                                        auto Member = Cast(b32*, &Element->BravoilMcGuireOIT);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawMajorGrid");

                                                                        auto Member = Cast(b32*, &Element->DrawMajorGrid);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawMinorGrid");

                                                                        auto Member = Cast(b32*, &Element->DrawMinorGrid);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MajorGridDim");

                                                                                                auto Member = Cast(r32*, &Element->MajorGridDim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 0, 32 );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawCameraGhost");

                                                                        auto Member = Cast(b32*, &Element->DrawCameraGhost);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CameraGhostSize");

                                                                                                auto Member = Cast(r32*, &Element->CameraGhostSize);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 1, 100 );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Ignored");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Headless");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ToneMappingType");

                                                                                                auto Member = Cast(tone_mapping_type*, &Element->ToneMappingType);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GameCameraFOV");

                                                                                                auto Member = Cast(f32*, &Element->GameCameraFOV);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 5, 250 );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Lighting");

                                                                                                auto Member = Cast(lighting_settings*, &Element->Lighting);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ApplicationResolution");

                                                                                                auto Member = Cast(v2*, &Element->ApplicationResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ShadowMapResolution");

                                                                                                auto Member = Cast(v2*, &Element->ShadowMapResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LuminanceMapResolution");

                                                                                                auto Member = Cast(v2*, &Element->LuminanceMapResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("iApplicationResolution");

                                                                                                auto Member = Cast(v2i*, &Element->iApplicationResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("iShadowMapResolution");

                                                                                                auto Member = Cast(v2i*, &Element->iShadowMapResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("iLuminanceMapResolution");

                                                                                                auto Member = Cast(v2i*, &Element->iLuminanceMapResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, window_layout *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3698CBF4);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle window_layout", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HashtableKey");

                                                                                                auto Member = Cast(ui_id*, &Element->HashtableKey);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Title");

                                                                                                auto Member = Cast(cs*, &Element->Title);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Minimized");

                                                                        auto Member = Cast(b32*, &Element->Minimized);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MinimizeIndex");

                                                                                                auto Member = Cast(u32*, &Element->MinimizeIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Flags");

                                                                                                auto Member = Cast(s32*, &Element->Flags);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Basis");

                                                                                                auto Member = Cast(v2*, &Element->Basis);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MaxClip");

                                                                                                auto Member = Cast(v2*, &Element->MaxClip);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Scroll");

                                                                                                auto Member = Cast(v2*, &Element->Scroll);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CachedFlags");

                                                                                                auto Member = Cast(s32*, &Element->CachedFlags);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CachedBasis");

                                                                                                auto Member = Cast(v2*, &Element->CachedBasis);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CachedMaxClip");

                                                                                                auto Member = Cast(v2*, &Element->CachedMaxClip);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CachedScroll");

                                                                                                auto Member = Cast(v2*, &Element->CachedScroll);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("InteractionStackIndex");

                                                                                                auto Member = Cast(u64*, &Element->InteractionStackIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("zBackground");

                                                                                                auto Member = Cast(r32*, &Element->zBackground);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("zText");

                                                                                                auto Member = Cast(r32*, &Element->zText);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("zBorder");

                                                                                                auto Member = Cast(r32*, &Element->zBorder);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("zTitleBar");

                                                                                                auto Member = Cast(r32*, &Element->zTitleBar);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("NextHotWindow");

                                                                                                auto Member = Cast(window_layout*, Element->NextHotWindow);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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






















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, level_editor *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x346D8421);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle level_editor", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Heap");

                                                                                                auto Member = Cast(heap_allocator*, &Element->Heap);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Memory");

                                                                                                auto Member = Cast(memory_arena*, Element->Memory);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SelectedTool");

                                                                                                auto Member = Cast(ui_editor_tool*, &Element->SelectedTool);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BrushWindowMode");

                                                                                                auto Member = Cast(brush_window_mode*, &Element->BrushWindowMode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Selection");

                                                                                                auto Member = Cast(selection_region*, &Element->Selection);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("AssetThumbnails");

                                                                                                auto Member = Cast(asset_thumbnail_block_array*, &Element->AssetThumbnails);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("NextLayerIndex");

                                                                                                auto Member = Cast(u32*, &Element->NextLayerIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Layers");

                                                                                                auto Member = Cast(world_edit_layer_block_array*, &Element->Layers);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SelectedLayerIndex");

                                                                                                auto Member = Cast(world_edit_layer_block_array_index*, &Element->SelectedLayerIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Edits");

                                                                                                auto Member = Cast(world_edit_block_array*, &Element->Edits);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SelectedEditIndices");

                                                                                                auto Member = Cast(world_edit_block_array_index_block_array*, &Element->SelectedEditIndices);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HotEdit");

                                                                                                auto Member = Cast(world_edit*, Element->HotEdit);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HotEditIndex");

                                                                                                auto Member = Cast(world_edit_block_array_index*, &Element->HotEditIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LoadedBrushes");

                                                                                                auto Member = Cast(world_edit_brush_hashtable*, &Element->LoadedBrushes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CurrentBrush");

                                                                                                auto Member = Cast(world_edit_brush*, Element->CurrentBrush);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CurrentBrush_SelectedLayerIndex");

                                                                                                auto Member = Cast(s32*, &Element->CurrentBrush_SelectedLayerIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Prefabs");

                                                                                                auto Member = Cast(prefab_hashtable*, &Element->Prefabs);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SelectedPrefab");

                                                                                                auto Member = Cast(prefab*, Element->SelectedPrefab);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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








link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, gen_chunk *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x434E433);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle gen_chunk", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Next");

                                                                                                auto Member = Cast(gen_chunk*, Element->Next);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Chunk");

                                                                                                auto Member = Cast(world_chunk*, &Element->Chunk);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Voxels");

                                                                                                auto Member = Cast(voxel*, Element->Voxels);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Mesh");

                                                                                                auto Member = Cast(gpu_mapped_element_buffer*, &Element->Mesh);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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










link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_chunk *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3331643E);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_chunk", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Next");

                                                                                                auto Member = Cast(world_chunk*, Element->Next);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CollisionVolume");

                                                                                                auto Member = Cast(v3i*, &Element->CollisionVolume);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Dim");

                                                                                                auto Member = Cast(v3i*, &Element->Dim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Occupancy");

                                                                                                auto Member = Cast(u64*, Element->Occupancy);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("xOccupancyBorder");

                                                                                                auto Member = Cast(u64*, Element->xOccupancyBorder);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FaceMasks");

                                                                                                auto Member = Cast(u64*, Element->FaceMasks);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("IsOnFreelist");

                                                                        auto Member = Cast(b32*, &Element->IsOnFreelist);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Handles");

                                                                                                auto Member = Cast(gpu_element_buffer_handles*, &Element->Handles);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("OcclusionQueryId");

                                                                                                auto Member = Cast(u32*, &Element->OcclusionQueryId);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("QueryActive");

                                                                        auto Member = Cast(b32*, &Element->QueryActive);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("OcclusionFrames");

                                                                        auto Member = Cast(b32*, &Element->OcclusionFrames);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("StandingSpots");

                                                                                                auto Member = Cast(voxel_position_cursor*, &Element->StandingSpots);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DimInChunks");

                                                                                                auto Member = Cast(v3i*, &Element->DimInChunks);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WorldP");

                                                                                                auto Member = Cast(v3i*, &Element->WorldP);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FilledCount");

                                                                                                auto Member = Cast(s32*, &Element->FilledCount);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Entities");

                                                                                                auto Member = Cast(entity_ptr_block_array*, &Element->Entities);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, ui_toggle_hashtable *Container, cs Name, u32 ParentHash, UI_FUNCTION_PROTO_NAMES)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x11C9E45C);

  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);
      IterateOver(Container, Element, ElementIndex)
      {
        if (Element)
        {
          DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          PushNewRow(Ui);
        }
      }
    }
    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S", Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_brush *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2083C45D);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_edit_brush", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("NameBuf");

                                    auto Value = CS(Element->NameBuf);
            DoEditorUi(Ui, Window, &Value, MemberName, ThisHash, Params);


                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BrushBlendMode");

                                                                                                auto Member = Cast(world_edit_blend_mode*, &Element->BrushBlendMode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Modifier");

                                                                                                auto Member = Cast(world_edit_blend_mode_modifier*, &Element->Modifier);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Smoothing");

                                                                                                auto Member = Cast(smooth_blend_params*, &Element->Smoothing);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LayerCount");

                                                                                                auto Member = Cast(s32*, &Element->LayerCount);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Layers");

                                                

            if (ToggleButton(Ui,
                CSz("v Layers[16]"),
                CSz("> Layers[16]"),
                UiId(Window, "toggle world_edit_brush brush_layer Layers", Element->Layers, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = s32(Element->LayerCount);
              Assert( End < 16 );

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Layers+ArrayIndex,
                  FSz("Layers[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, game_lights *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xFE5B45E);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle game_lights", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ColorTex");

                                                                                                auto Member = Cast(texture*, &Element->ColorTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("PositionTex");

                                                                                                auto Member = Cast(texture*, &Element->PositionTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("IndexToUV");

                                                                                                auto Member = Cast(r32*, &Element->IndexToUV);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Count");

                                                                                                auto Member = Cast(s32*, &Element->Count);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Lights");

                                                                                                auto Member = Cast(light*, Element->Lights);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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


















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, octree_node_freelist *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x17DC2499);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle octree_node_freelist", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("First");

                                                                                                auto Member = Cast(octree_node*, Element->First);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Memory");

                                                                                                auto Member = Cast(memory_arena*, Element->Memory);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Lock");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->Lock);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ElementsAllocated");

                                                                                                auto Member = Cast(u32*, &Element->ElementsAllocated);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, gen_chunk_freelist *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x188A1CBB);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle gen_chunk_freelist", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("First");

                                                                                                auto Member = Cast(gen_chunk*, Element->First);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Memory");

                                                                                                auto Member = Cast(memory_arena*, Element->Memory);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Lock");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->Lock);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ElementsAllocated");

                                                                                                auto Member = Cast(u32*, &Element->ElementsAllocated);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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










link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, memory_arena *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xB6B4CD6);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle memory_arena", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Start");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("At");

                                    auto Value =   u64(Element->End-Element->At); (Element->At);
            DoEditorUi(Ui, Window, &Value, MemberName, ThisHash, Params);


                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("End");

                                    auto Value =   u64(Element->End-Element->Start); (Element->End);
            DoEditorUi(Ui, Window, &Value, MemberName, ThisHash, Params);


                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("NextBlockSize");

                                                                                                auto Member = Cast(umm*, &Element->NextBlockSize);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Prev");

                                                                                                auto Member = Cast(memory_arena*, Element->Prev);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugFutex");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->DebugFutex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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










link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, render_buffers_2d *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x83E1CFD);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle render_buffers_2d", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugTextureArray");

                                                                                                auto Member = Cast(texture*, &Element->DebugTextureArray);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TextTextureUniform");

                                                                                                auto Member = Cast(s32*, &Element->TextTextureUniform);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UiShader");

                                                                                                auto Member = Cast(shader*, &Element->UiShader);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Buf");

                                                                                                auto Member = Cast(triple_buffered_gpu_mapped_ui_buffer*, &Element->Buf);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, input *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x8A2F50A);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle input", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Enter");

                                                                                                auto Member = Cast(input_event*, &Element->Enter);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Escape");

                                                                                                auto Member = Cast(input_event*, &Element->Escape);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Alt");

                                                                                                auto Member = Cast(input_event*, &Element->Alt);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Ctrl");

                                                                                                auto Member = Cast(input_event*, &Element->Ctrl);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Shift");

                                                                                                auto Member = Cast(input_event*, &Element->Shift);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Backspace");

                                                                                                auto Member = Cast(input_event*, &Element->Backspace);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Delete");

                                                                                                auto Member = Cast(input_event*, &Element->Delete);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F12");

                                                                                                auto Member = Cast(input_event*, &Element->F12);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F11");

                                                                                                auto Member = Cast(input_event*, &Element->F11);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F10");

                                                                                                auto Member = Cast(input_event*, &Element->F10);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F9");

                                                                                                auto Member = Cast(input_event*, &Element->F9);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F8");

                                                                                                auto Member = Cast(input_event*, &Element->F8);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F7");

                                                                                                auto Member = Cast(input_event*, &Element->F7);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F6");

                                                                                                auto Member = Cast(input_event*, &Element->F6);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F5");

                                                                                                auto Member = Cast(input_event*, &Element->F5);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F4");

                                                                                                auto Member = Cast(input_event*, &Element->F4);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F3");

                                                                                                auto Member = Cast(input_event*, &Element->F3);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F2");

                                                                                                auto Member = Cast(input_event*, &Element->F2);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F1");

                                                                                                auto Member = Cast(input_event*, &Element->F1);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Dot");

                                                                                                auto Member = Cast(input_event*, &Element->Dot);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Minus");

                                                                                                auto Member = Cast(input_event*, &Element->Minus);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FSlash");

                                                                                                auto Member = Cast(input_event*, &Element->FSlash);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Space");

                                                                                                auto Member = Cast(input_event*, &Element->Space);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N0");

                                                                                                auto Member = Cast(input_event*, &Element->N0);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N1");

                                                                                                auto Member = Cast(input_event*, &Element->N1);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N2");

                                                                                                auto Member = Cast(input_event*, &Element->N2);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N3");

                                                                                                auto Member = Cast(input_event*, &Element->N3);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N4");

                                                                                                auto Member = Cast(input_event*, &Element->N4);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N5");

                                                                                                auto Member = Cast(input_event*, &Element->N5);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N6");

                                                                                                auto Member = Cast(input_event*, &Element->N6);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N7");

                                                                                                auto Member = Cast(input_event*, &Element->N7);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N8");

                                                                                                auto Member = Cast(input_event*, &Element->N8);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N9");

                                                                                                auto Member = Cast(input_event*, &Element->N9);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("A");

                                                                                                auto Member = Cast(input_event*, &Element->A);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("B");

                                                                                                auto Member = Cast(input_event*, &Element->B);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("C");

                                                                                                auto Member = Cast(input_event*, &Element->C);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("D");

                                                                                                auto Member = Cast(input_event*, &Element->D);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("E");

                                                                                                auto Member = Cast(input_event*, &Element->E);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("F");

                                                                                                auto Member = Cast(input_event*, &Element->F);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("G");

                                                                                                auto Member = Cast(input_event*, &Element->G);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("H");

                                                                                                auto Member = Cast(input_event*, &Element->H);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("I");

                                                                                                auto Member = Cast(input_event*, &Element->I);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("J");

                                                                                                auto Member = Cast(input_event*, &Element->J);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("K");

                                                                                                auto Member = Cast(input_event*, &Element->K);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("L");

                                                                                                auto Member = Cast(input_event*, &Element->L);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("M");

                                                                                                auto Member = Cast(input_event*, &Element->M);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("N");

                                                                                                auto Member = Cast(input_event*, &Element->N);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("O");

                                                                                                auto Member = Cast(input_event*, &Element->O);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("P");

                                                                                                auto Member = Cast(input_event*, &Element->P);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Q");

                                                                                                auto Member = Cast(input_event*, &Element->Q);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("R");

                                                                                                auto Member = Cast(input_event*, &Element->R);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("S");

                                                                                                auto Member = Cast(input_event*, &Element->S);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("T");

                                                                                                auto Member = Cast(input_event*, &Element->T);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("U");

                                                                                                auto Member = Cast(input_event*, &Element->U);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("V");

                                                                                                auto Member = Cast(input_event*, &Element->V);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("W");

                                                                                                auto Member = Cast(input_event*, &Element->W);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("X");

                                                                                                auto Member = Cast(input_event*, &Element->X);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Y");

                                                                                                auto Member = Cast(input_event*, &Element->Y);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Z");

                                                                                                auto Member = Cast(input_event*, &Element->Z);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("RMB");

                                                                                                auto Member = Cast(input_event*, &Element->RMB);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LMB");

                                                                                                auto Member = Cast(input_event*, &Element->LMB);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MMB");

                                                                                                auto Member = Cast(input_event*, &Element->MMB);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MouseWheelDelta");

                                                                                                auto Member = Cast(s32*, &Element->MouseWheelDelta);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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








link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_brush_hashtable *Container, cs Name, u32 ParentHash, UI_FUNCTION_PROTO_NAMES)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2375951A);

  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);
      IterateOver(Container, Element, ElementIndex)
      {
        if (Element)
        {
          DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          PushNewRow(Ui);
        }
      }
    }
    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S", Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, bonsai_stdlib *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1311052E);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle bonsai_stdlib", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Os");

                                                                                                auto Member = Cast(os*, &Element->Os);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Plat");

                                                                                                auto Member = Cast(platform*, &Element->Plat);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("AppApi");

                                                                                                auto Member = Cast(application_api*, &Element->AppApi);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GL");

                                                                                                auto Member = Cast(opengl*, &Element->GL);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ThreadStates");

                                                                                                auto Member = Cast(thread_local_state*, Element->ThreadStates);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DefaultThreadState");

                                                                                                auto Member = Cast(thread_local_state*, &Element->DefaultThreadState);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ShaderHeaderFile");

                                                                                                auto Member = Cast(hot_reloadable_file*, &Element->ShaderHeaderFile);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ShaderHeaderCode");

                                                                                                auto Member = Cast(ansi_stream*, &Element->ShaderHeaderCode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("AllTextures");

                                                                                                auto Member = Cast(texture_block_array*, &Element->AllTextures);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("AllShaders");

                                                                                                auto Member = Cast(shader_ptr_block_array*, &Element->AllShaders);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugState");

                                                                                                auto Member = Cast(debug_state*, &Element->DebugState);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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











link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, chunk_thumbnail *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xB590D48);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle chunk_thumbnail", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Chunk");

                                                                                                auto Member = Cast(world_chunk*, &Element->Chunk);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Thumbnail");

                                                                                                auto Member = Cast(asset_thumbnail*, &Element->Thumbnail);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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

















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, keyframe *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x14339D6D);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle keyframe", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("tEnd");

                                                                                                auto Member = Cast(r32*, &Element->tEnd);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Value");

                                                                                                auto Member = Cast(r32*, &Element->Value);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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











link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, rectangular_lattice_params *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x5F2458A);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle rectangular_lattice_params", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Radius");

                                                                                                auto Member = Cast(r32*, &Element->Radius);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Jitter");

                                                                                                auto Member = Cast(f32*, &Element->Jitter);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Period");

                                                                                                auto Member = Cast(v3*, &Element->Period);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 0.1f, 20.f );







            


          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_update_op_shape_params_torus *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x39AD558C);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_update_op_shape_params_torus", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MajorRadius");

                                                                                                auto Member = Cast(f32*, &Element->MajorRadius);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MinorRadius");

                                                                                                auto Member = Cast(f32*, &Element->MinorRadius);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shader *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1813CD99);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle shader", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ID");

                                                                                                auto Member = Cast(u32*, &Element->ID);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                                                                auto Member = Cast(shader_uniform_buffer*, &Element->Uniforms);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("VertexSourceFilename");

                                                                                                auto Member = Cast(cs*, &Element->VertexSourceFilename);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FragSourceFilename");

                                                                                                auto Member = Cast(cs*, &Element->FragSourceFilename);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("VertexTimeModifiedWhenLoaded");

                                                                                                auto Member = Cast(s64*, &Element->VertexTimeModifiedWhenLoaded);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FragmentTimeModifiedWhenLoaded");

                                                                                                auto Member = Cast(s64*, &Element->FragmentTimeModifiedWhenLoaded);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HotReloaded");

                                                                        auto Member = Cast(b32*, &Element->HotReloaded);
            DoEditorUi(Ui,
              Window,
              Cast(b32*, Member),
              MemberName,
              ThisHash,
              &DefaultUiRenderParams_Checkbox
              );





                        PushNewRow(Ui);



          }
        }
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





















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, animation *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x34AA15E5);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle animation", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("t");

                                                                                                auto Member = Cast(r32*, &Element->t);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("tEnd");

                                                                                                auto Member = Cast(r32*, &Element->tEnd);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("xKeyframeCount");

                                                                                                auto Member = Cast(u32*, &Element->xKeyframeCount);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("xKeyframes");

                                                                                                auto Member = Cast(keyframe*, Element->xKeyframes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("yKeyframeCount");

                                                                                                auto Member = Cast(u32*, &Element->yKeyframeCount);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("yKeyframes");

                                                                                                auto Member = Cast(keyframe*, Element->yKeyframes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("zKeyframeCount");

                                                                                                auto Member = Cast(u32*, &Element->zKeyframeCount);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("zKeyframes");

                                                                                                auto Member = Cast(keyframe*, Element->zKeyframes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shape_layer *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x11CD2DF2);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle shape_layer", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Type");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == ShapeType_Rect) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(world_update_op_shape_params_rect*, &Element->Rect);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == ShapeType_Sphere) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(world_update_op_shape_params_sphere*, &Element->Sphere);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == ShapeType_Line) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(world_update_op_shape_params_line*, &Element->Line);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == ShapeType_Cylinder) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(world_update_op_shape_params_cylinder*, &Element->Cylinder);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == ShapeType_Plane) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(world_update_op_shape_params_plane*, &Element->Plane);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == ShapeType_Torus) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(world_update_op_shape_params_torus*, &Element->Torus);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == ShapeType_Pyramid) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(world_update_op_shape_params_pyramid*, &Element->Pyramid);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Advanced");

                                                                                                auto Member = Cast(shape_layer_advanced_params*, &Element->Advanced);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, noise_layer_2 *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x307DA5F6);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle noise_layer_2", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Type");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == NoiseType_White) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(white_noise_params*, &Element->White);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == NoiseType_Perlin) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(perlin_noise_params*, &Element->Perlin);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == NoiseType_Voronoi) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(voronoi_noise_params*, &Element->Voronoi);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Power");

                                                                                                auto Member = Cast(r32*, &Element->Power);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, untextured_3d_geometry_buffer *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x7A74DF9);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle untextured_3d_geometry_buffer", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Type");

                                                                                                auto Member = Cast(data_type*, &Element->Type);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Next");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Verts");

                                                                                                auto Member = Cast(void *, Element->Verts);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Normals");

                                                                                                auto Member = Cast(void *, Element->Normals);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Mat");

                                                                                                auto Member = Cast(vertex_material*, Element->Mat);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("End");

                                                                                                auto Member = Cast(u32*, &Element->End);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("At");

                                                                                                auto Member = Cast(u32*, &Element->At);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Parent");

                                                                                                auto Member = Cast(untextured_3d_geometry_buffer*, Element->Parent);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BufferNeedsToGrow");

                                                                                                auto Member = Cast(u32*, &Element->BufferNeedsToGrow);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Timestamp");

                                                                                                auto Member = Cast(u64*, &Element->Timestamp);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, work_queue *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3AF675FC);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle work_queue", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("EnqueueFutex");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->EnqueueFutex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("EnqueueIndex");

                                                                                                auto Member = Cast(u32*, &Element->EnqueueIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DequeueIndex");

                                                                                                auto Member = Cast(u32*, &Element->DequeueIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Entries");

                                                                                                auto Member = Cast(work_queue_entry*, Element->Entries);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, bloom_render_group *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x26D95603);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle bloom_render_group", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("BlurFBO");

                                                                                                auto Member = Cast(framebuffer*, &Element->BlurFBO);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Tex");

                                                                                                auto Member = Cast(texture*, &Element->Tex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MipChain");

                                                

            if (ToggleButton(Ui,
                CSz("v MipChain[(3)]"),
                CSz("> MipChain[(3)]"),
                UiId(Window, "toggle bloom_render_group texture MipChain", Element->MipChain, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = (3);

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->MipChain+ArrayIndex,
                  FSz("MipChain[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DownsampleShader");

                                                                                                auto Member = Cast(bloom_downsample_shader*, &Element->DownsampleShader);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UpsampleShader");

                                                                                                auto Member = Cast(bloom_upsample_shader*, &Element->UpsampleShader);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, terrain_derivs_render_context *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x302E0E17);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle terrain_derivs_render_context", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[1]"),
                CSz("> Uniforms[1]"),
                UiId(Window, "toggle terrain_derivs_render_context shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 1;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DestTex");

                                                                                                auto Member = Cast(texture*, &Element->DestTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DestFBO");

                                                                                                auto Member = Cast(framebuffer*, &Element->DestFBO);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Ignored");

                                                                                                auto Member = Cast(v3*, &Element->Ignored);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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





















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_debug *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x318BDE5D);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle engine_debug", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Memory");

                                                                                                auto Member = Cast(memory_arena*, Element->Memory);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ViewMode");

                                                                                                auto Member = Cast(engine_debug_view_mode*, &Element->ViewMode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("PickedChunks");

                                                                                                auto Member = Cast(picked_world_chunk_static_buffer*, &Element->PickedChunks);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Textures");

                                                                                                auto Member = Cast(texture_ptr_block_array*, &Element->Textures);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UiDebug");

                                                                                                auto Member = Cast(ui_debug*, &Element->UiDebug);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Render");

                                                                                                auto Member = Cast(render_debug*, &Element->Render);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawEntityCollisionVolumes");

                                                                                                auto Member = Cast(b8*, &Element->DrawEntityCollisionVolumes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawWorldAxies");

                                                                                                auto Member = Cast(b8*, &Element->DrawWorldAxies);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("TriggerRuntimeBreak");

                                                                                                auto Member = Cast(b8*, &Element->TriggerRuntimeBreak);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ResetAssetNodeView");

                                                                                                auto Member = Cast(b8*, &Element->ResetAssetNodeView);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawBranchNodesWithMeshes");

                                                                                                auto Member = Cast(b8*, &Element->DrawBranchNodesWithMeshes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawBranchNodes");

                                                                                                auto Member = Cast(b8*, &Element->DrawBranchNodes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawLeafNodes");

                                                                                                auto Member = Cast(b8*, &Element->DrawLeafNodes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawNodesWithChunks");

                                                                                                auto Member = Cast(b8*, &Element->DrawNodesWithChunks);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawQueuedNodes");

                                                                                                auto Member = Cast(b8*, &Element->DrawQueuedNodes);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MarkChunkBorderVoxels");

                                                                                                auto Member = Cast(b8*, &Element->MarkChunkBorderVoxels);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DrawGameCameraLocation");

                                                                                                auto Member = Cast(b8*, &Element->DrawGameCameraLocation);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkGenTimeElapsedMS");

                                                                                                auto Member = Cast(r64*, &Element->ChunkGenTimeElapsedMS);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CellsGenerated");

                                                                                                auto Member = Cast(u64*, &Element->CellsGenerated);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkGenCyclesElapsed");

                                                                                                auto Member = Cast(r64*, &Element->ChunkGenCyclesElapsed);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("PickedChunkState");

                                                                                                auto Member = Cast(u8*, &Element->PickedChunkState);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("PickedNode");

                                                                                                auto Member = Cast(octree_node*, Element->PickedNode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("AssetWindowViewMode");

                                                                                                auto Member = Cast(asset_window_view_mode*, &Element->AssetWindowViewMode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SelectedAsset");

                                                                                                auto Member = Cast(asset_id*, &Element->SelectedAsset);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ModelIndex");

                                                                                                auto Member = Cast(u64*, &Element->ModelIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SelectedEntity");

                                                                                                auto Member = Cast(entity_id*, &Element->SelectedEntity);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("OctreeDrawDepth");

                                                                                                auto Member = Cast(u32*, &Element->OctreeDrawDepth);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SelectedNode");

                                                                                                auto Member = Cast(octree_node*, Element->SelectedNode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, bloom_downsample_shader *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xA55F66A);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle bloom_downsample_shader", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[1]"),
                CSz("> Uniforms[1]"),
                UiId(Window, "toggle bloom_downsample_shader shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 1;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SrcResolution");

                                                                                                auto Member = Cast(v2*, Element->SrcResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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

















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, brush_layer *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x133E0EA1);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle brush_layer", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(layer_settings*, &Element->Settings);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("PrevSettings");

                        

          }
        }
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




















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, random_series *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x33A36ED5);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle random_series", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Seed");

                                                                                                auto Member = Cast(u64*, &Element->Seed);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_resources *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2FE376DA);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle engine_resources", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ReadyToStartMainLoop");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->ReadyToStartMainLoop);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Stdlib");

                                                                                                auto Member = Cast(bonsai_stdlib*, &Element->Stdlib);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("EngineApi");

                                                                                                auto Member = Cast(engine_api*, &Element->EngineApi);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Settings");

                                                                                                auto Member = Cast(engine_settings*, &Element->Settings);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Ui");

                                                                                                auto Member = Cast(renderer_2d*, &Element->Ui);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Hotkeys");

                                                                                                auto Member = Cast(hotkeys*, &Element->Hotkeys);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MaybeMouseRay");

                                                                                                auto Member = Cast(maybe_ray*, &Element->MaybeMouseRay);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("World");

                                                                                                auto Member = Cast(world*, Element->World);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GameState");

                                                                                                auto Member = Cast(game_state*, Element->GameState);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Graphics");

                                                                                                auto Member = Cast(graphics*, &Element->Graphics);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("RequestedGameLibReloadNode");

                                                                                                auto Member = Cast(file_traversal_node*, &Element->RequestedGameLibReloadNode);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("RequestedGameLibReloadBehavior");

                                                                                                auto Member = Cast(game_lib_reload_behavior*, &Element->RequestedGameLibReloadBehavior);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Heap");

                                                                                                auto Member = Cast(heap_allocator*, &Element->Heap);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GameMemory");

                                                                                                auto Member = Cast(memory_arena*, Element->GameMemory);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WorldUpdateMemory");

                                                                                                auto Member = Cast(memory_arena*, Element->WorldUpdateMemory);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("EntityTable");

                                                                                                auto Member = Cast(entity*, Element->EntityTable);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("AssetSystem");

                                                                                                auto Member = Cast(asset_system*, &Element->AssetSystem);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FrameIndex");

                                                                                                auto Member = Cast(u32*, &Element->FrameIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("geo_u3d_MeshFreelist");

                                                                                                auto Member = Cast(tiered_mesh_freelist*, &Element->geo_u3d_MeshFreelist);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("world_chunk_MeshFreelist");

                                                                                                auto Member = Cast(tiered_mesh_freelist*, &Element->world_chunk_MeshFreelist);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("EngineDebug");

                                                                                                auto Member = Cast(engine_debug*, &Element->EngineDebug);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Editor");

                                                                                                auto Member = Cast(level_editor*, &Element->Editor);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MousedOverVoxel");

                                                                                                auto Member = Cast(maybe_picked_voxel*, &Element->MousedOverVoxel);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ClosestStandingSpotToCursor");

                                                                                                auto Member = Cast(maybe_standing_spot*, &Element->ClosestStandingSpotToCursor);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("HoverEntity");

                                                                                                auto Member = Cast(maybe_entity_ptr*, &Element->HoverEntity);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("RTTGroup");

                                                                                                auto Member = Cast(render_to_texture_group*, &Element->RTTGroup);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GenChunkFreelist");

                                                                                                auto Member = Cast(gen_chunk_freelist*, &Element->GenChunkFreelist);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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















link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, rtt_framebuffer_static_cursor_3 *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x18804EFE);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle rtt_framebuffer_static_cursor_3", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Start");

                                                

            if (ToggleButton(Ui,
                CSz("v Start[3]"),
                CSz("> Start[3]"),
                UiId(Window, "toggle rtt_framebuffer_static_cursor_3 rtt_framebuffer Start", Element->Start, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = s32(Element->At);
              Assert( End < 3 );

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Start+ArrayIndex,
                  FSz("Start[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("At");

                                                                                                auto Member = Cast(u32*, &Element->At);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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
























link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, asset_thumbnail_block_array *Container, cs Name, u32 ParentHash, UI_FUNCTION_PROTO_NAMES)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2C0C745);

  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);
      IterateOver(Container, Element, ElementIndex)
      {
        if (Element)
        {
          DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          PushNewRow(Ui);
        }
      }
    }
    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S", Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}









link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, g_buffer_render_group *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x576CF56);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle g_buffer_render_group", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FBO");

                                                                                                auto Member = Cast(framebuffer*, &Element->FBO);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Textures");

                                                                                                auto Member = Cast(g_buffer_textures*, &Element->Textures);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("gBufferShader");

                                                                                                auto Member = Cast(shader*, &Element->gBufferShader);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("InverseViewMatrix");

                                                                                                auto Member = Cast(m4*, &Element->InverseViewMatrix);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("InverseProjectionMatrix");

                                                                                                auto Member = Cast(m4*, &Element->InverseProjectionMatrix);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ViewProjection");

                                                                                                auto Member = Cast(m4*, &Element->ViewProjection);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("GlTimerObject");

                                                                                                auto Member = Cast(u32*, &Element->GlTimerObject);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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








link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, terrain_shaping_render_context *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1818B763);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle terrain_shaping_render_context", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Program");

                                                                                                auto Member = Cast(shader*, &Element->Program);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Uniforms");

                                                

            if (ToggleButton(Ui,
                CSz("v Uniforms[5]"),
                CSz("> Uniforms[5]"),
                UiId(Window, "toggle terrain_shaping_render_context shader_uniform Uniforms", Element->Uniforms, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = 5;

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Uniforms+ArrayIndex,
                  FSz("Uniforms[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DestTex");

                                                                                                auto Member = Cast(texture*, &Element->DestTex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DestFBO");

                                                                                                auto Member = Cast(framebuffer*, &Element->DestFBO);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkDim");

                                                                                                auto Member = Cast(v3*, &Element->ChunkDim);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("WorldspaceChunkBasis");

                                                                                                auto Member = Cast(v3*, &Element->WorldspaceChunkBasis);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ChunkResolution");

                                                                                                auto Member = Cast(v3*, &Element->ChunkResolution);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ReshapeFunc");

                                                                                                auto Member = Cast(easing_function*, &Element->ReshapeFunc);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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
































link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_update_op_shape_params_pyramid *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2A40C7C9);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_update_op_shape_params_pyramid", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Height");

                                                                                                auto Member = Cast(f32*, &Element->Height);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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





link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, v2_static_cursor_16 *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1498EFCB);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle v2_static_cursor_16", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Start");

                                                

            if (ToggleButton(Ui,
                CSz("v Start[16]"),
                CSz("> Start[16]"),
                UiId(Window, "toggle v2_static_cursor_16 v2 Start", Element->Start, ThisHash),
                Params ))
            {
              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              PushNewRow(Ui);
                            s32 End = s32(Element->At);
              Assert( End < 16 );

              RangeIterator(ArrayIndex, End)
              {
                                DoEditorUi(Ui,
                  Window,
                  Element->Start+ArrayIndex,
                  FSz("Start[%d]", ArrayIndex),
                  ThisHash,
                  Params);

                
              }
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            PushNewRow(Ui);



            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("At");

                                                                                                auto Member = Cast(u32*, &Element->At);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, noise_layer *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x11D457CF);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle noise_layer", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Type");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == NoiseType_White) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(white_noise_params*, &Element->White);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == NoiseType_Perlin) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(perlin_noise_params*, &Element->Perlin);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == NoiseType_Voronoi) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(voronoi_noise_params*, &Element->Voronoi);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
           if (Element->Type == NoiseType_RectLattice) 
          { 
            
            
            
            cs MemberName = {};

                                                                                                auto Member = Cast(rectangular_lattice_params*, &Element->RectLattice);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Power");

                                                                                                auto Member = Cast(r32*, &Element->Power);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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











link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_settings *Element, cs Name, u32 ParentHash, ui_render_params *Params)

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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("AutoDayNightCycle");

                                                                                                auto Member = Cast(b8*, &Element->AutoDayNightCycle);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("tDaySpeed");

                                                                                                auto Member = Cast(r32*, &Element->tDaySpeed);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 1.f, 30.f );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("tDay");

                                                                                                auto Member = Cast(r32*, &Element->tDay);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , -PI32, PI32 );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SunP");

                        

          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FogPower");

                                                                                                auto Member = Cast(r32*, &Element->FogPower);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("FogColor");

                                                            PushColumn(Ui, CSz("FogColor")); DoColorPickerToggle(Ui, Window, &Element->FogColor, False, ThisHash);




            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DawnIntensity");

                                                                                                auto Member = Cast(f32*, &Element->DawnIntensity);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 0.f, 3.f );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DawnHSV");

                                                            PushColumn(Ui, CSz("DawnColor")); DoColorPickerToggle(Ui, Window, &Element->DawnHSV, False, ThisHash);




            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SunIntensity");

                                                                                                auto Member = Cast(f32*, &Element->SunIntensity);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 0.f, 3.f );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("SunHSV");

                                                            PushColumn(Ui, CSz("SunColor")); DoColorPickerToggle(Ui, Window, &Element->SunHSV, False, ThisHash);




            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DuskIntensity");

                                                                                                auto Member = Cast(f32*, &Element->DuskIntensity);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 0.f, 3.f );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DuskColor");

                                                                                                auto Member = Cast(v3*, &Element->DuskColor);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DuskHSV");

                                                            PushColumn(Ui, CSz("DuskColor")); DoColorPickerToggle(Ui, Window, &Element->DuskHSV, False, ThisHash);




            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MoonIntensity");

                                                                                                auto Member = Cast(f32*, &Element->MoonIntensity);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , 0.f, 3.f );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("MoonHSV");

                                                            PushColumn(Ui, CSz("MoonColor")); DoColorPickerToggle(Ui, Window, &Element->MoonHSV, False, ThisHash);




            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("CurrentSunColor");

                                                                                                auto Member = Cast(v3*, &Element->CurrentSunColor);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
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







link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, ui_debug *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x175317FA);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle ui_debug", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("OutlineUiValues");

                                                                                                auto Member = Cast(b8*, &Element->OutlineUiValues);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("OutlineUiButtons");

                                                                                                auto Member = Cast(b8*, &Element->OutlineUiButtons);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("OutlineUiTables");

                                                                                                auto Member = Cast(b8*, &Element->OutlineUiTables);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("OutlineUiTableColumns");

                                                                                                auto Member = Cast(b8*, &Element->OutlineUiTableColumns);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugBreakOnElementClick");

                                                                                                auto Member = Cast(b8*, &Element->DebugBreakOnElementClick);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("DebugBreakUiCommand");

                                                                                                auto Member = Cast(b8*, &Element->DebugBreakUiCommand);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LogClickEvents");

                                                                                                auto Member = Cast(b8*, &Element->LogClickEvents);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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






link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_chunk_freelist *Element, cs Name, u32 ParentHash, ui_render_params *Params)

{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2EA127FE);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_chunk_freelist", Element, ThisHash), Params))
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("First");

                                                                                                auto Member = Cast(world_chunk*, Element->First);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Memory");

                                                                                                auto Member = Cast(memory_arena*, Element->Memory);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Lock");

                                                                                                auto Member = Cast(bonsai_futex*, &Element->Lock);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ElementsAllocated");

                                                                                                auto Member = Cast(u32*, &Element->ElementsAllocated);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
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




