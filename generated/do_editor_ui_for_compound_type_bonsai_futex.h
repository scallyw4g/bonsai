link_internal void
DoEditorUi(renderer_2d *Ui, bonsai_futex *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    /* PushTableStart(Ui); */
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      /* Padding.x += 20.f; */
      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->SignalValue, CSz("u32 SignalValue"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ThreadsWaiting, CSz("u32 ThreadsWaiting"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Initialized, CSz("b32 Initialized"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




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

