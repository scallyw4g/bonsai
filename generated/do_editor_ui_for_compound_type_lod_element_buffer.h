link_internal void
DoEditorUi(renderer_2d *Ui, lod_element_buffer *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    /* PushTableStart(Ui); */
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      /* Padding.x += 20.f; */
      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->MeshMask, CSz("u32 MeshMask"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      RangeIterator(ArrayIndex, MeshIndex_Count)
      {
        DoEditorUi(Ui, Element->GpuBufferHandles+ArrayIndex, CSz("gpu_element_buffer_handles GpuBufferHandles"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }


      PushNewRow(Ui);
      RangeIterator(ArrayIndex, MeshIndex_Count)
      {
        DoEditorUi(Ui, Element->E+ArrayIndex, CSz("geo_u3d_ptr E"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }


      PushNewRow(Ui);
      RangeIterator(ArrayIndex, MeshIndex_Count)
      {
        DoEditorUi(Ui, Element->Locks+ArrayIndex, CSz("bonsai_futex Locks"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }


      PushNewRow(Ui);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
    }
    else
    {
      PushNewRow(Ui);
    }
  }
  else
  {
    PushColumn(Ui, FSz("%S = (null)", Name));
    PushNewRow(Ui);
  }
  
}

