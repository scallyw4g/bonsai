link_internal void
DoEditorUi(renderer_2d *Ui, entity *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, "toggle entity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, &Element->Version, CSz("u64 Version"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->Id, CSz("entity_id Id"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
DoEditorUi_entity_P(Ui, Element, CSz("cp P"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      
      DoEditorUi(Ui, &Element->EulerAngles, CSz("v3 EulerAngles"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,-180.f, 180.f);





      
      DoEditorUi(Ui, &Element->Scale, CSz("r32 Scale"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->_CollisionVolumeRadius, CSz("v3 _CollisionVolumeRadius"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->Physics, CSz("physics Physics"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->AssetId, CSz("asset_id AssetId"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->ModelIndex, CSz("u64 ModelIndex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->LastResolvedCollision, CSz("collision_event LastResolvedCollision"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->LastResolvedPosInfo, CSz("entity_position_info LastResolvedPosInfo"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Element->Emitter, CSz("particle_system Emitter"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, &Element->State, CSz("entity_state State"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->Behavior, CSz("entity_behavior_flags Behavior"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->UserType, CSz("u64 UserType"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->UserData, CSz("u64 UserData"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

