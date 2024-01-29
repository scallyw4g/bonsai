// src/engine/editor.cpp:0:182

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle_system *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle particle_system", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, Window, &Element->Entropy, CSz("random_series Entropy"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->SpawnType, CSz("particle_spawn_type SpawnType"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->Drag, CSz("r32 Drag"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->Lifetime, CSz("r32 Lifetime"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->EmissionDelay, CSz("r32 EmissionDelay"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->EmissionLifespan, CSz("r32 EmissionLifespan"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ActiveParticles, CSz("u32 ActiveParticles"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->LifespanMod, CSz("r32 LifespanMod"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ParticleLifespan, CSz("r32 ParticleLifespan"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ParticlesPerSecond, CSz("r32 ParticlesPerSecond"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ParticleLightEmission, CSz("r32 ParticleLightEmission"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ParticleLightEmissionChance, CSz("r32 ParticleLightEmissionChance"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ParticleStartingTransparency, CSz("r32 ParticleStartingTransparency"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ParticleEndingTransparency, CSz("r32 ParticleEndingTransparency"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ParticleStartingDim, CSz("v3 ParticleStartingDim"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->ParticleEndingDim, CSz("f32 ParticleEndingDim"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ParticleTurbMin, CSz("v3 ParticleTurbMin"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->ParticleTurbMax, CSz("v3 ParticleTurbMax"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->SpawnRegion, CSz("aabb SpawnRegion"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->SystemMovementCoefficient, CSz("r32 SystemMovementCoefficient"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->ElapsedSinceLastEmission, CSz("r32 ElapsedSinceLastEmission"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v u8 Colors[6]"), CSz("> u8 Colors[6]"), UiId(Window, "toggle particle_system u8 Colors", Element->Colors), EDITOR_UI_FUNCTION_INSTANCE_NAMES ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 6)
        {
          DoEditorUi(Ui, Window, Element->Colors+ArrayIndex, FSz("u8 Colors[%d]", ArrayIndex), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
 PushNewRow(Ui); 
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v particle Particles[(4096)]"), CSz("> particle Particles[(4096)]"), UiId(Window, "toggle particle_system particle Particles", Element->Particles), EDITOR_UI_FUNCTION_INSTANCE_NAMES ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (4096))
        {
          DoEditorUi(Ui, Window, Element->Particles+ArrayIndex, FSz("particle Particles[%d]", ArrayIndex), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
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

