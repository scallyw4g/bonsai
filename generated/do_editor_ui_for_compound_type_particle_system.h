link_internal void
DoEditorUi(renderer_2d *Ui, particle_system *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    /* PushTableStart(Ui); */
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      /* Padding.x += 20.f; */
      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Entropy, CSz("random_series Entropy"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->SpawnType, CSz("particle_spawn_type SpawnType"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Drag, CSz("r32 Drag"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Lifetime, CSz("r32 Lifetime"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->EmissionDelay, CSz("r32 EmissionDelay"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->EmissionLifespan, CSz("r32 EmissionLifespan"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ActiveParticles, CSz("u32 ActiveParticles"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->LifespanMod, CSz("r32 LifespanMod"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ParticleLifespan, CSz("r32 ParticleLifespan"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ParticlesPerSecond, CSz("r32 ParticlesPerSecond"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ParticleLightEmission, CSz("r32 ParticleLightEmission"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ParticleLightEmissionChance, CSz("r32 ParticleLightEmissionChance"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ParticleStartingTransparency, CSz("r32 ParticleStartingTransparency"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ParticleEndingTransparency, CSz("r32 ParticleEndingTransparency"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      /* member.member(0, (union_member) { */
      /* DoEditorUi(Ui, &Element->(union_member.name), "union_member.type union_member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
      /* }) */
      DoEditorUi(Ui, &Element->ParticleStartingDim, CSz("v3 ParticleStartingDim"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ParticleEndingDim, CSz("f32 ParticleEndingDim"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      /* member.member(0, (union_member) { */
      /* DoEditorUi(Ui, &Element->(union_member.name), "union_member.type union_member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
      /* }) */
      DoEditorUi(Ui, &Element->ParticleTurbMin, CSz("v3 ParticleTurbMin"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      /* member.member(0, (union_member) { */
      /* DoEditorUi(Ui, &Element->(union_member.name), "union_member.type union_member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
      /* }) */
      DoEditorUi(Ui, &Element->ParticleTurbMax, CSz("v3 ParticleTurbMax"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->SpawnRegion, CSz("aabb SpawnRegion"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->SystemMovementCoefficient, CSz("r32 SystemMovementCoefficient"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ElapsedSinceLastEmission, CSz("r32 ElapsedSinceLastEmission"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      RangeIterator(ArrayIndex, 6)
      {
        DoEditorUi(Ui, Element->Colors+ArrayIndex, CSz("u8 Colors"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }


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
  }
  
}

