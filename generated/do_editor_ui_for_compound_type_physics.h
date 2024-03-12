// src/engine/editor.cpp:156:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, physics *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle physics", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->Velocity,
        CSz("Velocity"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Force,
        CSz("Force"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Delta,
        CSz("Delta"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Mass,
        CSz("Mass"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Speed,
        CSz("Speed"),
        Params
        );





      PushNewRow(Ui);
      CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
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

