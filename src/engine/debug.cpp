#define UiId(base, mod) umm(umm(base) | ( umm(mod) << 32 ))
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

link_internal void
DebugSlider_(renderer_2d *Ui, r32 *Value, const char* Name, r32 Min, r32 Max)
{
  /* u32 Start = StartColumn(Ui); */
  /*   PushTableStart(Ui); */
      if (Name) { PushColumn(Ui, CS(Name)); }
      /* PushColumn(Ui, CS(*Value)); */

      auto Range = Max-Min;
      r32 PercFilled = ((*Value)-Min)/Range;

      r32 Width = 125.f;
      interactable_handle BargraphButton = PushButtonStart(Ui, (umm)(umm("DebugSlider") ^ umm(Value)));
        PushBargraph(Ui, PercFilled, V3(0.75f), V3(0.4f), Width);
      PushButtonEnd(Ui);

      v2 Offset = {};
      if (Pressed(Ui, &BargraphButton, &Offset))
      {
        r32 NewPerc = Clamp01(Offset.x / Width);
        r32 NewValue = (Range*NewPerc) + Min;
        *Value = NewValue;
      }
    /* PushTableEnd(Ui); */
  /* EndColumn(Ui, Start); */
}





/* #define DoEditorUi(Ui, Value, "Value") DoEditorUi(Ui, Value, STRINGIZE(Value)) */
#define DebugSlider(Ui, Value, Min, Max) DebugSlider_(Ui, Value, STRINGIZE(Value), Min, Max)

link_internal void
DoEditorUi(renderer_2d *Ui, void *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
  Value ?
    PushColumn(Ui, FSz("0x%x",umm(Value)), EDITOR_UI_FUNCTION_INSTANCE_NAMES) :
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, r32 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
      if (Value)
      {
        if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1.f; }

        if (*Value >= 10.f)
        {
          PushColumn(Ui, FSz("%.1f", r64(*Value)), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        }
        else
        {
          PushColumn(Ui, FSz("%.2f", r64(*Value)), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        }

        if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1.f; }
#if 1
          DebugSlider_(Ui, Value, 0, 0.f, 32.f);
#else
        if (*Value <= 2.f)
        {
          DebugSlider_(Ui, Value, 0, -2.f, 2.f);
        }
        else if (*Value >= 2.f && *Value <= 5.f)
        {
          DebugSlider_(Ui, Value, 0, 2.f, 5.f);
        }
        else if (*Value >= 5.f && *Value <= 10.f)
        {
          DebugSlider_(Ui, Value, 0, 5.f, 10.f);
        }
        else if (*Value >= 10.f && *Value <= 50.f)
        {
          DebugSlider_(Ui, Value, 0, 10.f, 50.f);
        }
        else if (*Value >= 50.f && *Value <= 100.f)
        {
          DebugSlider_(Ui, Value, 0, 50.f, 100.f);
        }
        else if (*Value >= 100.f && *Value <= 1000.f)
        {
          DebugSlider_(Ui, Value, 0, 100.f, 1000.f);
        }
        else
        {
          DebugSlider_(Ui, Value, 0, 1000.f, 10000.f);
        }
#endif
        PushNewRow(Ui);
      }
      else
      {
        PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }
    PushTableEnd(Ui);
  EndColumn(Ui, Start);
}

link_internal void
DoEditorUi(renderer_2d *Ui, b8 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Button(Ui, CS(Name), (umm)Value + (umm)"toggle", EDITOR_UI_FUNCTION_INSTANCE_NAMES )) { *Value = !(*Value); }

  counted_string Display = Value ?
       *Value ? CSz("True") : CSz("False") :
       CSz("(null)");

  Text(Ui, Display);
}


poof(do_editor_ui_for_scalar_type({s64 u64 s32 u32 s16 u16 s8 u8}));
#include <generated/do_editor_ui_for_scalar_type_688724926.h>

poof(do_editor_ui_for_vector_type({v4i v4 v3i v3 v2 Quaternion}));
#include <generated/do_editor_ui_for_vector_type_688873645.h>

link_internal void
DoEditorUi(renderer_2d *Ui, cp *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); */

  DoEditorUi(Ui, &Value->WorldP, "WorldP");
  PushNewRow(Ui);
  DoEditorUi(Ui, &Value->Offset, "Offset");
}



#if DO_EDITOR_UI_FOR_ENTITY_TYPE
link_internal void
DoEditorUi(renderer_2d *Ui, entity_type *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_ARGUMENTS);
#endif


poof(do_editor_ui_for_compound_type(physics))
#include <generated/do_editor_ui_for_compound_type_physics.h>

poof(do_editor_ui_for_compound_type(aabb))
#include <generated/do_editor_ui_for_compound_type_aabb.h>

poof(do_editor_ui_for_compound_type(particle_system))
#include <generated/do_editor_ui_for_compound_type_particle_system.h>

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

  u32 Start = StartColumn(Ui);
    Text(Ui, Name);
    Text(Ui, Value->Title);
  EndColumn(Ui, Start);
  PushNewRow(Ui);
}

link_internal void
DebugUi(engine_resources *Engine, cs Name, interactable *Value)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  PushColumn(Ui, Name);
  PushNewRow(Ui);

  DoEditorUi(Ui, &Value->ID, "ID");
  PushNewRow(Ui);

  DoEditorUi(Ui, &Value->MinP, "MinP");
  PushNewRow(Ui);

  DoEditorUi(Ui, &Value->MaxP, "MaxP");
  PushNewRow(Ui);
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

      u32 EntityCount = 0;
      RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
      {
        entity *E = EntityTable[EntityIndex];
        if (Spawned(E)) { ++EntityCount; }
      }
      Header.EntityCount = EntityCount;


      WriteToFile(&LevelFile, (u8*)&Header, sizeof(level_header));

      u64 Delimeter = LEVEL_FILE_DEBUG_OBJECT_DELIM;
      RangeIterator(HashIndex, s32(World->HashSize))
      {
        if (world_chunk *Chunk = World->ChunkHash[HashIndex])
        {
          SerializeChunk(Chunk, &LevelFile);
          /* Ensure(Serialize(&LevelFile, &Delimeter)); */
        }
      }

      Ensure(Serialize(&LevelFile, &Delimeter));

      RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
      {
        entity *E = EntityTable[EntityIndex];
        if (Spawned(E))
        {
          Serialize(&LevelFile, E);
          /* Ensure(Serialize(&LevelFile, &Delimeter)); */
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
    thread_local_state *Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);
    u8_stream LevelBytes = U8_StreamFromFile(Filename, Thread->PermMemory);

    if (LevelBytes.Start)
    {
      level_header LevelHeader = {};
      ReadBytesIntoBuffer(&LevelBytes, sizeof(level_header), Cast(u8*, &LevelHeader));

      if (LevelHeader.MagicNumber == LEVEL_HEADER_MAGIC_NUMBER)
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

        *Graphics->Camera      = LevelHeader.Camera;
        Engine->CameraGhost->P = LevelHeader.CameraTarget;

        /* World->Flags  = Cast(world_flag, LevelHeader.WorldFlags); */
        World->Center = LevelHeader.WorldCenter;
        /* World->VisibleRegion = LevelHeader.VisibleRegion; */

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

        s32 EntityCount = Cast(s32, LevelHeader.EntityCount);

        RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT) { Unspawn(EntityTable[EntityIndex]); }

        Ensure(Read_u64(&LevelBytes) == LEVEL_FILE_DEBUG_OBJECT_DELIM);

        RangeIterator(EntityIndex, EntityCount)
        {
          Deserialize(&LevelBytes, EntityTable[EntityIndex], Thread->PermMemory);
        }

        Assert(LevelBytes.At == LevelBytes.End);
      }
      else
      {
        SoftError("Could not load corrupt level file");
      }

    }
  }

  PushWindowEnd(Ui, &Window);
}

link_internal void
DoEntityWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);


  {
    local_persist window_layout EntityWindow = WindowLayout("All Entities");

    PushWindowStart(Ui, &EntityWindow);
      PushTableStart(Ui);

        RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
        {
          entity *Entity = EntityTable[EntityIndex];
          DoEditorUi(Ui, Entity, GetNullTerminated(FSz("E (%d)", EntityIndex)) );
          PushNewRow(Ui);
        }
      PushTableEnd(Ui);

    PushWindowEnd(Ui, &EntityWindow);
  }

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
    DrawEntityCollisionVolume(EngineDebug->SelectedEntity, &GpuMap->Buffer, Graphics, World->ChunkDim, WHITE);

    local_persist window_layout EntityWindow = WindowLayout("Entity");

    PushWindowStart(Ui, &EntityWindow);
      PushTableStart(Ui);
        PushColumn(Ui, CSz("EntityIndex"));
        PushColumn(Ui, CS(EngineDebug->SelectedEntity->Id), &DefaultStyle, {}, ColumnRenderParam_LeftAlign);
        PushNewRow(Ui);

        DoEditorUi(Ui, EngineDebug->SelectedEntity, "SelectedEntity");
        PushNewRow(Ui);
      PushTableEnd(Ui);
    PushWindowEnd(Ui, &EntityWindow);


    aabb EntityAABB = GetSimSpaceAABB(World, EngineDebug->SelectedEntity);

    if (Engine->MaybeMouseRay.Tag)
    {
      aabb_intersect_result AABBTest = Intersect(EntityAABB, &Engine->MaybeMouseRay.Ray);
      if (AABBTest.Face)
      {
        HighlightFace(Engine, AABBTest.Face, EntityAABB, 0.f, GREEN, 0.15f);
      }
    }
  }


}

link_internal void
DoGraphicsDebugWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  local_persist window_layout Window = WindowLayout("Graphics");

  PushWindowStart(Ui, &Window);
    PushTableStart(Ui);
    PushColumn(Ui, CSz("Solid Bytes :"));
    PushColumn(Ui, CS(EngineDebug->Render.BytesSolidGeoLastFrame));
    PushNewRow(Ui);

    PushColumn(Ui, CSz("Transp Bytes :"));
    PushColumn(Ui, CS(EngineDebug->Render.BytesTransGeoLastFrame));

    PushTableEnd(Ui);
  PushWindowEnd(Ui, &Window);
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
          EngineDebug->PickedChunk = Engine->MousedOverVoxel.Value.Chunks[PickedVoxel_FirstFilled].Chunk;
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

      u32 Start = StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Transparency.AccumTex, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui, Start);

      Start = StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Transparency.RevealTex, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui, Start);

      Start = StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Transparency.Depth, TexDim, zDepth_Text, True);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui, Start);

      Start = StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Lighting.LightingTex, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui, Start);

      Start = StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->Lighting.BloomTex, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui, Start);
      PushNewRow(Ui);

      PushColumn(Ui, CSz("gBuffer Albedo"));
      PushColumn(Ui, CSz("gBuffer Normal"));
      PushColumn(Ui, CSz("gBuffer Position"));
      PushColumn(Ui, CSz("gBuffer Depth "));
      PushNewRow(Ui);

      Start = StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->gBuffer->Textures->Color, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui, Start);

      Start = StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->gBuffer->Textures->Normal, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui, Start);

      Start = StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->gBuffer->Textures->Position, TexDim, zDepth_Text);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui, Start);

      Start = StartColumn(Ui);
        PushTexturedQuad(Ui, Graphics->gBuffer->Textures->Depth, TexDim, zDepth_Text, True);
        PushForceAdvance(Ui, V2(xAdvance, 0));
      EndColumn(Ui, Start);
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
        PushNewRow(Ui);
        DoEditorUi(Ui, (b8*)&Settings->UseShadowMapping, "UseShadowMapping");
        PushNewRow(Ui);
        DoEditorUi(Ui, (b8*)&Settings->UseLightingBloom, "UseLightingBloom");
        PushNewRow(Ui);

        DoEditorUi(Ui, (b8*)&Settings->BravoilMyersOIT, "BravoilMyersOIT");
        PushNewRow(Ui);
        DoEditorUi(Ui, (b8*)&Settings->BravoilMcGuireOIT, "BravoilMcGuireOIT");
        PushNewRow(Ui);

        // TODO(Jesse): Make a slider for time of day

        DebugSlider(Ui, &Settings->MajorGridDim, 1.0f, 16.f);
        DebugSlider(Ui, &Graphics->Exposure, 0.0f, 5.f);

      PushTableEnd(Ui);

    PushWindowEnd(Ui, &RenderSettingsWindow);
  }

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_EngineDebug))
  {
    DoGraphicsDebugWindow(Engine);

    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout Window = WindowLayout("Engine Debug", WindowLayoutFlag_StartupAlign_Right);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &Window);
      PushTableStart(Ui);

        DoEditorUi(Ui, &EngineDebug->DrawEntityCollisionVolumes, "DrawEntityCollisionVolumes");
        PushNewRow(Ui);
        DoEditorUi(Ui, &EngineDebug->DrawWorldAxies, "DrawWorldAxies");
        PushNewRow(Ui);

        DoEditorUi(Ui, &EngineDebug->UiDebug.OutlineUiValues,  "OutlineUiValues");
        PushNewRow(Ui);
        DoEditorUi(Ui, &EngineDebug->UiDebug.OutlineUiButtons, "OutlineUiButtons");
        PushNewRow(Ui);
        DoEditorUi(Ui, &EngineDebug->UiDebug.OutlineUiTables,  "OutlineUiTables");
        PushNewRow(Ui);

        DoEditorUi(Ui, &Engine->Editor.SelectedColorIndex, "SelectedColorIndex");
        PushNewRow(Ui);

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


      PushTableStart(Ui);
        auto AssetSpawnModeRadioGroup = RadioButtonGroup_asset_spawn_mode(Ui, umm("asset_spawn_mode_radio_group"));

        asset *Asset = GetAsset(Engine, &EngineDebug->SelectedAsset);

        PushColumn(Ui, Asset->FileNode.Name);
        PushNewRow(Ui);
        switch (Asset->LoadState)
        {
          case AssetLoadState_Loaded:
          {
            IterateOver(&Asset->Models, Model, ModelIndex)
            {
              auto *RTTGroup = &Engine->RTTGroup;
              if (ModelIndex >= TotalElements(&Editor->AssetThumbnails))
              {
                // TODO(Jesse): Where to allocate these?
                texture *T = MakeTexture_RGBA(V2i(256), (u32*)0, Engine->Memory);
                asset_thumbnail Thumb = { T, {} };
                StandardCamera(&Thumb.Camera, 10000.0f, 100.0f, {});

                Push(&Editor->AssetThumbnails, &Thumb);
              }

              asset_thumbnail *Thumb = GetPtr(&Editor->AssetThumbnails, ModelIndex);
              texture *Texture = Thumb->Texture;
              camera  *ThumbCamera  = &Thumb->Camera;

              interactable_handle B = PushButtonStart(Ui, UiId(Thumb, "asset_texture_viewport") );
                u32 Index = StartColumn(Ui);
                  if (Model == Editor->SelectedAssetModel) { PushRelativeBorder(Ui, V2(256), UI_WINDOW_BEZEL_DEFAULT_COLOR*1.8f, V4(2.f)); }
                  PushTexturedQuad(Ui, Texture, V2(Texture->Dim), zDepth_Text);
                  PushForceAdvance(Ui, V2(8, 0));
                EndColumn(Ui, Index);
              PushButtonEnd(Ui);

              v3 ModelCenterpointOffset = Model->Dim/-2.f;
              if (EngineDebug->ResetAssetNodeView)
              {
                f32 SmallObjectCorrectionFactor = 350.f/Length(ModelCenterpointOffset);
                ThumbCamera->DistanceFromTarget = LengthSq(ModelCenterpointOffset)*0.50f + SmallObjectCorrectionFactor;
                UpdateGameCamera(World, {}, 0.f, {}, ThumbCamera, 1.f);
                RenderToTexture(Engine, Thumb, &Model->Mesh, ModelCenterpointOffset);
              }


              v2 MouseDP = {};
              r32 CameraZDelta = {};
              if (Pressed(Ui, &B))
              {
                Editor->SelectedAssetModel = Model;

                if (Input->LMB.Pressed) {MouseDP = GetMouseDelta(Plat)*2.f; }
                if (Input->RMB.Pressed) { CameraZDelta += GetMouseDelta(Plat).y*2.f; }
                UpdateGameCamera(World, MouseDP, CameraZDelta, {}, ThumbCamera, 1.f);
                RenderToTexture(Engine, Thumb, &Model->Mesh, ModelCenterpointOffset);
              }


              if ( Engine->MousedOverVoxel.Tag )
              {
                cp EntityOrigin = Canonical_Position(&Engine->MousedOverVoxel.Value);
                EntityOrigin.Offset = Round(EntityOrigin.Offset);
                if ( !UiCapturedMouseInput(Ui) && Model == Editor->SelectedAssetModel )
                {

                  /* Assert(Model->Mesh.Mat == 0); */

                  RangeIterator_t(u32, ElementIndex, Model->Mesh.At) { Model->Mesh.Mat[ElementIndex].Transparency = 0.5f; }

                  {
                    untextured_3d_geometry_buffer *Dest = &Graphics->Transparency.GpuBuffer.Buffer;
                    BufferChunkMesh(Graphics, Dest, &Model->Mesh, World->ChunkDim, EntityOrigin.WorldP, 1.f, EntityOrigin.Offset, Quaternion());
                  }
                  {
                    /* untextured_3d_geometry_buffer *Dest = &GpuMap->Buffer; */
                    /* BufferChunkMesh(Graphics, Dest, &Model->Mesh, World->ChunkDim, EntityOrigin.WorldP, 1.f, EntityOrigin.Offset, Quaternion()); */
                  }

                  RangeIterator_t(u32, ElementIndex, Model->Mesh.At) { Model->Mesh.Mat[ElementIndex].Transparency = 0.f; }

                  if ( Input->Space.Clicked )
                  {
                    world_update_op_shape_params_asset AssetUpdateShape =
                    {
                      Asset,
                      ModelIndex,
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
                        SpawnEntity(E, Model, EntityBehaviorFlags_Default, 0, &EntityOrigin, Model->Dim/2.f);
                      } break;
                    }
                  }
                }
              }

              if ( (ModelIndex+1) % 4 == 0)
              {
                PushNewRow(Ui);
                PushForceAdvance(Ui, V2(0, 8));
              }
            }

            if (EngineDebug->ResetAssetNodeView) { EngineDebug->ResetAssetNodeView = False; }
          } break;

          case AssetLoadState_Queued:
          case AssetLoadState_Loading:
          {
            PushColumn(Ui, CSz("Loading Asset"));
          } break;

          case AssetLoadState_Unloaded:
          {
            PushColumn(Ui, CSz("Not Loading Asset .. ?"));
          } break;

          case AssetLoadState_Error:
          {
            PushColumn(Ui, CSz("Error Loading Asset :("));
          } break;
        }
      PushTableEnd(Ui);

      PushWindowEnd(Ui, &AssetViewWindow);
    }
  }
}
