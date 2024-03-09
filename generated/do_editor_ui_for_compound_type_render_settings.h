// src/engine/editor.cpp:129:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, render_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle render_settings", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->UseSsao),
        CSz("UseSsao"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->UseShadowMapping),
        CSz("UseShadowMapping"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->UseLightingBloom),
        CSz("UseLightingBloom"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->BravoilMyersOIT),
        CSz("BravoilMyersOIT"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->BravoilMcGuireOIT),
        CSz("BravoilMcGuireOIT"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DrawMajorGrid),
        CSz("DrawMajorGrid"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DrawMinorGrid),
        CSz("DrawMinorGrid"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->MajorGridDim,
        CSz("MajorGridDim"),
        Params
,0, 32 );





      PushNewRow(Ui);

      

      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ToneMappingType,
        CSz("ToneMappingType"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Lighting,
        CSz("Lighting"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ApplicationResolution,
        CSz("ApplicationResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ShadowMapResolution,
        CSz("ShadowMapResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LuminanceMapResolution,
        CSz("LuminanceMapResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->iApplicationResolution,
        CSz("iApplicationResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->iShadowMapResolution,
        CSz("iShadowMapResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->iLuminanceMapResolution,
        CSz("iLuminanceMapResolution"),
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

