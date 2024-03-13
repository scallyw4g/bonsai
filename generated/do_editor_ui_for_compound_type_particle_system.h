// src/engine/editor.cpp:189:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle_system *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle particle_system", Element), &DefaultUiRenderParams_Generic))
      {
        DidToggle = True;
        PushNewRow(Ui);
      }
      else
      {
        DrawChildren = False;
      }
    }

    if (DrawChildren)
    {
      PushTableStart(Ui);
      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
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
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 6)
        {
          DoEditorUi(Ui, Window, Element->Colors+ArrayIndex, FSz("Colors[%d]", ArrayIndex), Params);
 PushNewRow(Ui); 
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v Particles[(4096)]"), CSz("> Particles[(4096)]"), UiId(Window, "toggle particle_system particle Particles", Element->Particles), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (4096))
        {
          DoEditorUi(Ui, Window, Element->Particles+ArrayIndex, FSz("Particles[%d]", ArrayIndex), Params);
          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);
      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
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

