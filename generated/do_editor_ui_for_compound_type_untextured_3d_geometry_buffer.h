// src/engine/editor.cpp:186:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, untextured_3d_geometry_buffer *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle untextured_3d_geometry_buffer", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      

      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->Verts,
        CSz("Verts"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Normals,
        CSz("Normals"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Mat,
        CSz("Mat"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->End,
        CSz("End"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->At,
        CSz("At"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->Parent,
        CSz("Parent"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->BufferNeedsToGrow,
        CSz("BufferNeedsToGrow"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Timestamp,
        CSz("Timestamp"),
        Params
        );





      PushNewRow(Ui);
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

