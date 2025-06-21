// src/engine/editor.cpp:175:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, brush_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle brush_settings", Element), Params))
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
        
        
        
        cs MemberName = CSz("BrushType");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(brush_layer_type*, &Element->Type),
          MemberName,
          Params
          );








        
      }
       if (Element->Type == BrushLayerType_Noise) 

      { 
        
        
        
        cs MemberName = {};
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(noise_layer*, &Element->Noise),
          MemberName,
          Params
          );








        
      }
       if (Element->Type == BrushLayerType_Shape) 

      { 
        
        
        
        cs MemberName = {};
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(shape_layer*, &Element->Shape),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Power");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->Power),
          MemberName,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("ValueBias");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(r32*, &Element->ValueBias),
          MemberName,
          Params
          , -1.f,  1.f );








                PushNewRow(Ui);

      }
       if (HasThresholdModifier(Element)) 

      { 
        
        
        
        cs MemberName = CSz("Threshold");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->Threshold),
          MemberName,
          Params
          ,  0.f,  1.f );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("ValueModifier");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_blend_mode_modifier*, &Element->ValueModifier),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("BlendMode");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_blend_mode*, &Element->BlendMode),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ColorMode");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_color_blend_mode*, &Element->ColorMode),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Invert");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(b8*, &Element->Invert),
          MemberName,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("Iterations");
        

                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("Offset");
        

        
      }
      

      { 
        
        
        
        cs MemberName = CSz("NoiseBasisOffset");
        

        
      }
      

      { 
        
        
        
        cs MemberName = CSz("HSVColor");
                                        PushColumn(Ui, CSz("HSVColor")); DoColorPickerToggle(Ui, Window, &Element->HSVColor, False);





        
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

