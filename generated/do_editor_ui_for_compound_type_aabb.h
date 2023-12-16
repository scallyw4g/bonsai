link_internal void
DoEditorUi(renderer_2d *Ui, aabb *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    /* member.member(0, (union_member) { */
    /* DoEditorUi(Ui, &Element->(union_member.name), "union_member.type union_member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
    /* }) */
    DoEditorUi(Ui, &Element->Min, "v3 Min", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    /* member.member(0, (union_member) { */
    /* DoEditorUi(Ui, &Element->(union_member.name), "union_member.type union_member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
    /* }) */
    DoEditorUi(Ui, &Element->Max, "v3 Max", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
  }
  else
  {
    PushNewRow(Ui);
  }
}

