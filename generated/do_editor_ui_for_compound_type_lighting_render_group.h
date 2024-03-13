// src/engine/editor.cpp:380:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_render_group *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_render_group", Element), &DefaultUiRenderParams_Generic))
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
&Element->Lights,
        CSz("Lights"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->FBO,
        CSz("FBO"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->BloomTextureFBO,
        CSz("BloomTextureFBO"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LightingTex,
        CSz("LightingTex"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->BloomTex,
        CSz("BloomTex"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Shader,
        CSz("Shader"),
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

