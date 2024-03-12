// src/engine/editor.cpp:244:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, asset *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle asset", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->LoadState,
        CSz("LoadState"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Id,
        CSz("Id"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LRUFrameIndex,
        CSz("LRUFrameIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Models,
        CSz("Models"),
        Params
        );
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

