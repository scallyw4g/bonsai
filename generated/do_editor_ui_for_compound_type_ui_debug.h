link_internal void
DoEditorUi(renderer_2d *Ui, ui_debug *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, &Element->OutlineUiValues, CSz("b8 OutlineUiValues"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushNewRow(Ui);



      DoEditorUi(Ui, &Element->OutlineUiButtons, CSz("b8 OutlineUiButtons"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushNewRow(Ui);



      DoEditorUi(Ui, &Element->OutlineUiTables, CSz("b8 OutlineUiTables"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushNewRow(Ui);



      DoEditorUi(Ui, &Element->OutlineUiTableColumns, CSz("b8 OutlineUiTableColumns"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushNewRow(Ui);



      DoEditorUi(Ui, &Element->DebugBreakOnClick, CSz("b8 DebugBreakOnClick"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushNewRow(Ui);



      DoEditorUi(Ui, &Element->DebugBreakUiCommand, CSz("b8 DebugBreakUiCommand"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushNewRow(Ui);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S = (null)", Name));
    PushNewRow(Ui);
  }

}

