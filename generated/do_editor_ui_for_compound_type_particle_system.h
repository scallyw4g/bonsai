// src/engine/editor.cpp:246:0

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
&Element->Entropy,
        CSz("Entropy"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SpawnType,
        CSz("SpawnType"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Drag,
        CSz("Drag"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Lifetime,
        CSz("Lifetime"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->EmissionDelay,
        CSz("EmissionDelay"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->EmissionLifespan,
        CSz("EmissionLifespan"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ActiveParticles,
        CSz("ActiveParticles"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->LifespanMod,
        CSz("LifespanMod"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ParticleLifespan,
        CSz("ParticleLifespan"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ParticlesPerSecond,
        CSz("ParticlesPerSecond"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ParticleLightEmission,
        CSz("ParticleLightEmission"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ParticleLightEmissionChance,
        CSz("ParticleLightEmissionChance"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ParticleStartingTransparency,
        CSz("ParticleStartingTransparency"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ParticleEndingTransparency,
        CSz("ParticleEndingTransparency"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ParticleStartingDim,
        CSz("ParticleStartingDim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ParticleEndingDim,
        CSz("ParticleEndingDim"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ParticleTurbMin,
        CSz("ParticleTurbMin"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ParticleTurbMax,
        CSz("ParticleTurbMax"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SpawnRegion,
        CSz("SpawnRegion"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SystemMovementCoefficient,
        CSz("SystemMovementCoefficient"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ElapsedSinceLastEmission,
        CSz("ElapsedSinceLastEmission"),
        Params
        );





      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v Colors[6]"), CSz("> Colors[6]"), UiId(Window, "toggle particle_system u8 Colors", Element->Colors), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 6)
        {
          DoEditorUi(Ui, Window, Element->Colors+ArrayIndex, FSz("Colors[%d]", ArrayIndex), Params);
 PushNewRow(Ui); 
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v Particles[(4096)]"), CSz("> Particles[(4096)]"), UiId(Window, "toggle particle_system particle Particles", Element->Particles), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (4096))
        {
          DoEditorUi(Ui, Window, Element->Particles+ArrayIndex, FSz("Particles[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }
    else
    {
      PushNewRow(Ui);
    }

  }
  else
  {
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

