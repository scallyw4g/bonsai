link_internal void
DoEditorUi(renderer_2d *Ui, untextured_3d_geometry_buffer *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, Element->Verts, CSz("v3 Verts"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Element->Normals, CSz("v3 Normals"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Element->Mat, CSz("vertex_material Mat"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, &Element->End, CSz("u32 End"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->At, CSz("u32 At"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, Element->Parent, CSz("untextured_3d_geometry_buffer Parent"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, &Element->BufferNeedsToGrow, CSz("u32 BufferNeedsToGrow"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->Timestamp, CSz("u64 Timestamp"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






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

