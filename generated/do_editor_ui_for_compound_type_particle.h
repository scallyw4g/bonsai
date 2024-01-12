link_internal void
DoEditorUi(renderer_2d *Ui, particle *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      PushTableStart(Ui);
      DoEditorUi(Ui, &Element->Velocity, CSz("v3 Velocity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, &Element->Offset, CSz("v3 Offset"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, &Element->Color, CSz("u8 Color"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      /* PushNewRow(Ui); */



      DoEditorUi(Ui, &Element->IsLight, CSz("b8 IsLight"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      /* PushNewRow(Ui); */



      DoEditorUi(Ui, &Element->RemainingLifespan, CSz("r32 RemainingLifespan"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

