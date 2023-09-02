
link_internal engine_debug *
GetEngineDebug()
{
  return &Global_EngineResources->EngineDebug;
}

ui_debug *
GetUiDebug()
{
  return &Global_EngineResources->EngineDebug.UiDebug;
}


#define DebugValue(Ui, Value) DebugValue_(Ui, Value, STRINGIZE(Value))
#define DebugSlider(Ui, Value, Min, Max) DebugSlider_(Ui, Value, STRINGIZE(Value), Min, Max)

link_internal void
DebugValue_(renderer_2d *Ui, r32 *Value, const char* Name)
{
  PushColumn(Ui, CS(Name));
  PushColumn(Ui, CS(*Value));
  PushNewRow(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, v2 *Value, const char* Name)
{
  PushColumn(Ui, CS(Name));
  PushColumn(Ui, CS(Value->x));
  PushColumn(Ui, CS(Value->y));
  PushNewRow(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, b8 *Value, const char* Name)
{
  if (Button(Ui, CS(Name), (umm)Value + (umm)"toggle" )) { *Value = !(*Value); }

  counted_string Display = *Value ? CSz("True") : CSz("False");
  Text(Ui, Display);

  PushNewRow(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, s32 *Value, const char* Name)
{
  PushColumn(Ui, CS(Name));

  if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
  PushColumn(Ui, CS(*Value));
  if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
  PushNewRow(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, u32 *Value, const char* Name)
{
  PushColumn(Ui, CS(Name));

  if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
  PushColumn(Ui, CS(*Value));
  if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
  PushNewRow(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, u64 *Value, const char* Name)
{
  PushTableStart(Ui);
    PushColumn(Ui, CS(Name));
    PushColumn(Ui, CS(*Value));
    PushNewRow(Ui);
  PushTableEnd(Ui);
}


link_internal void
DebugSlider_(renderer_2d *Ui, r32 *Value, const char* Name, r32 Min, r32 Max)
{
  PushTableStart(Ui);
    PushColumn(Ui, CS(Name));
    PushColumn(Ui, CS(*Value));

    auto Range = Max-Min;
    r32 PercFilled = ((*Value)-Min)/Range;

    r32 Width = 125.f;
    interactable_handle BargraphButton = PushButtonStart(Ui, (umm)(umm("DebugSlider") ^ umm(Value)) );
      PushBargraph(Ui, PercFilled, V3(0.75f), V3(0.4f), Width);
    PushButtonEnd(Ui);

    v2 Offset = {};
    if (Pressed(Ui, &BargraphButton, &Offset))
    {
      r32 NewPerc = Clamp01(Offset.x / Width);
      r32 NewValue = (Range*NewPerc) + Min;
      *Value = NewValue;
    }

    PushNewRow(Ui);
  PushTableEnd(Ui);
}


link_internal void
DebugValue_(renderer_2d *Ui, world_position *Value, const char* Name)
{
  Text(Ui, CS(Name));
  PushNewRow(Ui);

  DebugValue(Ui, &Value->x);
  DebugValue(Ui, &Value->y);
  DebugValue(Ui, &Value->z);
}

link_internal void
DebugUi(engine_resources *Engine, cs Name, untextured_3d_geometry_buffer *Value)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Text(Ui, Name);
  if (Value)
  {
    PushColumn(Ui, CS("Verts : "));
    PushColumn(Ui, CS((u64)Value->Verts));
    PushNewRow(Ui);

    PushColumn(Ui, CS("Colors : "));
    PushColumn(Ui, CS((u64)Value->Colors));
    PushNewRow(Ui);

    PushColumn(Ui, CS("Normals : "));
    PushColumn(Ui, CS((u64)Value->Normals));
    PushNewRow(Ui);

    PushColumn(Ui, CS("End : "));
    PushColumn(Ui, CS(Value->End));
    PushNewRow(Ui);

    PushColumn(Ui, CS("At : "));
    PushColumn(Ui, CS(Value->At));
    PushNewRow(Ui);

    PushColumn(Ui, CS("Timestamp : "));
    PushColumn(Ui, CS(Value->Timestamp));
    PushNewRow(Ui);
  }
  else
  {
    Text(Ui, CSz(" 0"));
    PushNewRow(Ui);
  }
  PushNewRow(Ui);
}

link_internal void
DebugUi(engine_resources *Engine, cs Name, world_chunk *Value)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Text(Ui, Name);
  PushNewRow(Ui);

  world_position WorldP = Value->WorldP;
  DebugValue(Ui, &WorldP);

  if (WorldP != Value->WorldP)
  {
    EngineDebug->PickedChunk = GetWorldChunkFromHashtable(World, WorldP);
  }

  world_chunk *Chunk = EngineDebug->PickedChunk;

  if (Chunk)
  {
    auto Flags = Value->Flags;

    RangeIterator(MeshIndex, MeshIndex_Count)
    {
      world_chunk_mesh_bitfield Bit  = world_chunk_mesh_bitfield(1 << MeshIndex);
      untextured_3d_geometry_buffer *Mesh = TakeOwnershipSync(&Chunk->Meshes,  Bit);
      DebugUi(Engine, CSz("Mesh"), Mesh);
      ReleaseOwnership(&Chunk->Meshes, Bit, Mesh);
    }

    Text(Ui, CSz("Flags : "));
    Text(Ui, CS(Flags));
    Text(Ui, CSz(" ("));
    while (u32 Flag = UnsetLeastSignificantSetBit((u32*)&Flags))
    {
      Text(Ui, ToString((chunk_flag)Flag));
      if (Flags != 0) { Text(Ui, CSz("|")); }
    }
    Text(Ui, CSz(")"));
    PushNewRow(Ui);

    Text(Ui, CSz("FilledCount : "));
    Text(Ui, CS(Chunk->FilledCount));
    PushNewRow(Ui);

    Text(Ui, CSz("Picked : "));
    Text(Ui, CS(Chunk->Picked));
    PushNewRow(Ui);

    Text(Ui, CSz("DrawBoundingVoxels : "));
    Text(Ui, CS(Chunk->DrawBoundingVoxels));
    PushNewRow(Ui);


  }
}

link_internal void
DebugUi(engine_resources *Engine, cs Name, window_layout *Value)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Text(Ui, Name);
  Text(Ui, Value->Title);
}

link_internal void
DebugUi(engine_resources *Engine, cs Name, interactable *Value)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  DebugValue(Ui, &Value->ID);
  DebugValue(Ui, &Value->MinP);
  DebugValue(Ui, &Value->MaxP);
}



link_internal void
DoEngineDebugMenu(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  local_persist ui_element_toggle_button Buttons[] = {
    {CSz("Edit"),            False, False},
    {CSz("WorldChunks"),     False, False},
    {CSz("Textures"),        False, False},
    {CSz("RenderSettings"),  False, False},
    {CSz("EngineDebug"),     False, False},
  };

  ui_element_toggle_button_group ButtonGroup = {
    .Buttons = Buttons,
    .Count = ArrayCount(Buttons),
  };

  DrawToggleButtonGroup(Ui, &ButtonGroup);

  if (ToggledOn(&ButtonGroup, CSz("Edit")))
  {
    DoLevelEditor(Engine);
  }

  if (ToggledOn(&ButtonGroup, CSz("WorldChunks")))
  {
    v2 WindowDim = {{650.f, 950.f}};
    local_persist window_layout WorldChunkWindow = WindowLayout("World Chunks", AlignRightWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);
    PushWindowStart(Ui, &WorldChunkWindow);

      if ( Clicked(&ButtonGroup, CSz("WorldChunks")) ||
           Button(Ui, CSz("PickNewChunk"), (umm)"Pick"^(umm)"WorldChunks") )
      {
        EngineDebug->PickedChunkState = PickedChunkState_Hover;
      }
      if (Button(Ui, CSz("RebuildMesh"), (umm)"RebuildMesh"^(umm)"WorldChunks"))
      {
        QueueChunkForMeshRebuild(&Plat->LowPriority, EngineDebug->PickedChunk);
      }
      PushNewRow(Ui);

      if (EngineDebug->PickedChunkState == PickedChunkState_Hover)
      {
        EngineDebug->PickedChunk = Engine->MousedOverVoxel.PickedChunk.Chunk;
        if (Input->LMB.Clicked) { EngineDebug->PickedChunkState = PickedChunkState_None; }
      }

      if (EngineDebug->PickedChunk)
      {
        DebugUi(Engine, CSz("PickedChunk"), EngineDebug->PickedChunk );
      }

    PushWindowEnd(Ui, &WorldChunkWindow);
  }

#if 0
  if (ToggledOn(&ButtonGroup, CSz("Textures")))
  {
    local_persist window_layout TexturesWindow = WindowLayout("Textures", DefaultWindowBasis(*Ui->ScreenDim));
    PushWindowStart(Ui, &TexturesWindow);

    IterateOver(&EngineDebug->Textures, Texture, TextureIndex)
    {
      PushTexturedQuad(Ui, DebugTextureArraySlice_Font, V2(120), zDepth_Text);
    }

    PushWindowEnd(Ui, &TexturesWindow);
  }
#endif

  if (ToggledOn(&ButtonGroup, CSz("RenderSettings")))
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout RenderSettingsWindow = WindowLayout("Render Settings", DefaultWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &RenderSettingsWindow);

      PushTableStart(Ui);
        /* DebugValue(Ui, &Ui->Pressed.ID); */

        DebugValue(Ui, (b8*)&Settings->DoDayNightCycle);
        PushNewRow(Ui);

        DebugValue(Ui, (b8*)&Settings->UseSsao);
        DebugValue(Ui, (b8*)&Settings->UseShadowMapping);
        DebugValue(Ui, (b8*)&Settings->UseLightingBloom);

        // TODO(Jesse): Make a slider for time of day

        DebugSlider(Ui, &Graphics->Exposure, 0.0f, 5.f);

      PushTableEnd(Ui);

    PushWindowEnd(Ui, &RenderSettingsWindow);
  }

  if (ToggledOn(&ButtonGroup, CSz("EngineDebug")))
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout Window = WindowLayout("Engine Debug", DefaultWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &Window);
      PushTableStart(Ui);


        DebugValue(Ui, &EngineDebug->DrawEntityCollisionVolumes);
        DebugValue(Ui, &EngineDebug->DrawWorldAxies);

        DebugValue(Ui, &EngineDebug->UiDebug.OutlineUiValues);
        DebugValue(Ui, &EngineDebug->UiDebug.OutlineUiButtons);
        DebugValue(Ui, &EngineDebug->UiDebug.OutlineUiTables);


        DebugValue(Ui, &Engine->Editor.SelectedColorIndex);
        DebugUi(Engine, CSz("Selected Color"), &Engine->Editor.SelectedColorSquare);

      PushTableEnd(Ui);
    PushWindowEnd(Ui, &Window);
  }
}
