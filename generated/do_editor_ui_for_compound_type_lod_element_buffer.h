link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lod_element_buffer *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lod_element_buffer", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, Window, &Element->MeshMask, CSz("u32 MeshMask"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      RangeIterator(ArrayIndex, MeshIndex_Count)
      {
        DoEditorUi(Ui, Window, Element->GpuBufferHandles+ArrayIndex, CSz("gpu_element_buffer_handles GpuBufferHandles"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        
      }



      
      RangeIterator(ArrayIndex, MeshIndex_Count)
      {
        DoEditorUi(Ui, Window, Element->E+ArrayIndex, CSz("geo_u3d_ptr E"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        
      }



      
      RangeIterator(ArrayIndex, MeshIndex_Count)
      {
        DoEditorUi(Ui, Window, Element->Locks+ArrayIndex, CSz("bonsai_futex Locks"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        
      }
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

