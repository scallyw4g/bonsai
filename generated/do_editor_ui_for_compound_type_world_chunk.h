// src/engine/editor.cpp:352:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_chunk *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_chunk", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
        Element->Next,
        CSz("world_chunk Next"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Flags,
        CSz("chunk_flag Flags"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Dim,
        CSz("v3i Dim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Voxels,
        CSz("voxel Voxels"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->VoxelLighting,
        CSz("voxel_lighting VoxelLighting"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Meshes,
        CSz("lod_element_buffer Meshes"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->StandingSpots,
        CSz("voxel_position_cursor StandingSpots"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WorldP,
        CSz("v3i WorldP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->FilledCount,
        CSz("u32 FilledCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DrawBoundingVoxels),
        CSz("b32 DrawBoundingVoxels"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->PointsToLeaveRemaining,
        CSz("s32 PointsToLeaveRemaining"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TriCount,
        CSz("u32 TriCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->EdgeBoundaryVoxelCount,
        CSz("s32 EdgeBoundaryVoxelCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DimX,
        CSz("u8 DimX"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DimY,
        CSz("u8 DimY"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DimZ,
        CSz("u8 DimZ"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->_Pad0,
        CSz("u8 _Pad0"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Entities,
        CSz("entity_ptr_block_array Entities"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v u8 _Pad1[32]"), CSz("> u8 _Pad1[32]"), UiId(Window, "toggle world_chunk u8 _Pad1", Element->_Pad1), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 32)
        {
          DoEditorUi(Ui, Window, Element->_Pad1+ArrayIndex, FSz("u8 _Pad1[%d]", ArrayIndex), Params);
 PushNewRow(Ui); 
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      PushNewRow(Ui);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }
    else
    {
      PushNewRow(Ui);
    }

  }
  else
  {
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

