link_internal void
DoEditorUi(renderer_2d *Ui, untextured_3d_geometry_buffer *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    DoEditorUi(Ui, Element->Verts, "v3 Verts", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, Element->Normals, "v3 Normals", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, Element->Mat, "vertex_material Mat", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->End, "u32 End", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->At, "u32 At", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
  }
  else
  {
    PushNewRow(Ui);
  }
}

