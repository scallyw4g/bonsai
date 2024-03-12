// src/engine/editor.cpp:354:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, game_lights *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle game_lights", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->ColorTex,
        CSz("ColorTex"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->PositionTex,
        CSz("PositionTex"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->IndexToUV,
        CSz("IndexToUV"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Count,
        CSz("Count"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->Lights,
        CSz("Lights"),
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

