// src/engine/editor.cpp:333:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, graphics_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle graphics_settings", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
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
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
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

