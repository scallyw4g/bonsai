link_internal void
DoEditorUi(renderer_2d *Ui, animation *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->t, "r32 t", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->tEnd, "r32 tEnd", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->xKeyframeCount, "u32 xKeyframeCount", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, Element->xKeyframes, "keyframe xKeyframes", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->yKeyframeCount, "u32 yKeyframeCount", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, Element->yKeyframes, "keyframe yKeyframes", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->zKeyframeCount, "u32 zKeyframeCount", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, Element->zKeyframes, "keyframe zKeyframes", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
  }
  else
  {
    PushNewRow(Ui);
  }
}

