link_internal void
DoEditorUi(renderer_2d *Ui, gpu_element_buffer_handles *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    /* PushTableStart(Ui); */
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      /* Padding.x += 20.f; */
      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->VertexHandle, CSz("u32 VertexHandle"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->NormalHandle, CSz("u32 NormalHandle"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->MatHandle, CSz("u32 MatHandle"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ElementCount, CSz("u32 ElementCount"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




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

