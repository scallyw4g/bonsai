link_internal void
DoEditorUi(renderer_2d *Ui, graphics *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, "toggle graphics"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, &Element->Settings, CSz("render_settings Settings"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->GameCamera, CSz("camera GameCamera"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->DebugCamera, CSz("camera DebugCamera"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Element->Camera, CSz("camera Camera"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, &Element->Exposure, CSz("r32 Exposure"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Element->gBuffer, CSz("g_buffer_render_group gBuffer"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Element->AoGroup, CSz("ao_render_group AoGroup"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Element->SG, CSz("shadow_render_group SG"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, &Element->Transparency, CSz("transparency_render_group Transparency"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->Lighting, CSz("lighting_render_group Lighting"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->Gaussian, CSz("gaussian_render_group Gaussian"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->CompositeGroup, CSz("composite_render_group CompositeGroup"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      RangeIterator(ArrayIndex, 2)
      {
        DoEditorUi(Ui, Element->GpuBuffers+ArrayIndex, CSz("gpu_mapped_element_buffer GpuBuffers"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        
      }



      
      DoEditorUi(Ui, &Element->GpuBufferWriteIndex, CSz("u32 GpuBufferWriteIndex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Element->Memory, CSz("memory_arena Memory"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

