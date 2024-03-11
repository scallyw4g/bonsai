// src/engine/editor.cpp:96:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, noise_params *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle noise_params", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->Type,
        CSz("Type"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->EditParams,
        CSz("EditParams"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->PerlinParams,
        CSz("PerlinParams"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->VoronoiParams,
        CSz("VoronoiParams"),
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

