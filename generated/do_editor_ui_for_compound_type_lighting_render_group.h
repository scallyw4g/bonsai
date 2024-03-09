// src/engine/editor.cpp:330:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_render_group *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_render_group", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
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

