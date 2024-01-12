link_internal void
DoEditorUi(renderer_2d *Ui, chunk_data *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      PushTableStart(Ui);
      DoEditorUi(Ui, &Element->Flags, CSz("chunk_flag Flags"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, &Element->Dim, CSz("v3i Dim"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, Element->Voxels, CSz("voxel Voxels"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);


      DoEditorUi(Ui, Element->VoxelLighting, CSz("voxel_lighting VoxelLighting"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushTableEnd(Ui);

      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S = (null)", Name));
    PushNewRow(Ui);
  }

}

