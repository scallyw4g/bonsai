// src/engine/editor.cpp:250:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_position_info *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle entity_position_info", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->P,
        CSz("P"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->_CollisionVolumeRadius,
        CSz("_CollisionVolumeRadius"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Scale,
        CSz("Scale"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->EulerAngles,
        CSz("EulerAngles"),
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

