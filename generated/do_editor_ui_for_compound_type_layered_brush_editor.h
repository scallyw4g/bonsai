// src/engine/editor.cpp:353:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, layered_brush_editor *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
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
      if (ToggleButton(Ui, CSz("v NameBuf[(256) + 1]"), CSz("> NameBuf[(256) + 1]"), UiId(Window, "toggle layered_brush_editor char  NameBuf", Element->NameBuf), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (256) + 1)
        {
          DoEditorUi(Ui, Window, Element->NameBuf+ArrayIndex, FSz("NameBuf[%d]", ArrayIndex), Params);
 PushNewRow(Ui); 
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s32*,&Element->LayerCount),
        CSz("LayerCount"),
        Params
        );






      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v Layers[16]"), CSz("> Layers[16]"), UiId(Window, "toggle layered_brush_editor brush_layer Layers", Element->Layers), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 16)
        {
          DoEditorUi(Ui, Window, Element->Layers+ArrayIndex, FSz("Layers[%d]", ArrayIndex), Params);
          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(b8*,&Element->SeedBrushWithSelection),
        CSz("SeedBrushWithSelection"),
        Params
        );






      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(b8*,&Element->BrushFollowsCursor),
        CSz("BrushFollowsCursor"),
        Params
        );






      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(chunk_thumbnail*,&Element->Preview),
        CSz("Preview"),
        Params
        );
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

