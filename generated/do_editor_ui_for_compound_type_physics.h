link_internal void
DoEditorUi(renderer_2d *Ui, physics *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, CS(Name), CS(Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Velocity, "v3 Velocity", EDITOR_UI_FUNCTION_INSTANCE_NAMES);


    DoEditorUi(Ui, &Element->Force, "v3 Force", EDITOR_UI_FUNCTION_INSTANCE_NAMES);


    DoEditorUi(Ui, &Element->Delta, "v3 Delta", EDITOR_UI_FUNCTION_INSTANCE_NAMES);


    DoEditorUi(Ui, &Element->Mass, "r32 Mass", EDITOR_UI_FUNCTION_INSTANCE_NAMES);


    DoEditorUi(Ui, &Element->Speed, "r32 Speed", EDITOR_UI_FUNCTION_INSTANCE_NAMES);


  }
  else
  {
    PushNewRow(Ui);
  }
  /* PushTableEnd(Ui); */
}

