// src/engine/editor.h:847:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, layered_brush_editor *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle layered_brush_editor", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->LayerCount,
        CSz("LayerCount"),
        Params
        );





      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v Layers[8]"), CSz("> Layers[8]"), UiId(Window, "toggle layered_brush_editor brush_layer Layers", Element->Layers), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 8)
        {
          DoEditorUi(Ui, Window, Element->Layers+ArrayIndex, FSz("Layers[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
&Element->PreviewChunkDim,
        CSz("PreviewChunkDim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Preview,
        CSz("Preview"),
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

