link_internal void
DoEditorUi(renderer_2d *Ui, entity_ptr_block_array *Container, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), umm(Container) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      IterateOver(Container, Element, ElementIndex)
      {
        DoEditorUi(Ui, Element, CS(ElementIndex), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }
    }
  }
  else
  {
    PushColumn(Ui, FSz("%S = (null)", Name));
  }
}

