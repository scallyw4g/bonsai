// src/engine/editor.cpp:231:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle_system *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle particle_system", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&(Element->Entropy),
        CSz("random_series Entropy"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->SpawnType),
        CSz("particle_spawn_type SpawnType"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->Drag),
        CSz("r32 Drag"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->Lifetime),
        CSz("r32 Lifetime"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->EmissionDelay),
        CSz("r32 EmissionDelay"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->EmissionLifespan),
        CSz("r32 EmissionLifespan"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ActiveParticles),
        CSz("u32 ActiveParticles"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->LifespanMod),
        CSz("r32 LifespanMod"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ParticleLifespan),
        CSz("r32 ParticleLifespan"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ParticlesPerSecond),
        CSz("r32 ParticlesPerSecond"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ParticleLightEmission),
        CSz("r32 ParticleLightEmission"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ParticleLightEmissionChance),
        CSz("r32 ParticleLightEmissionChance"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ParticleStartingTransparency),
        CSz("r32 ParticleStartingTransparency"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ParticleEndingTransparency),
        CSz("r32 ParticleEndingTransparency"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ParticleStartingDim),
        CSz("v3 ParticleStartingDim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->ParticleEndingDim),
        CSz("f32 ParticleEndingDim"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ParticleTurbMin),
        CSz("v3 ParticleTurbMin"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->ParticleTurbMax),
        CSz("v3 ParticleTurbMax"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->SpawnRegion),
        CSz("aabb SpawnRegion"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->SystemMovementCoefficient),
        CSz("r32 SystemMovementCoefficient"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->ElapsedSinceLastEmission),
        CSz("r32 ElapsedSinceLastEmission"),
        Params
        );





      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v u8 Colors[6]"), CSz("> u8 Colors[6]"), UiId(Window, "toggle particle_system u8 Colors", Element->Colors), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 6)
        {
          DoEditorUi(Ui, Window, Element->Colors+ArrayIndex, FSz("u8 Colors[%d]", ArrayIndex), Params);
 PushNewRow(Ui); 
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v particle Particles[(4096)]"), CSz("> particle Particles[(4096)]"), UiId(Window, "toggle particle_system particle Particles", Element->Particles), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (4096))
        {
          DoEditorUi(Ui, Window, Element->Particles+ArrayIndex, FSz("particle Particles[%d]", ArrayIndex), Params);
          
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
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

