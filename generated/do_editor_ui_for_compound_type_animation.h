// src/engine/editor.cpp:177:0

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
&Element->t,
        CSz("t"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->tEnd,
        CSz("tEnd"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->xKeyframeCount,
        CSz("xKeyframeCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->xKeyframes,
        CSz("xKeyframes"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->yKeyframeCount,
        CSz("yKeyframeCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->yKeyframes,
        CSz("yKeyframes"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->zKeyframeCount,
        CSz("zKeyframeCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->zKeyframes,
        CSz("zKeyframes"),
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

