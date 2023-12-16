link_internal void
DoEditorUi(renderer_2d *Ui, world_chunk *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Flags, "chunk_flag Flags", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    /* member.member(0, (union_member) { */
    /* DoEditorUi(Ui, &Element->(union_member.name), "union_member.type union_member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
    /* }) */
    DoEditorUi(Ui, &Element->Dim, "v3i Dim", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, Element->Voxels, "voxel Voxels", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, Element->VoxelLighting, "voxel_lighting VoxelLighting", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Meshes, "lod_element_buffer Meshes", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->StandingSpots, "voxel_position_cursor StandingSpots", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    /* member.member(0, (union_member) { */
    /* DoEditorUi(Ui, &Element->(union_member.name), "union_member.type union_member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
    /* }) */
    DoEditorUi(Ui, &Element->WorldP, "v3i WorldP", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->FilledCount, "u32 FilledCount", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->DrawBoundingVoxels, "b32 DrawBoundingVoxels", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->PointsToLeaveRemaining, "s32 PointsToLeaveRemaining", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->TriCount, "u32 TriCount", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->EdgeBoundaryVoxelCount, "s32 EdgeBoundaryVoxelCount", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->DimX, "u8 DimX", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->DimY, "u8 DimY", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->DimZ, "u8 DimZ", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->_Pad0, "u8 _Pad0", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, Element->Next, "world_chunk Next", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    RangeIterator(ArrayIndex, 40)
    {
      DoEditorUi(Ui, Element->_Pad1+ArrayIndex, "u8 _Pad1", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    }


    PushNewRow(Ui);
    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
  }
  else
  {
    PushNewRow(Ui);
  }
}

