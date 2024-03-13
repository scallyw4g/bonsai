// src/engine/editor.cpp:407:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, graphics_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle graphics_settings", Element), &DefaultUiRenderParams_Generic))
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
&Element->Resolution,
        CSz("Resolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ShadowQuality,
        CSz("ShadowQuality"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LightingQuality,
        CSz("LightingQuality"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ShaderLanguage,
        CSz("ShaderLanguage"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WindowStartingSize,
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

