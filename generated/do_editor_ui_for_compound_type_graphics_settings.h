// src/engine/editor.cpp:448:0

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
        CSz("resolution_setting Resolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ShadowQuality,
        CSz("shadow_quality_setting ShadowQuality"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LightingQuality,
        CSz("lighting_quality_setting LightingQuality"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ShaderLanguage,
        CSz("shader_language_setting ShaderLanguage"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WindowStartingSize,
        CSz("resolution_setting WindowStartingSize"),
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

