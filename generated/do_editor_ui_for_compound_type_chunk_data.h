// src/engine/editor.cpp:283:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, chunk_data *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle chunk_data", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
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

