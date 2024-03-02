// src/engine/editor.cpp:226:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, physics *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle physics", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->Velocity,
        CSz("v3 Velocity"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Force,
        CSz("v3 Force"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Delta,
        CSz("v3 Delta"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Mass,
        CSz("r32 Mass"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Speed,
        CSz("r32 Speed"),
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

