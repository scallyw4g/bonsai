link_internal void
DoEditorUi(renderer_2d *Ui, model *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, &Element->Vox, CSz("vox_data Vox"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, &Element->Meshes, CSz("lod_element_buffer Meshes"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, &Element->TransparentMesh, CSz("untextured_3d_geometry_buffer TransparentMesh"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, &Element->Animation, CSz("animation Animation"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, &Element->Dim, CSz("v3i Dim"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

