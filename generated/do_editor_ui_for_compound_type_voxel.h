link_internal void
DoEditorUi(renderer_2d *Ui, voxel *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    /* PushTableStart(Ui); */
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      /* Padding.x += 20.f; */
      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Flags, CSz("u8 Flags"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Transparency, CSz("u8 Transparency"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Color, CSz("u16 Color"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




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

