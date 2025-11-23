// callsite
// src/engine/editor.cpp:316:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:501:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle_spawn_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x14DBD6B7);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle particle_spawn_type", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum ParticleSpawnType_None", Element, ThisHash), Params))
    {
            *Element = ParticleSpawnType_None;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Random"), UiId(Window, "enum ParticleSpawnType_Random", Element, ThisHash), Params))
    {
            *Element = ParticleSpawnType_Random;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Expanding"), UiId(Window, "enum ParticleSpawnType_Expanding", Element, ThisHash), Params))
    {
            *Element = ParticleSpawnType_Expanding;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Contracting"), UiId(Window, "enum ParticleSpawnType_Contracting", Element, ThisHash), Params))
    {
            *Element = ParticleSpawnType_Contracting;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
}


