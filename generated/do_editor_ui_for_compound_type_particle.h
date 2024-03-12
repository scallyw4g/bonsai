// src/engine/editor.cpp:168:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle particle", Element), &DefaultUiRenderParams_Generic))
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
&Element->Offset,
        CSz("Offset"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Color,
        CSz("Color"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->IsLight,
        CSz("IsLight"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->RemainingLifespan,
        CSz("RemainingLifespan"),
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

