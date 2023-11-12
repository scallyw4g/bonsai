link_internal void
DoEditorUi(renderer_2d *Ui, aabb *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, CS(Name), CS(Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Center, "v3 Center", EDITOR_UI_FUNCTION_INSTANCE_NAMES);


    DoEditorUi(Ui, &Element->Radius, "v3 Radius", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



  }
  else
  {
    PushNewRow(Ui);
  }
  /* PushTableEnd(Ui); */
}

