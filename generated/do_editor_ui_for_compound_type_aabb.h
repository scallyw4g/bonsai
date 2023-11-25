link_internal void
DoEditorUi(renderer_2d *Ui, aabb *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Min, "v3 Min", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Max, "v3 Max", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

