link_internal void
DoEditorUi(renderer_2d *Ui, animation *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    /* PushTableStart(Ui); */
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      /* Padding.x += 20.f; */
      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->t, CSz("r32 t"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->tEnd, CSz("r32 tEnd"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->xKeyframeCount, CSz("u32 xKeyframeCount"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, Element->xKeyframes, CSz("keyframe xKeyframes"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->yKeyframeCount, CSz("u32 yKeyframeCount"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, Element->yKeyframes, CSz("keyframe yKeyframes"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->zKeyframeCount, CSz("u32 zKeyframeCount"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, Element->zKeyframes, CSz("keyframe zKeyframes"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      PushNewRow(Ui);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
    }
    else
    {
      PushNewRow(Ui);
    }
  }
  else
  {
    PushColumn(Ui, FSz("%S = (null)", Name));
    PushNewRow(Ui);
  }
  
}

