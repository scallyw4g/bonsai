// src/engine/editor.cpp:295:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, model *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle model", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->Vox,
        CSz("vox_data Vox"),
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
&Element->TransparentMesh,
        CSz("untextured_3d_geometry_buffer TransparentMesh"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Animation,
        CSz("animation Animation"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Dim,
        CSz("v3i Dim"),
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

