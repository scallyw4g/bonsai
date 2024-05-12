
link_internal void
DoLevelWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  local_persist window_layout Window = WindowLayout("Level");

  thread_local_state *Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);

  PushWindowStart(Ui, &Window);
  PushTableStart(Ui);
    if (Button(Ui, CSz("Export Level"), UiId(&Window, "export_level_button", 0ull)))
    {
      u8_cursor_block_array OutputStream = BeginSerialization();

      u32 ChunkCount = 0;
      RangeIterator(HashIndex, s32(World->HashSize))
      {
        if (World->ChunkHash[HashIndex])
        {
          ++ChunkCount;
        }
      }

      level_header Header = {};
      Header.ChunkCount = ChunkCount;

      Header.WorldFlags    = Cast(u32, World->Flags);
      Header.WorldCenter   = World->Center;
      Header.VisibleRegion = World->VisibleRegion;
      Header.Camera = *Camera;

      Header.RenderSettings = Graphics->Settings;

      u32 EntityCount = 0;
      RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
      {
        entity *E = EntityTable[EntityIndex];
        if (Spawned(E)) { ++EntityCount; }
      }
      Header.EntityCount = EntityCount;


      Serialize(&OutputStream, &Header);

      u64 Delimeter = LEVEL_FILE_DEBUG_OBJECT_DELIM;
      RangeIterator(HashIndex, s32(World->HashSize))
      {
        if (world_chunk *Chunk = World->ChunkHash[HashIndex])
        {
          SerializeChunk(Chunk, &OutputStream);
        }
      }

      Ensure(Serialize(&OutputStream, &Delimeter));

      RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
      {
        entity *E = EntityTable[EntityIndex];
        if (Spawned(E))
        {
          Serialize(&OutputStream, E);
        }
      }

      v3_cursor *Palette = GetColorPalette();
      Serialize(&OutputStream, Palette);

      const char *Filename = "../bonsai_levels/test.level";
      if (FinalizeSerialization(&OutputStream, Filename) == False)
      {
        SoftError("Could not serialize (%s).", Filename);
      }
    }
  PushTableEnd(Ui);
  PushNewRow(Ui);

  PushTableStart(Ui);
    maybe_file_traversal_node ClickedNode = PlatformTraverseDirectoryTreeUnordered(CSz("../bonsai_levels"), EngineDrawFileNodesHelper, Cast(u64, &Window));
  PushTableEnd(Ui);
  PushNewRow(Ui);

  // Import
  if (ClickedNode.Tag)
  {
    cs Filename = Concat(ClickedNode.Value.Dir, CSz("/"), ClickedNode.Value.Name, GetTranArena());

    u8_cursor LevelBytes = BeginDeserialization(Filename, GetTranArena());
    if (LevelBytes.Start)
    {
      level_header LevelHeader = {};


      if (Deserialize(&LevelBytes, &LevelHeader, Thread->PermMemory))
      {
        {
          engine_settings *EngineSettings = &GetEngineResources()->Settings;
          LevelHeader.RenderSettings.ApplicationResolution  = V2(GetApplicationResolution(EngineSettings));
          LevelHeader.RenderSettings.ShadowMapResolution    = V2(GetShadowMapResolution(EngineSettings));
          LevelHeader.RenderSettings.LuminanceMapResolution = V2(GetLuminanceMapResolution(EngineSettings));

          LevelHeader.RenderSettings.iApplicationResolution  = GetApplicationResolution(EngineSettings);
          LevelHeader.RenderSettings.iShadowMapResolution    = GetShadowMapResolution(EngineSettings);
          LevelHeader.RenderSettings.iLuminanceMapResolution = GetLuminanceMapResolution(EngineSettings);
        }

        SignalAndWaitForWorkers(&Plat->WorkerThreadsSuspendFutex);

        SoftResetEngine(Engine);

        /* World->Flags  = Cast(world_flag, LevelHeader.WorldFlags); */
        World->Center = LevelHeader.WorldCenter;

        maybe_bonsai_type_info MaybeLevelHeaderTypeInfo = GetByName(&Global_SerializeTypeTable, CSz("level_header"));
        if (MaybeLevelHeaderTypeInfo.Tag)
        {
          if (MaybeLevelHeaderTypeInfo.Value.Version > 2)
          {
            Graphics->Settings = LevelHeader.RenderSettings;
          }
        }

        *Graphics->Camera = LevelHeader.Camera;
        /* World->VisibleRegion = LevelHeader.VisibleRegion; */

        s32 ChunkCount = Cast(s32, LevelHeader.ChunkCount);
        /* Info("ChunksFreed (%u) ChunksLoaded (%u)", ChunksFreed, ChunkCount); */

        RangeIterator(ChunkIndex, ChunkCount)
        {
          world_chunk *Chunk = GetFreeWorldChunk(World);
          DeserializeChunk(&LevelBytes, Chunk, &Engine->world_chunk_MeshFreelist, World->ChunkMemory);

          if (IsInsideVisibleRegion(World, Chunk->WorldP))
          {
            chunk_init_flags Flags = ChunkInitFlag_Noop;

#if 0
            if (Editor->Flags & LevelEditorFlags_RecomputeStandingSpotsOnLevelLoad)
            {
              Chunk->StandingSpots.At = Chunk->StandingSpots.Start;
              Flags = ChunkInitFlag_ComputeStandingSpots;
            }
#endif

            InsertChunkIntoWorld(World, Chunk);

            QueueChunkForMeshRebuild(&GetEngineResources()->Stdlib.Plat.LowPriority, Chunk, Flags);
          }
        }

        u64 Delimeter = LEVEL_FILE_DEBUG_OBJECT_DELIM;
        Ensure(Read_u64(&LevelBytes) == Delimeter);

#if 1

        b32 Error = False;
        u32 EntityCount = LevelHeader.EntityCount;
        RangeIterator_t(u32, EntityIndex, EntityCount)
        {
          entity *E = EntityTable[EntityIndex];
          if (Deserialize(&LevelBytes, E, Thread->PermMemory) == False)
          {
            SoftError("Could not deserialize entity (%d), bailing.", EntityIndex);
            Error = True;
            break;
          }
          E->Id.Index = EntityIndex; // NOTE(Jesse): Hack.. entities got saved out with 0 indexes..
        }

        if (Error == False)
        {
          v3_cursor *Palette = GetColorPalette();
          Palette->At = Palette->Start;
          Deserialize(&LevelBytes, Palette, Thread->PermMemory);
          Assert(LevelBytes.At == LevelBytes.End);
        }

#endif

        Assert(ThreadLocal_ThreadIndex == 0);
        if (Engine->GameApi.OnLibraryLoad) { Engine->GameApi.OnLibraryLoad(Engine, GetThreadLocalState(ThreadLocal_ThreadIndex)); }

        UnsignalFutex(&Plat->WorkerThreadsSuspendFutex);
      }
    }
    FinalizeDeserialization(&LevelBytes);

  }
  PushWindowEnd(Ui, &Window);
}

link_internal void
DoEntityWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

#if 1
  {
    local_persist window_layout EntityWindow = WindowLayout("All Entities");

    PushWindowStart(Ui, &EntityWindow);
      PushTableStart(Ui);

        RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
        {
          entity *Entity = EntityTable[EntityIndex];
          DoEditorUi(Ui, &EntityWindow, Entity, FSz("(%d) (%S) (%S)", EntityIndex, ToString(Entity->State), Entity->AssetId.FileNode.Name) );
        }
      PushTableEnd(Ui);

    PushWindowEnd(Ui, &EntityWindow);
  }
#endif

  if (Engine->MaybeMouseRay.Tag)
  {
    if (Engine->HoverEntity.Tag)
    {
      entity *Entity = Engine->HoverEntity.Value;

      if (Entity && Entity->Id != EngineDebug->SelectedEntity)
      {
        Assert(Spawned(Entity));
        DrawEntityCollisionVolume(Entity, &GpuMap->Buffer, Graphics, World->ChunkDim, YELLOW);

        if (Input->LMB.Clicked)
        {
          EngineDebug->SelectedEntity = Entity->Id;
        }
      }
    }
  }

  entity *SelectedEntity = GetEntity(EntityTable, EngineDebug->SelectedEntity);
  if (SelectedEntity)
  {
    DrawEntityCollisionVolume(SelectedEntity, &GpuMap->Buffer, Graphics, World->ChunkDim, WHITE);

    local_persist window_layout EntityWindow = WindowLayout("Entity", WindowLayoutFlag_Align_Right);

    PushWindowStart(Ui, &EntityWindow);
      PushTableStart(Ui);
        DoEditorUi(Ui, &EntityWindow, SelectedEntity, FSz("SelectedEntity (%d)", SelectedEntity->Id.Index));
        PushNewRow(Ui);
      PushTableEnd(Ui);
    PushWindowEnd(Ui, &EntityWindow);


    aabb EntityAABB = GetSimSpaceAABB(World, SelectedEntity);

    if (Engine->MaybeMouseRay.Tag)
    {
      ray *Ray = &Engine->MaybeMouseRay.Ray;
      aabb_intersect_result AABBTest = Intersect(EntityAABB, Ray);

      auto Face = AABBTest.Face;
      if (Face)
      {
        HighlightFace(Engine, AABBTest.Face, EntityAABB, 0.f, GREEN, 0.15f);

        if ( Input->LMB.Clicked && (Input->Ctrl.Pressed || Input->Shift.Pressed) )
        {
          v3 PlaneBaseP = Ray->Origin + (AABBTest.t*Ray->Dir);
          Editor->Entity.ClickedFace = Face;
          Editor->Entity.ClickedP[0] = PlaneBaseP;
        }
      }

      if (Editor->Entity.ClickedFace)
      {
        world_edit_selection_mode SelectionMode = {};
        if (Input->Shift.Pressed && Input->Ctrl.Pressed)
        {
          SelectionMode = SelectionMode_TranslateLinear;
        }
        else if (Input->Ctrl.Pressed)
        {
          SelectionMode =  SelectionMode_TranslatePlanar;
        }

        rect3i ModifiedSelection = DoSelectonModification(Engine, Ray, SelectionMode, &Editor->Entity, EntityAABB);

        if (Input->LMB.Pressed)
        {
          Ui->RequestedForceCapture = True;
        }
        else
        {
          // Make ModifiedSelection permanent
          cp P = SimSpaceToCanonical(World, V3(ModifiedSelection.Min));
          SelectedEntity->P = P;
          Editor->Entity.ClickedFace = FaceIndex_None;
        }

      }
    }
  }

  if (Input->Delete.Clicked && Engine->HoverEntity.Tag) { Unspawn(Engine->HoverEntity.Value); }
}

link_internal void
DoGraphicsDebugWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  local_persist window_layout Window = WindowLayout("Graphics", window_layout_flags(WindowLayoutFlag_Align_BottomRight) );

  PushWindowStart(Ui, &Window);
    PushTableStart(Ui);
    PushColumn(Ui, CSz("Immediate  Solid Bytes :"));
    PushColumn(Ui, CS(EngineDebug->Render.BytesSolidGeoLastFrame));
    PushNewRow(Ui);

    PushColumn(Ui, CSz("Immediate Transp Bytes :"));
    PushColumn(Ui, CS(EngineDebug->Render.BytesTransGeoLastFrame));

    PushTableEnd(Ui);
  PushWindowEnd(Ui, &Window);
}

link_internal b32
FilterFilenamesByVoxExtension(file_traversal_node *Node)
{
  b32 Result = EndsWith(Node->Name, CSz(".vox"));
  return Result;
}

link_internal void
DoAssetWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  {
    local_persist window_layout Window = WindowLayout("Assets");

    PushWindowStart(Ui, &Window);

    DoEditorUi(Ui, &Window, &Engine->EngineDebug.AssetWindowViewMode, CSz("View"), &DefaultUiRenderParams_Generic);

    switch (Engine->EngineDebug.AssetWindowViewMode)
    {
      case AssetWindowViewMode_AssetFiles:
      {
        render_settings *Settings = &Graphics->Settings;
        filtered_file_traversal_helper_params HelperParams = {&Window, FilterFilenamesByVoxExtension};
        maybe_file_traversal_node ClickedFileNode = PlatformTraverseDirectoryTreeUnordered(CSz("models"), EngineDrawFileNodesFilteredHelper, u64(&HelperParams) );

        if (ClickedFileNode.Tag)
        {
          EngineDebug->ResetAssetNodeView = True;
          maybe_asset_ptr MaybeAsset = GetOrAllocateAsset(Engine, &ClickedFileNode.Value);

          if (MaybeAsset.Tag)
          {
            asset *Asset = MaybeAsset.Value;

            EngineDebug->SelectedAsset = Asset->Id;

            // We allocated a new asset, better load it
            if (Asset->LoadState == AssetLoadState_Allocated )
            {
              QueueAssetForLoad(&Plat->LowPriority, Asset);
            }
          }
          else
          {
            SoftError("Unable to allocate asset to select!");
          }
        }

      } break;

      case AssetWindowViewMode_AssetTable:
      {
        AcquireFutex(&Engine->AssetSystem.AssetFutex);
        {
          RangeIterator(AssetIndex, ASSET_TABLE_COUNT)
          {
            asset *Asset = Engine->AssetSystem.AssetTable + AssetIndex;
            DoEditorUi(Ui, &Window, Asset, CS(AssetIndex));
          }
        }
        ReleaseFutex(&Engine->AssetSystem.AssetFutex);

      } break;
    }

    PushWindowEnd(Ui, &Window);
  }

  if (EngineDebug->SelectedAsset.FileNode.Type)
  {
    local_persist window_layout AssetViewWindow =
      WindowLayout("Asset View", window_layout_flags(WindowLayoutFlag_Align_Right));
    PushWindowStart(Ui, &AssetViewWindow);


    PushTableStart(Ui);
      if (EngineDebug->SelectedAsset.FileNode.Type)
      {
        asset *Asset = GetAndLockAssetSync(Engine, &EngineDebug->SelectedAsset);

        Assert(Asset);
        {
          /* asset *Asset = MaybeAsset.Value; */

          PushColumn(Ui, Asset->Id.FileNode.Name);
          PushNewRow(Ui);
          switch (Asset->LoadState)
          {
            case AssetLoadState_Unloaded:
            case AssetLoadState_Allocated:
            case AssetLoadState_Queued:
            case AssetLoadState_Error:
            {
              PushColumn(Ui, ToString(Asset->LoadState));
            } break;

            case AssetLoadState_Loaded:
            {
              IterateOver(&Asset->Models, Model, ModelIndex)
              {
                SyncGpuBuffersAsync(Engine, &Model->Meshes);

                render_entity_to_texture_group *RTTGroup = &Engine->RTTGroup;
                if (ModelIndex >= TotalElements(&Editor->AssetThumbnails))
                {
                  v2i ThumbnailDim = V2i(128);
                  texture T = MakeTexture_RGBA(ThumbnailDim, (u32*)0, CSz("Thumbnail"));
                  asset_thumbnail Thumb = { T, {} };
                  StandardCamera(&Thumb.Camera, 10000.0f, 100.0f, 0.f);

                  Push(&Editor->AssetThumbnails, &Thumb);
                }

                asset_thumbnail *Thumb = GetPtr(&Editor->AssetThumbnails, ModelIndex);
                texture *Texture      = &Thumb->Texture;
                camera  *ThumbCamera  = &Thumb->Camera;

                if (EngineDebug->ResetAssetNodeView)
                {
                  AssetViewWindow.Scroll = {};
                  AssetViewWindow.CachedScroll = {};

                  v3 ModelCenterpointOffset = Model->Dim/-2.f;
                  f32 SmallObjectCorrectionFactor = 350.f/Length(ModelCenterpointOffset);
                  ThumbCamera->DistanceFromTarget = LengthSq(ModelCenterpointOffset)*0.50f + SmallObjectCorrectionFactor;
                  UpdateGameCamera(World, {}, 0.f, {}, ThumbCamera, 0.f);
                  RenderToTexture(Engine, Thumb, Model, {});
                }

                interactable_handle B = RenderAndInteractWithThumbnailTexture(Ui, &AssetViewWindow, "asset_texture_viewport", Thumb);
                if (ModelIndex == EngineDebug->ModelIndex) { PushRelativeBorder(Ui, V2(Texture->Dim)*V2(-1.f, 1.f), UI_WINDOW_BEZEL_DEFAULT_COLOR*1.8f, V4(2.f)); }
                PushForceAdvance(Ui, V2(8, 0));

                if (Pressed(Ui, &B))
                {
                  RenderToTexture(Engine, Thumb, Model, {});
                  EngineDebug->ModelIndex = ModelIndex;
                }

                if ( Engine->MousedOverVoxel.Tag )
                {
                  cp EntityOrigin = Canonical_Position(&Engine->MousedOverVoxel.Value);
                  EntityOrigin.Offset = Round(EntityOrigin.Offset);

                  if ( !UiHoveredMouseInput(Ui) && ModelIndex == EngineDebug->ModelIndex )
                  {
                    v3 AssetHalfDim = V3(Model->Dim)/2.f;

                    // Draw model marking where the asset will go
                    //
                    {
                      // TODO(Jesse): Setting up and tearing down the shader here
                      // is highly questionable.  We should probably keep a list
                      // of these guys that need this shader, then when we go
                      // to use it when drawing entities just draw them then..
                      //
                      // That said .. this is just editor code.. so .. meh
                      //
                      SetupGBufferShader(Graphics, GetApplicationResolution(&Engine->Settings));
                      v3 Basis = GetRenderP(Engine, EntityOrigin) + V3(0.f, 0.f, AssetHalfDim.z);
                      /* v3 Basis = V3(0,0,20); */
                      DrawLod(GetEngineResources(), &Graphics->gBuffer->gBufferShader, &Model->Meshes, 0.f, Basis);
                      TeardownGBufferShader(Graphics);
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

          }
        }

        UnlockAsset(Engine, Asset);
      }
    PushTableEnd(Ui);

    PushWindowEnd(Ui, &AssetViewWindow);
  }
}

#if 0
link_internal void
DoWorldEditDebugWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  engine_debug *Debug = &Engine->EngineDebug;

  local_persist window_layout Window = WindowLayout("WorldEditDebug");
  {
    PushWindowStart(Ui, &Window);

    UpdateGameCamera(World, {}, {}, {}, &Debug->WorldEditDebugThumb.Camera, {});
    if (Debug->WorldEditDebugMesh.At)
    {
      RenderToTexture(Engine, &Debug->WorldEditDebugThumb, &Debug->WorldEditDebugMesh, {});
      PushTexturedQuad(Ui, &Debug->WorldEditDebugThumb.Texture, V2(512), zDepth_Text);
    }

    PushWindowEnd(Ui, &Window);
  }
}
#endif

link_internal void
DoEngineDebug(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);


  // TODO(Jesse): Rework with DoEditorUi ?
  ui_toggle_button_group EditorButtonGroup =
    ToggleButtonGroup_engine_debug_view_mode(Ui, 0, CS(""), &Engine->EngineDebug.ViewMode, &DefaultUiRenderParams_Column);

  engine_debug_view_mode ViewMode = Engine->EngineDebug.ViewMode;

  /* Editor->EngineDebugViewModeToggleBits = EditorButtonGroup.ToggleBits; */

  // NOTE(Jesse): Do the world editor first so the SelectionChanged flag gets
  // set before the rest of the UI code runs.  This is not strictly necessary
  // for correctness, but  avoids a frame of lag.
  // @selection_changed_flag
  //
  if (ViewMode & EngineDebugViewMode_WorldEdit)
  {
    DoWorldEditor(Engine);
  }

  if (ViewMode & EngineDebugViewMode_Entities)
  {
    DoEntityWindow(Engine);
  }

  if (ViewMode & EngineDebugViewMode_Level)
  {
    DoLevelWindow(Engine);
  }

  if (ViewMode & EngineDebugViewMode_WorldChunks)
  {
    local_persist window_layout WorldChunkWindow = WindowLayout("World Chunks");
    WorldChunkWindow.Title = EngineDebug->PickedChunk ?
      FSz("World Chunk : (%p)", EngineDebug->PickedChunk) :
      CSz("World Chunk");

    PushWindowStart(Ui, &WorldChunkWindow);

      if ( Clicked(&EditorButtonGroup, CSz("WorldChunks")) ||
           Button(Ui, CSz("PickNewChunk"), UiId(&WorldChunkWindow, "PickWorldChunkButton", 0ull) ) )
      {
        EngineDebug->PickedChunkState = PickedChunkState_Hover;
      }

      if (Button(Ui, CSz("RebuildMesh"), UiId(&WorldChunkWindow, "RebuildMesh WorldChunkWindow", 0ull)) )
      {
        world_chunk *PickedChunk = EngineDebug->PickedChunk;
        /* MarkBoundaryVoxels_Debug(PickedChunk->Voxels, PickedChunk->Dim); */
        MarkBoundaryVoxels_NoExteriorFaces(PickedChunk->Voxels, PickedChunk->Dim, {}, PickedChunk->Dim);
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
        DoEditorUi(Ui, &WorldChunkWindow, EngineDebug->PickedChunk, CSz("PickedChunk"));
      }

    PushWindowEnd(Ui, &WorldChunkWindow);
  }

#if 1
  if (ViewMode & EngineDebugViewMode_Textures)
  {
    v2 DefaultTextureDim = V2(250);

    local_persist window_layout TexturesWindow = WindowLayout("Textures", WindowLayoutFlag_Align_Bottom);
    PushWindowStart(Ui, &TexturesWindow);

    s32 xAdvance = 15;

    s32 AtColumn = 0;
    IterateOver(&Engine->Stdlib.AllTextures, Texture, TextureIndex)
    {
      v2 Dim = Min(V2(Texture->Dim), DefaultTextureDim);

      PushTableStart(Ui);

        PushColumn(Ui, FSz("(%u) %S (%dx%d) Slices(%u) Channels(%u) IsDepthTexture(%b)", Texture->ID, Texture->DebugName, Texture->Dim.x, Texture->Dim.y, Texture->Slices, Texture->Channels, Texture->IsDepthTexture), UiElementAlignmentFlag_LeftAlign); PushNewRow(Ui);

        u32 StartOuter = StartColumn(Ui);
          PushTableStart(Ui);
              if (Texture->Slices > 1)
              {
                RangeIterator_t(u32, SliceIndex, Texture->Slices)
                {
                  interactable_handle Button = PushButtonStart(Ui, UiId(&TexturesWindow, u64(Texture), u64(SliceIndex)));
                    u32 Start = StartColumn(Ui);
                      PushTexturedQuad(Ui, Texture, s32(SliceIndex), Dim, zDepth_Text);
                      PushForceAdvance(Ui, V2(xAdvance, 0));
                    EndColumn(Ui, Start);
                  PushButtonEnd(Ui);

                  if ( (SliceIndex+1) % 8 == 0)
                  {
                    PushNewRow(Ui);
                  }

                  if (Hover(Ui, &Button))
                  {
                    PushTooltip(Ui, FSz("Slice(%d)/(%d)", SliceIndex, Texture->Slices));
                  }
                }
                PushNewRow(Ui);

              }
              else
              {
                u32 Start = StartColumn(Ui);
                  PushTexturedQuad(Ui, Texture, Dim, zDepth_Text);
                  PushForceAdvance(Ui, V2(xAdvance, 0));
                EndColumn(Ui, Start);
                PushNewRow(Ui);
              }
          PushTableEnd(Ui);
        EndColumn(Ui, StartOuter);
        PushNewRow(Ui);

      PushTableEnd(Ui);

      /* if ( (GetIndex(&TextureIndex)+1) % 6 == 0) */
      /* { */
      /*   PushNewRow(Ui); */
      /* } */
    }


    /* IterateOver(&EngineDebug->Textures, Texture, TextureIndex) */
    /* { */
    /*   PushTexturedQuad(Ui, DebugTextureArraySlice_Font, V2(120), zDepth_Text); */
    /* } */

    PushWindowEnd(Ui, &TexturesWindow);
  }
#endif

  if (ViewMode & EngineDebugViewMode_RenderSettings)
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout RenderSettingsWindow = WindowLayout("Graphics Settings", WindowLayoutFlag_Align_Right);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &RenderSettingsWindow);
      DoEditorUi(Ui, &RenderSettingsWindow, Settings, {});
    PushWindowEnd(Ui, &RenderSettingsWindow);
  }

  if (ViewMode & EngineDebugViewMode_EngineDebug)
  {
    v2 WindowDim = {{1200.f, 250.f}};

    {
      local_persist window_layout Window = WindowLayout("Engine Debug", WindowLayoutFlag_Align_Right);
      PushWindowStart(Ui, &Window);
        DoEditorUi(Ui, &Window, EngineDebug, {});
      PushWindowEnd(Ui, &Window);
    }
    {
      local_persist window_layout Window = WindowLayout("Engine");
      PushWindowStart(Ui, &Window);
        DoEditorUi(Ui, &Window, Engine, {});
      PushWindowEnd(Ui, &Window);
    }

    DoGraphicsDebugWindow(Engine);

    /* DoWorldEditDebugWindow(Engine); */
  }

  /* Debug_DrawTextureToDebugQuad(&Engine->RTTGroup.DebugShader); */

  if (ViewMode & EngineDebugViewMode_Assets)
  {
    DoAssetWindow(Engine);
  }
}

