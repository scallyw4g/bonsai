// src/engine/editor.cpp:197:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, untextured_3d_geometry_buffer *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle untextured_3d_geometry_buffer", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, Window, Element->Verts, CSz("v3 Verts"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, Element->Normals, CSz("v3 Normals"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, Element->Mat, CSz("vertex_material Mat"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, &Element->End, CSz("u32 End"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->At, CSz("u32 At"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, Element->Parent, CSz("untextured_3d_geometry_buffer Parent"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, &Element->BufferNeedsToGrow, CSz("u32 BufferNeedsToGrow"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->Timestamp, CSz("u64 Timestamp"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

