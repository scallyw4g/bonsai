link_internal void
DoEditorUi(renderer_2d *Ui, lod_element_buffer *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->MeshMask, "u32 MeshMask", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    RangeIterator(ArrayIndex, MeshIndex_Count)
    {
      DoEditorUi(Ui, Element->GpuBufferHandles+ArrayIndex, "gpu_element_buffer_handles GpuBufferHandles", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    }


    PushNewRow(Ui);
    RangeIterator(ArrayIndex, MeshIndex_Count)
    {
      DoEditorUi(Ui, Element->E+ArrayIndex, "geo_u3d E", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    }


    PushNewRow(Ui);
    RangeIterator(ArrayIndex, MeshIndex_Count)
    {
      DoEditorUi(Ui, Element->Locks+ArrayIndex, "bonsai_futex Locks", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    }


    PushNewRow(Ui);
    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
  }
  else
  {
    PushNewRow(Ui);
  }
}

