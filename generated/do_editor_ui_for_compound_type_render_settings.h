// src/engine/editor.cpp:223:0

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
        CSz("b32 UseSsao"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->UseShadowMapping),
        CSz("b32 UseShadowMapping"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->UseLightingBloom),
        CSz("b32 UseLightingBloom"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->BravoilMyersOIT),
        CSz("b32 BravoilMyersOIT"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->BravoilMcGuireOIT),
        CSz("b32 BravoilMcGuireOIT"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DrawMajorGrid),
        CSz("b32 DrawMajorGrid"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DrawMinorGrid),
        CSz("b32 DrawMinorGrid"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->MajorGridDim,
        CSz("r32 MajorGridDim"),
        Params
,0, 32 );





      PushNewRow(Ui);

      

      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ToneMappingType,
        CSz("tone_mapping_type ToneMappingType"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Lighting,
        CSz("lighting_settings Lighting"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ApplicationResolution,
        CSz("v2 ApplicationResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ShadowMapResolution,
        CSz("v2 ShadowMapResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LuminanceMapResolution,
        CSz("v2 LuminanceMapResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->iApplicationResolution,
        CSz("v2i iApplicationResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->iShadowMapResolution,
        CSz("v2i iShadowMapResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->iLuminanceMapResolution,
        CSz("v2i iLuminanceMapResolution"),
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

