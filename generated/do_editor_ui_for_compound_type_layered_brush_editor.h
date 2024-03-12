// src/engine/editor.cpp:300:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, layered_brush_editor *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle layered_brush_editor", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->LayerCount,
        CSz("LayerCount"),
        Params
        );





      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v Layers[8]"), CSz("> Layers[8]"), UiId(Window, "toggle layered_brush_editor brush_layer Layers", Element->Layers), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 8)
        {
          DoEditorUi(Ui, Window, Element->Layers+ArrayIndex, FSz("Layers[%d]", ArrayIndex), Params);
          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
&Element->Preview,
        CSz("Preview"),
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

