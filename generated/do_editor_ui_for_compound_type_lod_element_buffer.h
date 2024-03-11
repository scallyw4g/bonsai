// src/engine/editor.cpp:222:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lod_element_buffer *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lod_element_buffer", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->MeshMask,
        CSz("MeshMask"),
        Params
        );





      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v GpuBufferHandles[MeshIndex_Count]"), CSz("> GpuBufferHandles[MeshIndex_Count]"), UiId(Window, "toggle lod_element_buffer gpu_element_buffer_handles GpuBufferHandles", Element->GpuBufferHandles), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, MeshIndex_Count)
        {
          DoEditorUi(Ui, Window, Element->GpuBufferHandles+ArrayIndex, FSz("GpuBufferHandles[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      if (ToggleButton(Ui, CSz("v E[MeshIndex_Count]"), CSz("> E[MeshIndex_Count]"), UiId(Window, "toggle lod_element_buffer geo_u3d_ptr E", Element->E), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, MeshIndex_Count)
        {
          DoEditorUi(Ui, Window, Element->E+ArrayIndex, FSz("E[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      if (ToggleButton(Ui, CSz("v Locks[MeshIndex_Count]"), CSz("> Locks[MeshIndex_Count]"), UiId(Window, "toggle lod_element_buffer bonsai_futex Locks", Element->Locks), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, MeshIndex_Count)
        {
          DoEditorUi(Ui, Window, Element->Locks+ArrayIndex, FSz("Locks[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
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

