link_internal void
DoEditorUi(renderer_2d *Ui, ui_debug *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->OutlineUiValues, "b8 OutlineUiValues", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->OutlineUiButtons, "b8 OutlineUiButtons", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->OutlineUiTables, "b8 OutlineUiTables", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->DebugBreakOnClick, "b8 DebugBreakOnClick", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
  }
  else
  {
    PushNewRow(Ui);
  }
}

