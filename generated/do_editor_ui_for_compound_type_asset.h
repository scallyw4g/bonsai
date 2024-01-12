link_internal void
DoEditorUi(renderer_2d *Ui, asset *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      PushTableStart(Ui);
      DoEditorUi(Ui, &Element->LoadState, CSz("asset_load_state LoadState"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, &Element->Id, CSz("asset_id Id"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      DoEditorUi(Ui, &Element->LRUFrameIndex, CSz("u64 LRUFrameIndex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      /* PushNewRow(Ui); */



      DoEditorUi(Ui, &Element->Models, CSz("model_buffer Models"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

