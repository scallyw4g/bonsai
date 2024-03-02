// src/engine/editor.cpp:237:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, animation *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle animation", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&(Element->t),
        CSz("r32 t"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->tEnd),
        CSz("r32 tEnd"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->xKeyframeCount),
        CSz("u32 xKeyframeCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        (Element->xKeyframes),
        CSz("keyframe xKeyframes"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->yKeyframeCount),
        CSz("u32 yKeyframeCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        (Element->yKeyframes),
        CSz("keyframe yKeyframes"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->zKeyframeCount),
        CSz("u32 zKeyframeCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        (Element->zKeyframes),
        CSz("keyframe zKeyframes"),
        Params
        );
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

