// src/engine/editor.cpp:238:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle particle", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->Velocity,
        CSz("v3 Velocity"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Offset,
        CSz("v3 Offset"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Color,
        CSz("u8 Color"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->IsLight,
        CSz("b8 IsLight"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->RemainingLifespan,
        CSz("r32 RemainingLifespan"),
        Params
        );





      PushNewRow(Ui);
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

