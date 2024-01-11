link_internal void
DoEditorUi(renderer_2d *Ui, world_chunk *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, &Element->Flags, CSz("chunk_flag Flags"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Dim, CSz("v3i Dim"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, Element->Voxels, CSz("voxel Voxels"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      PushNewRow(Ui);
      DoEditorUi(Ui, Element->VoxelLighting, CSz("voxel_lighting VoxelLighting"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Meshes, CSz("lod_element_buffer Meshes"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->StandingSpots, CSz("voxel_position_cursor StandingSpots"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->WorldP, CSz("v3i WorldP"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->FilledCount, CSz("u32 FilledCount"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->DrawBoundingVoxels, CSz("b32 DrawBoundingVoxels"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->PointsToLeaveRemaining, CSz("s32 PointsToLeaveRemaining"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->TriCount, CSz("u32 TriCount"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->EdgeBoundaryVoxelCount, CSz("s32 EdgeBoundaryVoxelCount"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->DimX, CSz("u8 DimX"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->DimY, CSz("u8 DimY"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->DimZ, CSz("u8 DimZ"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->_Pad0, CSz("u8 _Pad0"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, Element->Next, CSz("world_chunk Next"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Entities, CSz("entity_ptr_block_array Entities"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      RangeIterator(ArrayIndex, 32)
      {
        DoEditorUi(Ui, Element->_Pad1+ArrayIndex, CSz("u8 _Pad1"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }


      PushNewRow(Ui);

      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
    }
    else
    {
      PushNewRow(Ui);
    }
  }
  else
  {
    PushColumn(Ui, FSz("%S = (null)", Name));
    PushNewRow(Ui);
  }

}

