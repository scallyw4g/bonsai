link_internal void
DoEditorUi(renderer_2d *Ui, entity *Element, const char* Name)
{
  PushTableStart(Ui);
  DoEditorUi(Ui, &Element->Model, "model Model");
  DoEditorUi(Ui, &Element->CollisionVolumeRadius, "v3 CollisionVolumeRadius");
  DoEditorUi(Ui, &Element->Emitter, "particle_system Emitter");
  DoEditorUi(Ui, &Element->Physics, "physics Physics");
  DoEditorUi(Ui, &Element->P, "canonical_position P");
  DoEditorUi(Ui, &Element->Rotation, "Quaternion Rotation");
  DoEditorUi(Ui, &Element->State, "entity_state State");
  DoEditorUi(Ui, &Element->Type, "entity_type Type");
  DoEditorUi(Ui, &Element->Scale, "r32 Scale");
  DoEditorUi(Ui, &Element->RateOfFire, "r32 RateOfFire");
  DoEditorUi(Ui, &Element->FireCooldown, "r32 FireCooldown");
  DoEditorUi(Ui, &Element->Health, "s32 Health");
  DoEditorUi(Ui, &Element->UserData, "void  UserData");
  PushTableEnd(Ui);
}

