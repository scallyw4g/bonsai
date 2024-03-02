// src/engine/editor.cpp:246:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, untextured_3d_geometry_buffer *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle untextured_3d_geometry_buffer", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      

      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        (Element->Verts),
        CSz("v3 Verts"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        (Element->Normals),
        CSz("v3 Normals"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        (Element->Mat),
        CSz("vertex_material Mat"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->End),
        CSz("u32 End"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->At),
        CSz("u32 At"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        (Element->Parent),
        CSz("untextured_3d_geometry_buffer Parent"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->BufferNeedsToGrow),
        CSz("u32 BufferNeedsToGrow"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->Timestamp),
        CSz("u64 Timestamp"),
        Params
        );





      PushNewRow(Ui);
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

