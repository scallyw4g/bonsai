// src/engine/editor.cpp:279:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, frustum *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle frustum", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->farClip,
        CSz("farClip"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->nearClip,
        CSz("nearClip"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->width,
        CSz("width"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->FOV,
        CSz("FOV"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Top,
        CSz("Top"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Bot,
        CSz("Bot"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Left,
        CSz("Left"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Right,
        CSz("Right"),
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

