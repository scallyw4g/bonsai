// src/engine/editor.cpp:310:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_position_info *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle entity_position_info", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&(Element->P),
        CSz("cp P"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->_CollisionVolumeRadius),
        CSz("v3 _CollisionVolumeRadius"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->Scale),
        CSz("r32 Scale"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->EulerAngles),
        CSz("v3 EulerAngles"),
        Params
        );
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

