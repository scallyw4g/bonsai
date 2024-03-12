// src/engine/editor.cpp:213:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, chunk_data *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle chunk_data", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
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
      CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
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

