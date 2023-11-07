link_internal void
DoEditorUi(renderer_2d *Ui, model *Element, const char* Name)
{
  PushTableStart(Ui);
  DoEditorUi(Ui, &Element->Vox, "vox_data Vox");
  DoEditorUi(Ui, &Element->Mesh, "untextured_3d_geometry_buffer Mesh");
  DoEditorUi(Ui, &Element->TransparentMesh, "untextured_3d_geometry_buffer TransparentMesh");
  DoEditorUi(Ui, &Element->Dim, "chunk_dimension Dim");
  DoEditorUi(Ui, &Element->Animation, "animation Animation");
  PushTableEnd(Ui);
}

