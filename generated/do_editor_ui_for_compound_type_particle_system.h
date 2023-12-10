link_internal void
DoEditorUi(renderer_2d *Ui, particle_system *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Entropy, "random_series Entropy", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->SpawnType, "particle_spawn_type SpawnType", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Drag, "r32 Drag", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Lifetime, "r32 Lifetime", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->EmissionDelay, "r32 EmissionDelay", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->EmissionLifespan, "r32 EmissionLifespan", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ActiveParticles, "u32 ActiveParticles", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->LifespanMod, "r32 LifespanMod", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ParticleLifespan, "r32 ParticleLifespan", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ParticlesPerSecond, "r32 ParticlesPerSecond", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ParticleLightEmission, "r32 ParticleLightEmission", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ParticleLightEmissionChance, "r32 ParticleLightEmissionChance", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ParticleStartingTransparency, "r32 ParticleStartingTransparency", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ParticleEndingTransparency, "r32 ParticleEndingTransparency", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    // Select first member in union ..?




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ParticleEndingDim, "f32 ParticleEndingDim", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    // Select first member in union ..?




    PushNewRow(Ui);
    // Select first member in union ..?




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->SpawnRegion, "aabb SpawnRegion", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->SystemMovementCoefficient, "r32 SystemMovementCoefficient", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ElapsedSinceLastEmission, "r32 ElapsedSinceLastEmission", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    RangeIterator(ArrayIndex, 6)
    {
      DoEditorUi(Ui, Element->Colors+ArrayIndex, "u8 Colors", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    }


    PushNewRow(Ui);
    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
  }
  else
  {
    PushNewRow(Ui);
  }
}

