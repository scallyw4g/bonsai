// examples/turn_based/game_types.h:68:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_game_data *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle entity_game_data", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, Window, &Element->FireballChargeLevel, CSz("u32 FireballChargeLevel"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->FireballCharges, CSz("u32 FireballCharges"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->IceBlockCharges, CSz("u32 IceBlockCharges"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->HoldingItem, CSz("u32 HoldingItem"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);
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

