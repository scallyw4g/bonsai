link_internal void
DoEditorUi(renderer_2d *Ui, animation *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, CS(Name), CS(Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->t, "r32 t", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->tEnd, "r32 tEnd", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->xKeyframeCount, "u32 xKeyframeCount", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->xKeyframes, "keyframe xKeyframes", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->yKeyframeCount, "u32 yKeyframeCount", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->yKeyframes, "keyframe yKeyframes", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->zKeyframeCount, "u32 zKeyframeCount", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    DoEditorUi(Ui, &Element->zKeyframes, "keyframe zKeyframes", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  }
  else
  {
    PushNewRow(Ui);
  }
  /* PushTableEnd(Ui); */
}

