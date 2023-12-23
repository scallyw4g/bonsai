link_internal void
DoEditorUi(renderer_2d *Ui, render_debug *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->BytesSolidGeoLastFrame, "u32 BytesSolidGeoLastFrame", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->BytesTransGeoLastFrame, "u32 BytesTransGeoLastFrame", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->DrawCallsLastFrame, "u32 DrawCallsLastFrame", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
  }
  else
  {
    PushNewRow(Ui);
  }
}

