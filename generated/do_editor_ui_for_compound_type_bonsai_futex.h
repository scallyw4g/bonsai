link_internal void
DoEditorUi(renderer_2d *Ui, bonsai_futex *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      PushTableStart(Ui);
      DoEditorUi(Ui, &Element->SignalValue, CSz("u32 SignalValue"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      /* PushNewRow(Ui); */



      DoEditorUi(Ui, &Element->ThreadsWaiting, CSz("u32 ThreadsWaiting"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      /* PushNewRow(Ui); */



      DoEditorUi(Ui, &Element->Initialized, CSz("b32 Initialized"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      /* PushNewRow(Ui); */
      PushTableEnd(Ui);

      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S = (null)", Name));
    PushNewRow(Ui);
  }

}

