// src/engine/editor.cpp:351:0

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
        CSz("Next"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Flags,
        CSz("Flags"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Dim,
        CSz("Dim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Voxels,
        CSz("Voxels"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->VoxelLighting,
        CSz("VoxelLighting"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Meshes,
        CSz("Meshes"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->StandingSpots,
        CSz("StandingSpots"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WorldP,
        CSz("WorldP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->FilledCount,
        CSz("FilledCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DrawBoundingVoxels),
        CSz("DrawBoundingVoxels"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->PointsToLeaveRemaining,
        CSz("PointsToLeaveRemaining"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TriCount,
        CSz("TriCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->EdgeBoundaryVoxelCount,
        CSz("EdgeBoundaryVoxelCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DimX,
        CSz("DimX"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DimY,
        CSz("DimY"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DimZ,
        CSz("DimZ"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->_Pad0,
        CSz("_Pad0"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Entities,
        CSz("Entities"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v _Pad1[32]"), CSz("> _Pad1[32]"), UiId(Window, "toggle world_chunk u8 _Pad1", Element->_Pad1), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 32)
        {
          DoEditorUi(Ui, Window, Element->_Pad1+ArrayIndex, FSz("_Pad1[%d]", ArrayIndex), Params);
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

