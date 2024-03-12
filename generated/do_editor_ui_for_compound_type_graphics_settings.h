// src/engine/editor.cpp:384:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, graphics_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle graphics_settings", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
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
      CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
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

