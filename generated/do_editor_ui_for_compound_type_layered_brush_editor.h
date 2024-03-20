// src/engine/editor.cpp:325:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, layered_brush_editor *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle layered_brush_editor", Element), &DefaultUiRenderParams_Generic))
      {
        DidToggle = True;
        PushNewRow(Ui);
      }
      else
      {
        DrawChildren = False;
      }
    }

    if (DrawChildren)
    {
      PushTableStart(Ui);
      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
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





      
      DoEditorUi(Ui,
        Window,
&Element->SeedBrushWithSelection,
        CSz("SeedBrushWithSelection"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ApplyBrushOnClick,
        CSz("ApplyBrushOnClick"),
        Params
        );





      PushNewRow(Ui);
      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
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

