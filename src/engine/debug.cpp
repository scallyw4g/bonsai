link_internal engine_debug *
GetEngineDebug()
{
  Assert(Global_EngineResources);
  return &Global_EngineResources->EngineDebug;
}

link_weak ui_debug *
GetUiDebug()
{
  Assert(Global_EngineResources);
  return &Global_EngineResources->EngineDebug.UiDebug;
}





/* #define DoEditorUi(Ui, Value, "Value") DoEditorUi(Ui, Value, STRINGIZE(Value)) */
#define DebugSlider(Ui, Value, Min, Max) DebugSlider_(Ui, Value, STRINGIZE(Value), Min, Max)

link_internal void
DoEditorUi(renderer_2d *Ui, void *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushColumn(Ui, FSz("0x%x",umm(Value)), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushNewRow(Ui);
}

link_internal void
DoEditorUi(renderer_2d *Ui, r32 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushNewRow(Ui);
}

link_internal void
DoEditorUi(renderer_2d *Ui, v3 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushColumn(Ui, FSz("%.2f %.2f %.2f", double(Value->x), double(Value->y), double(Value->z)), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushNewRow(Ui);
}


link_internal void
DoEditorUi(renderer_2d *Ui, v2 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name),     EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushColumn(Ui, CS(Value->x), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushColumn(Ui, CS(Value->y), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushNewRow(Ui);
}

link_internal void
DoEditorUi(renderer_2d *Ui, b8 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Button(Ui, CS(Name), (umm)Value + (umm)"toggle", EDITOR_UI_FUNCTION_INSTANCE_NAMES )) { *Value = !(*Value); }

  counted_string Display = *Value ? CSz("True") : CSz("False");
  Text(Ui, Display);

  PushNewRow(Ui);
}

link_internal void
DoEditorUi(renderer_2d *Ui, s32 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);

  if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
  PushColumn(Ui, CS(*Value));
  if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
  PushNewRow(Ui);
}

link_internal void
DoEditorUi(renderer_2d *Ui, u32 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);

  if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
  PushColumn(Ui, CS(*Value));
  if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
  PushNewRow(Ui);
}

link_internal void
DoEditorUi(renderer_2d *Ui, u64 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
    PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  /* PushTableEnd(Ui); */
}






link_internal void
DoEditorUi(renderer_2d *Ui, v3i *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, &Value->x, "x");
  DoEditorUi(Ui, &Value->y, "y");
  DoEditorUi(Ui, &Value->z, "z");
  PushNewRow(Ui);
}

link_internal void
DoEditorUi(renderer_2d *Ui, cp *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, &Value->WorldP, "WorldP");
  DoEditorUi(Ui, &Value->Offset, "Offset");
  PushNewRow(Ui);
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



poof(do_editor_ui_for_compound_type(physics))
#include <generated/do_editor_ui_for_compound_type_physics.h>

poof(do_editor_ui_for_compound_type(animation))
#include <generated/do_editor_ui_for_compound_type_animation.h>

poof(do_editor_ui_for_compound_type(untextured_3d_geometry_buffer))
#include <generated/do_editor_ui_for_compound_type_untextured_3d_geometry_buffer.h>

poof(do_editor_ui_for_compound_type(model))
#include <generated/do_editor_ui_for_compound_type_model.h>

poof(do_editor_ui_for_compound_type(entity))
#include <generated/do_editor_ui_for_compound_type_entity.h>



link_internal void
DebugUi(engine_resources *Engine, cs Name, untextured_3d_geometry_buffer *Value)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  /* Text(Ui, Name); */
  PushNewRow(Ui);

  if (Value)
  {
    cs ButtonName = FSz("%S : 0x%x", Name, Value);
    if (ToggleButton(Ui, Name, ButtonName, umm(Name.Start) ^ umm(Value)))
    {
      PushNewRow(Ui);

      /* PushTableStart(Ui, {}, {}, {}, &DefaultStyle, V4(32, 0, 0, 0)); */
        PushColumn(Ui, CSz("Verts : "));
        PushColumn(Ui, FSz("0x%x",(u64)Value->Verts));
        PushNewRow(Ui);

        PushColumn(Ui, CSz("Normals : "));
        PushColumn(Ui, FSz("0x%x", (u64)Value->Normals));
        PushNewRow(Ui);

        PushColumn(Ui, CSz("Mats : "));
        PushColumn(Ui, FSz("0x%x", (u64)Value->Mat));
        PushNewRow(Ui);

        PushColumn(Ui, CSz("At : "));
        PushColumn(Ui, FSz("%u", Value->At));
        PushNewRow(Ui);

        PushColumn(Ui, CSz("End : "));
        PushColumn(Ui, FSz("%u", Value->End));
        PushNewRow(Ui);

        PushColumn(Ui, CSz("Timestamp : "));
        PushColumn(Ui, FSz("%u", Value->Timestamp));
      /* PushTableEnd(Ui); */
      /* PushForceUpdateBasis(Ui, V2(-16, 0)); */
    }
  }
  else
  {
    PushColumn(Ui, Name);
    PushColumn(Ui, CSz(" : null"));
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
  DoEditorUi(Ui, &WorldP, "WorldP");

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
      DebugUi(Engine, ToString(Bit), Mesh);
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

  StartColumn(Ui);
    Text(Ui, Name);
    Text(Ui, Value->Title);
  EndColumn(Ui);
  PushNewRow(Ui);
}

link_internal void
DebugUi(engine_resources *Engine, cs Name, interactable *Value)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  PushColumn(Ui, Name);
  PushNewRow(Ui);
  DoEditorUi(Ui, &Value->ID, "ID");
  DoEditorUi(Ui, &Value->MinP, "MinP");
  DoEditorUi(Ui, &Value->MaxP, "MaxP");
}

link_internal maybe_file_traversal_node
EngineDrawFileNodesHelper(file_traversal_node Node)
{
  engine_resources *Engine = GetEngineResources();
  maybe_file_traversal_node Result = DrawFileNodes(&Engine->Ui, Node);
  return Result;
}

link_internal void
DoLevelWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  local_persist window_layout Window = WindowLayout("Level");

  PushWindowStart(Ui, &Window);

  PushTableStart(Ui);
    if (Button(Ui, CSz("Export Level"), umm("export_level_button")))
    {
      u32 ChunkCount = 0;
      RangeIterator(HashIndex, s32(World->HashSize))
      {
        if (World->ChunkHash[HashIndex])
        {
          ++ChunkCount;
        }
      }

      native_file LevelFile = OpenFile("levels/test.level", "w+b");

      level_header Header = {};
      Header.ChunkCount = ChunkCount;

      Header.WorldFlags    = Cast(u32, World->Flags);
      Header.WorldCenter   = World->Center;
      Header.VisibleRegion = World->VisibleRegion;

      Header.Camera = *Graphics->Camera;
      Header.CameraTarget = Engine->CameraGhost->P;

      WriteToFile(&LevelFile, (u8*)&Header, sizeof(level_header));

      RangeIterator(HashIndex, s32(World->HashSize))
      {
        if (world_chunk *Chunk = World->ChunkHash[HashIndex])
        {
          SerializeChunk(Chunk, &LevelFile);
        }
      }

      CloseFile(&LevelFile);
    }
  PushTableEnd(Ui);

  PushTableStart(Ui);
    maybe_file_traversal_node ClickedNode = PlatformTraverseDirectoryTree(CSz("levels"), EngineDrawFileNodesHelper);
  PushTableEnd(Ui);

  if (ClickedNode.Tag)
  {
    cs Filename = Concat(ClickedNode.Value.Dir, CSz("/"), ClickedNode.Value.Name, GetTranArena());
    u8_stream LevelBytes = U8_StreamFromFile(Filename, GetTranArena());

    if (LevelBytes.Start)
    {
      u32 ChunksFreed = 0;
      RangeIterator(HashIndex, s32(World->HashSize))
      {
        if (world_chunk *Chunk = World->ChunkHash[HashIndex])
        {
          FreeWorldChunk(World, Chunk, &Engine->MeshFreelist, World->Memory);
          World->ChunkHash[HashIndex] = 0;
          ++ChunksFreed;
        }
      }


      level_header LevelHeader = {};
      ReadBytesIntoBuffer(&LevelBytes, sizeof(level_header), Cast(u8*, &LevelHeader));

      *Graphics->Camera      = LevelHeader.Camera;
      Engine->CameraGhost->P = LevelHeader.CameraTarget;

      /* World->Flags  = Cast(world_flag, LevelHeader.WorldFlags); */
      World->Center = LevelHeader.WorldCenter;
      /* World->VisibleRegion = LevelHeader.VisibleRegion; */

      Assert(LevelHeader.MagicNumber == LEVEL_HEADER_MAGIC_NUMBER);

      s32 ChunkCount = Cast(s32, LevelHeader.ChunkCount);

      Info("ChunksFreed (%u) ChunksLoaded (%u)", ChunksFreed, ChunkCount);

      RangeIterator(ChunkIndex, ChunkCount)
      {
        world_chunk *Chunk = GetFreeWorldChunk(World, World->Memory);
        DeserializeChunk(&LevelBytes, Chunk, &Engine->MeshFreelist, World->Memory);
        if (IsInsideVisibleRegion(World, Chunk->WorldP))
        {
          InsertChunkIntoWorld(World, Chunk);
        }
      }

      Assert(LevelBytes.At == LevelBytes.End);

    }
  }

  PushWindowEnd(Ui, &Window);
}

link_internal void
DoEntityWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  if (Engine->MaybeMouseRay.Tag)
  {
    entity *Entity = GetEntitiesIntersectingRay(World, EntityTable, &Engine->MaybeMouseRay.Ray);
    if (Entity && Entity != EngineDebug->SelectedEntity)
    {
      Assert(Spawned(Entity));
      DrawEntityCollisionVolume(Entity, &GpuMap->Buffer, Graphics, World->ChunkDim, YELLOW);

      if (Input->LMB.Clicked)
      {
        EngineDebug->SelectedEntity = Entity;
      }
    }
  }

  if (EngineDebug->SelectedEntity)
  {
    DrawEntityCollisionVolume(EngineDebug->SelectedEntity, &GpuMap->Buffer, Graphics, World->ChunkDim, GREEN);

    local_persist window_layout EntityWindow = WindowLayout("Entity");

    PushWindowStart(Ui, &EntityWindow);
      PushTableStart(Ui);
        PushColumn(Ui, CSz("EntityIndex"));
        PushColumn(Ui, CS(EngineDebug->SelectedEntity->Id), &DefaultStyle, {}, ColumnRenderParam_LeftAlign);
        PushNewRow(Ui);

        DoEditorUi(Ui, EngineDebug->SelectedEntity, "SelectedEntity");
      PushTableEnd(Ui);
    PushWindowEnd(Ui, &EntityWindow);
  }
}

link_internal void
DoEngineDebug(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

#if 0
  ui_toggle_button_handle Buttons[] = {
    UiToggle(CSz("Level"),          0),
    UiToggle(CSz("Edit"),           0),
    UiToggle(CSz("Assets"),         0),
    UiToggle(CSz("WorldChunks"),    0),
    UiToggle(CSz("Textures"),       0),
    UiToggle(CSz("RenderSettings"), 0),
    UiToggle(CSz("EngineDebug"),    0),
  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    .Start = Buttons,
    .Count = ArrayCount(Buttons)
  };

  ui_toggle_button_group EditorButtonGroup = UiToggleButtonGroup(Ui, &ButtonBuffer);
#else
  ui_toggle_button_group EditorButtonGroup = ToggleButtonGroup_engine_debug_view_mode(Ui, umm("engine_debug_view_mode"));
#endif


  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_Entities))
  {
    DoEntityWindow(Engine);
  }

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_Level))
  {
    DoLevelWindow(Engine);
  }

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_WorldEdit))
  {
    DoLevelEditor(Engine);
  }

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_WorldChunks))
  {
    local_persist window_layout WorldChunkWindow = WindowLayout("World Chunks");
    PushWindowStart(Ui, &WorldChunkWindow);

      if ( Clicked(&EditorButtonGroup, CSz("WorldChunks")) ||
           Button(Ui, CSz("PickNewChunk"), (umm)"Pick"^(umm)"WorldChunks") )
      {
        EngineDebug->PickedChunkState = PickedChunkState_Hover;
      }

      if (Button(Ui, CSz("RebuildMesh"), (umm)"RebuildMesh"^(umm)"WorldChunks"))
      {
        world_chunk *PickedChunk = EngineDebug->PickedChunk;
        MarkBoundaryVoxels_Debug(PickedChunk->Voxels, PickedChunk->Dim);
        /* MarkBoundaryVoxels_NoExteriorFaces(PickedChunk->Voxels, PickedChunk->Dim, {}, PickedChunk->Dim); */
        QueueChunkForMeshRebuild(&Plat->LowPriority, PickedChunk);
      }
      PushNewRow(Ui);

      if (EngineDebug->PickedChunkState == PickedChunkState_Hover)
      {
        if (Engine->MousedOverVoxel.Tag)
        {
          EngineDebug->PickedChunk = Engine->MousedOverVoxel.Value.PickedChunk.Chunk;
          if (Input->LMB.Clicked) { EngineDebug->PickedChunkState = PickedChunkState_None; }
        }
      }

      if (EngineDebug->PickedChunk)
      {
        DebugUi(Engine, CSz("PickedChunk"), EngineDebug->PickedChunk );
      }

    PushWindowEnd(Ui, &WorldChunkWindow);
  }

#if 1
  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_Textures))
  {
    v2 TexDim = V2(400);

    auto Flags = window_layout_flags(WindowLayoutFlag_StartupAlign_Bottom|WindowLayoutFlag_StartupSize_InferWidth);
    local_persist window_layout TexturesWindow = WindowLayout("Textures", {}, V2(0.f, TexDim.y + 30.f), Flags);
    PushWindowStart(Ui, &TexturesWindow);

    s32 xAdvance = 15;

    PushTableStart(Ui);

      PushColumn(Ui, CSz("Transparency AccumTex"));
      PushColumn(Ui, CSz("Transparency RevealTex"));
      PushColumn(Ui, CSz("Transparency Depth"));
      PushColumn(Ui, CSz("Lighting"));
      PushColumn(Ui, CSz("Bloom"));
      PushNewRow(Ui);

      StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Transparency.AccumTex, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui);

      StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Transparency.RevealTex, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui);

      StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Transparency.Depth, TexDim, zDepth_Text, True);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui);

      StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Lighting.LightingTex, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui);

      StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Lighting.BloomTex, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui);
      PushNewRow(Ui);

      PushColumn(Ui, CSz("gBuffer Albedo"));
      PushColumn(Ui, CSz("gBuffer Normal"));
      PushColumn(Ui, CSz("gBuffer Position"));
      PushColumn(Ui, CSz("gBuffer Depth "));
      PushNewRow(Ui);

      StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->gBuffer->Textures->Color, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui);

      StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->gBuffer->Textures->Normal, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui);

      StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->gBuffer->Textures->Position, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui);

      StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->gBuffer->Textures->Depth, TexDim, zDepth_Text, True);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui);
      PushNewRow(Ui);



    PushTableEnd(Ui);


    /* IterateOver(&EngineDebug->Textures, Texture, TextureIndex) */
    /* { */
    /*   PushTexturedQuad(Ui, DebugTextureArraySlice_Font, V2(120), zDepth_Text); */
    /* } */

    PushWindowEnd(Ui, &TexturesWindow);
  }
#endif

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_RenderSettings))
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout RenderSettingsWindow = WindowLayout("Render Settings", WindowLayoutFlag_StartupAlign_Right);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &RenderSettingsWindow);

      PushTableStart(Ui);
        /* DoEditorUi(Ui, &Ui->Pressed.ID, "ID"); */

        DoEditorUi(Ui, (b8*)&Settings->DoDayNightCycle, "DoDayNightCycle");
        PushNewRow(Ui);

        DoEditorUi(Ui, (b8*)&Settings->UseSsao, "UseSsao");
        DoEditorUi(Ui, (b8*)&Settings->UseShadowMapping, "UseShadowMapping");
        DoEditorUi(Ui, (b8*)&Settings->UseLightingBloom, "UseLightingBloom");

        DoEditorUi(Ui, (b8*)&Settings->BravoilMyersOIT, "BravoilMyersOIT");
        DoEditorUi(Ui, (b8*)&Settings->BravoilMcGuireOIT, "BravoilMcGuireOIT");

        // TODO(Jesse): Make a slider for time of day

        DebugSlider(Ui, &Graphics->Exposure, 0.0f, 5.f);

      PushTableEnd(Ui);

    PushWindowEnd(Ui, &RenderSettingsWindow);
  }

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_EngineDebug))
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout Window = WindowLayout("Engine Debug", WindowLayoutFlag_StartupAlign_Right);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &Window);
      PushTableStart(Ui);

        DoEditorUi(Ui, &EngineDebug->DrawEntityCollisionVolumes, "DrawEntityCollisionVolumes");
        DoEditorUi(Ui, &EngineDebug->DrawWorldAxies, "DrawWorldAxies");

        DoEditorUi(Ui, &EngineDebug->UiDebug.OutlineUiValues, "OutlineUiValues");
        DoEditorUi(Ui, &EngineDebug->UiDebug.OutlineUiButtons, "OutlineUiButtons");
        DoEditorUi(Ui, &EngineDebug->UiDebug.OutlineUiTables, "OutlineUiTables");

        DoEditorUi(Ui, &Engine->Editor.SelectedColorIndex, "SelectedColorIndex");
        DebugUi(Engine, CSz("Selected Color"), &Engine->Editor.SelectedColorSquare);

      PushTableEnd(Ui);
    PushWindowEnd(Ui, &Window);
  }

  /* Debug_DrawTextureToDebugQuad(&Engine->RTTGroup.DebugShader); */

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_Assets))
  {
    v2 AssetListWindowDim = {{350.f, 1200.f}};
    local_persist window_layout Window = WindowLayout("Assets", DefaultWindowBasis(*Ui->ScreenDim, AssetListWindowDim), AssetListWindowDim);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &Window);
      maybe_file_traversal_node ClickedFileNode = PlatformTraverseDirectoryTree(CSz("models"), EngineDrawFileNodesHelper);
    PushWindowEnd(Ui, &Window);

    if (ClickedFileNode.Tag)
    {
      EngineDebug->ResetAssetNodeView = True;
      EngineDebug->SelectedAsset = ClickedFileNode.Value;
    }

    if (EngineDebug->SelectedAsset.Type)
    {
      v2 AssetDetailWindowDim = {{400.f, 400.f}};
      local_persist window_layout AssetViewWindow = WindowLayout("Asset View", {}, AssetDetailWindowDim, window_layout_flags(WindowLayoutFlag_StartupAlign_Right|WindowLayoutFlag_Default));
      PushWindowStart(Ui, &AssetViewWindow);

      auto AssetSpawnModeRadioGroup = RadioButtonGroup_asset_spawn_mode(Ui, umm("asset_spawn_mode_radio_group"));

      asset *Asset = GetAsset(Engine, &EngineDebug->SelectedAsset);

      PushColumn(Ui, Asset->FileNode.Name);
      PushNewRow(Ui);

      switch (Asset->LoadState)
      {
        case AssetLoadState_Loaded:
        {
          v3 Offset = Asset->Model.Dim/-2.f;
          RenderToTexture(Engine, &Asset->Model.Mesh, Offset);

          interactable_handle B = PushButtonStart(Ui, umm("asset_texture_viewport") );
            PushTexturedQuad(Ui, Engine->RTTGroup.Texture, V2(Engine->RTTGroup.Texture->Dim), zDepth_Text);
          PushButtonEnd(Ui);

          if (EngineDebug->ResetAssetNodeView)
          {
            Engine->RTTGroup.Camera->DistanceFromTarget = Length(Offset) * 25.f;
            EngineDebug->ResetAssetNodeView = False;
          }

          v2 MouseDP = {};
          if (Pressed(Ui, &B)) { MouseDP = GetMouseDelta(Plat); }
          UpdateGameCamera(World, MouseDP, Input, {}, Engine->RTTGroup.Camera);

          if (UiCapturedMouseInput(Ui) == False && Input->Space.Clicked)
          {

            if (Engine->MousedOverVoxel.Tag)
            {
              cp EntityOrigin = Canonical_Position(&Engine->MousedOverVoxel.Value);
              world_update_op_shape_params_asset AssetUpdateShape =
              {
                Asset,
                EntityOrigin
              };


              asset_spawn_mode AssetSpawnMode = {};
              GetRadioEnum(&AssetSpawnModeRadioGroup, &AssetSpawnMode);
              switch (AssetSpawnMode)
              {
                case AssetSpawnMode_BlitIntoWorld:
                {
                  world_update_op_shape Shape =
                  {
                    type_world_update_op_shape_params_asset,
                    .world_update_op_shape_params_asset = AssetUpdateShape,
                  };
                  QueueWorldUpdateForRegion(Engine, {}, &Shape, {}, World->Memory);
                } break;

                case AssetSpawnMode_Entity:
                {
                  entity *E = GetFreeEntity(Engine->EntityTable);
                  SpawnEntity(E, &Asset->Model, EntityBehaviorFlags_Default, 0, &EntityOrigin, Asset->Model.Dim/2.f);
                } break;
              }

            }
          }

        } break;

        case AssetLoadState_Queued:
        case AssetLoadState_Loading:
        {
          Text(Ui, CSz("Loading Asset"));
        } break;

        case AssetLoadState_Unloaded:
        {
          Text(Ui, CSz("Not Loading Asset .. ?"));
        } break;

        case AssetLoadState_Error:
        {
          Text(Ui, CSz("Error Loading Asset :("));
        } break;
      }

      PushWindowEnd(Ui, &AssetViewWindow);
    }
  }
}
