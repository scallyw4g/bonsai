// src/engine/editor.cpp:363:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, render_debug *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle render_debug", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->BytesSolidGeoLastFrame,
        CSz("BytesSolidGeoLastFrame"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->BytesTransGeoLastFrame,
        CSz("BytesTransGeoLastFrame"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DrawCallsLastFrame,
        CSz("DrawCallsLastFrame"),
        Params
        );





      PushNewRow(Ui);
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

