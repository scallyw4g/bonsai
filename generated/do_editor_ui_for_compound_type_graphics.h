// src/engine/editor.cpp:414:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, graphics *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle graphics", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&(Element->Settings),
        CSz("render_settings Settings"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->GameCamera),
        CSz("camera GameCamera"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->DebugCamera),
        CSz("camera DebugCamera"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->Exposure),
        CSz("r32 Exposure"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        (Element->gBuffer),
        CSz("g_buffer_render_group gBuffer"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        (Element->AoGroup),
        CSz("ao_render_group AoGroup"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        (Element->SG),
        CSz("shadow_render_group SG"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->Transparency),
        CSz("transparency_render_group Transparency"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->Lighting),
        CSz("lighting_render_group Lighting"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->Gaussian),
        CSz("gaussian_render_group Gaussian"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->CompositeGroup),
        CSz("composite_render_group CompositeGroup"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v gpu_mapped_element_buffer GpuBuffers[2]"), CSz("> gpu_mapped_element_buffer GpuBuffers[2]"), UiId(Window, "toggle graphics gpu_mapped_element_buffer GpuBuffers", Element->GpuBuffers), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 2)
        {
          DoEditorUi(Ui, Window, Element->GpuBuffers+ArrayIndex, FSz("gpu_mapped_element_buffer GpuBuffers[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
&(Element->GpuBufferWriteIndex),
        CSz("u32 GpuBufferWriteIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        (Element->Memory),
        CSz("memory_arena Memory"),
        Params
        );
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

