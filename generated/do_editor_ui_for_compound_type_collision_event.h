link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, collision_event *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle collision_event", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, Window, &Element->FrameIndex, CSz("u32 FrameIndex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->Count, CSz("u32 Count"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->MinP, CSz("cp MinP"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->MaxP, CSz("cp MaxP"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

