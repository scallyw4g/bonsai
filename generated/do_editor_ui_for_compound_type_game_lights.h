// src/engine/editor.cpp:421:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, game_lights *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle game_lights", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->ColorTex,
        CSz("texture ColorTex"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->PositionTex,
        CSz("texture PositionTex"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->IndexToUV,
        CSz("r32 IndexToUV"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Count,
        CSz("s32 Count"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->Lights,
        CSz("light Lights"),
        Params
        );
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

