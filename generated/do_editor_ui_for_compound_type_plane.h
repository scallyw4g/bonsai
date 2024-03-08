// src/engine/editor.cpp:249:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, plane *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle plane", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->P,
        CSz("P"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Normal,
        CSz("Normal"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->d,
        CSz("d"),
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

