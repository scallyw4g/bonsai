link_internal void
DoEditorUi(renderer_2d *Ui, model *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, CS(Name), CS(Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Vox, "vox_data Vox", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->Mesh, "untextured_3d_geometry_buffer Mesh", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->TransparentMesh, "untextured_3d_geometry_buffer TransparentMesh", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->Animation, "animation Animation", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->Dim, "v3i Dim", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  }
  else
  {
    PushNewRow(Ui);
  }
  /* PushTableEnd(Ui); */
}

