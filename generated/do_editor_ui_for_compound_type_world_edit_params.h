// src/engine/editor.h:708:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_params *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_edit_params", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->Mode,
        CSz("Mode"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Modifier,
        CSz("Modifier"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Iterations,
        CSz("Iterations"),
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

