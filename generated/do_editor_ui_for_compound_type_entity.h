link_internal void
DoEditorUi(renderer_2d *Ui, entity *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    /* PushTableStart(Ui); */
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      /* Padding.x += 20.f; */
      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Version, CSz("u64 Version"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Id, CSz("u64 Id"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->P, CSz("cp P"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      /* member.member(0, (union_member) { */
      /* DoEditorUi(Ui, &Element->(union_member.name), "union_member.type union_member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
      /* }) */
      DoEditorUi(Ui, &Element->EulerAngles, CSz("v4 EulerAngles"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Scale, CSz("r32 Scale"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      /* member.member(0, (union_member) { */
      /* DoEditorUi(Ui, &Element->(union_member.name), "union_member.type union_member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
      /* }) */
      DoEditorUi(Ui, &Element->_CollisionVolumeRadius, CSz("v3 _CollisionVolumeRadius"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Physics, CSz("physics Physics"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->AssetId, CSz("asset_id AssetId"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, Element->Emitter, CSz("particle_system Emitter"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->State, CSz("entity_state State"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Behavior, CSz("entity_behavior_flags Behavior"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->UserType, CSz("u64 UserType"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->UserData, CSz("u64 UserData"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




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

