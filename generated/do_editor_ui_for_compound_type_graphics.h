// src/engine/editor.cpp:333:0

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
&Element->Settings,
        CSz("Settings"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->GameCamera,
        CSz("GameCamera"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->DebugCamera,
        CSz("DebugCamera"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Exposure,
        CSz("Exposure"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->gBuffer,
        CSz("gBuffer"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->AoGroup,
        CSz("AoGroup"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->SG,
        CSz("SG"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Transparency,
        CSz("Transparency"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Lighting,
        CSz("Lighting"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Gaussian,
        CSz("Gaussian"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->CompositeGroup,
        CSz("CompositeGroup"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v GpuBuffers[2]"), CSz("> GpuBuffers[2]"), UiId(Window, "toggle graphics gpu_mapped_element_buffer GpuBuffers", Element->GpuBuffers), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 2)
        {
          DoEditorUi(Ui, Window, Element->GpuBuffers+ArrayIndex, FSz("GpuBuffers[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
&Element->GpuBufferWriteIndex,
        CSz("GpuBufferWriteIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->Memory,
        CSz("Memory"),
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

