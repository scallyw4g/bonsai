// src/engine/editor.cpp:464:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, graphics_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle graphics_settings", Element), Params))
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
      PushTableStart(Ui);
      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
                                                      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(resolution_setting*, &Element->Resolution),
        CSz("Resolution"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(shadow_quality_setting*, &Element->ShadowQuality),
        CSz("ShadowQuality"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(lighting_quality_setting*, &Element->LightingQuality),
        CSz("LightingQuality"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(shader_language_setting*, &Element->ShaderLanguage),
        CSz("ShaderLanguage"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(resolution_setting*, &Element->WindowStartingSize),
        CSz("WindowStartingSize"),
        Params
        );
      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
      PushTableEnd(Ui);
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

