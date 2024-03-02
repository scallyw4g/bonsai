// src/engine/editor.cpp:304:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, asset *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle asset", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&(Element->LoadState),
        CSz("asset_load_state LoadState"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->Id),
        CSz("asset_id Id"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->LRUFrameIndex),
        CSz("u64 LRUFrameIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->Models),
        CSz("model_buffer Models"),
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

