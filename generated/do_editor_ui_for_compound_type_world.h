// src/engine/editor.cpp:147:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->Center,
        CSz("Center"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->VisibleRegion,
        CSz("VisibleRegion"),
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

