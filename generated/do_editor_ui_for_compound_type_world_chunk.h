link_internal void
DoEditorUi(renderer_2d *Ui, world_chunk *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    // Select first member in union ..?




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Meshes, "lod_element_buffer Meshes", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->StandingSpots, "voxel_position_cursor StandingSpots", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    // Select first member in union ..?




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

