
link_internal maybe_file_traversal_node
EngineDrawFileNodesHelper(file_traversal_node Node, u64 Window)
{
  engine_resources *Engine = GetEngineResources();
  maybe_file_traversal_node Result = DrawFileNodes(&Engine->Ui, (window_layout*)Window, Node);
  return Result;
}

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
      Global_SerializeTypeTableArena = AllocateArena();
      Global_SerializeTypeTable = Allocate_bonsai_type_info_hashtable(64, Global_SerializeTypeTableArena);

      u32 ChunkCount = 0;
      RangeIterator(HashIndex, s32(World->HashSize))
      {
        if (World->ChunkHash[HashIndex])
        {
          ++ChunkCount;
        }
      }

      /* native_file LevelFile = OpenFile("../bonsai_levels/test.level", "w+b"); */

      u8_cursor_block_array OutputStream = {};
      {
        OutputStream.BlockSize = Megabytes(32);
        u8_cursor First = U8Cursor(OutputStream.BlockSize, Global_SerializeTypeTableArena);
        Ensure( Push(&OutputStream, &First) );
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
      /* WriteToFile(&LevelFile, (u8*)&Header, sizeof(level_header)); */

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
      /* Serialize(&LevelFile, Palette); */


      u8_cursor_block_array TypeBufferOutputStream = {};
      {
        TypeBufferOutputStream.BlockSize = Megabytes(32);

        u8_cursor First = U8Cursor(TypeBufferOutputStream.BlockSize, Global_SerializeTypeTableArena);
        Ensure( Push(&TypeBufferOutputStream, &First) );

        bonsai_type_info_buffer TypeBuffer = ToBuffer(&Global_SerializeTypeTable, Global_SerializeTypeTableArena);
        Serialize(&TypeBufferOutputStream, &TypeBuffer);
      }


      native_file LevelFile = OpenFile("../bonsai_levels/test.level", "w+b");
        u64 FileFormatVersion = LEVEL_FILE_FORMAT_VERSION_NUMBER;
        WriteToFile(&LevelFile, FileFormatVersion);
        WriteToFile(&LevelFile, &TypeBufferOutputStream);
        WriteToFile(&LevelFile, &OutputStream);
      CloseFile(&LevelFile);

      Global_SerializeTypeTable = {};
      VaporizeArena(Global_SerializeTypeTableArena);
      Global_SerializeTypeTableArena = 0;
    }
  PushTableEnd(Ui);
  PushNewRow(Ui);

  PushTableStart(Ui);
    maybe_file_traversal_node ClickedNode = PlatformTraverseDirectoryTree(CSz("../bonsai_levels"), EngineDrawFileNodesHelper, Cast(u64, &Window));
  PushTableEnd(Ui);
  PushNewRow(Ui);

  // Import
  if (ClickedNode.Tag)
  {
    cs Filename = Concat(ClickedNode.Value.Dir, CSz("/"), ClickedNode.Value.Name, GetTranArena());
    u8_stream LevelBytes = U8_StreamFromFile(Filename, Thread->PermMemory);

    b32 Error = False;
    if (LevelBytes.Start)
    {
      u64 LevelHeaderVersion = {};
      Deserialize(&LevelBytes, &LevelHeaderVersion, 0);

      Global_SerializeTypeTableArena = AllocateArena();
      switch(LevelHeaderVersion)
      {
        case 0:
        {
          Global_SerializeTypeTable = Allocate_bonsai_type_info_hashtable(64, Global_SerializeTypeTableArena);

          {
            entity *Dummy = 0;
            auto TI = TypeInfo(Dummy);
            TI.Version = 1;
            Insert(TI, &Global_SerializeTypeTable, Global_SerializeTypeTableArena);
          }

          {
            camera *Dummy = 0;
            auto TI = TypeInfo(Dummy);
            TI.Version = 0;
            Insert(TI, &Global_SerializeTypeTable, Global_SerializeTypeTableArena);
          }

          {
            level_header *Dummy = 0;
            auto TI = TypeInfo(Dummy);
            TI.Version = 0;
            Insert(TI, &Global_SerializeTypeTable, Global_SerializeTypeTableArena);
          }
        } break;

        case 1:
        {
          bonsai_type_info_buffer TypeInfoBuffer = {};
          Deserialize(&LevelBytes, &TypeInfoBuffer, Thread->TempMemory);
          Global_SerializeTypeTable = Allocate_bonsai_type_info_hashtable(NextPowerOfTwo(TypeInfoBuffer.Count), Global_SerializeTypeTableArena);

          IterateOver(&TypeInfoBuffer, TypeInfo, TypeInfoIndex)
          {
            Insert(*TypeInfo, &Global_SerializeTypeTable, Global_SerializeTypeTableArena);
          }
        } break;

        default: { Error=True; SoftError("Could not load level file claiming version (%lu), bailing.", LevelHeaderVersion); }
      }

      level_header LevelHeader = {};
      Deserialize(&LevelBytes, &LevelHeader, Thread->PermMemory);

      if (Error == False)
      {
        SignalAndWaitForWorkers(&Plat->WorkerThreadsSuspendFutex);

        HardResetWorld(Engine);

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
          world_chunk *Chunk = GetFreeWorldChunk(World, World->Memory);
          DeserializeChunk(&LevelBytes, Chunk, &Engine->MeshFreelist, World->Memory);

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

      Global_SerializeTypeTable = {};
      VaporizeArena(Global_SerializeTypeTableArena);
      Global_SerializeTypeTableArena = 0;
    }

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

    local_persist window_layout EntityWindow = WindowLayout("Entity", WindowLayoutFlag_StartupAlign_Right);

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
        selection_mode SelectionMode = {};
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

  local_persist window_layout Window = WindowLayout("Graphics", window_layout_flags(WindowLayoutFlag_StartupAlign_BottomRight|WindowLayoutFlag_DynamicSize|WindowLayoutFlag_StartupSize_Infer) );

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

link_internal void
DoAssetWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  {
    local_persist window_layout Window = WindowLayout("Asset Table");
    PushWindowStart(Ui, &Window);

    AcquireFutex(&Engine->AssetFutex);
    {
      RangeIterator(AssetIndex, ASSET_TABLE_COUNT)
      {
        asset *Asset = Engine->AssetTable + AssetIndex;
        DoEditorUi(Ui, &Window, Asset, CS(AssetIndex));
      }
    }
    ReleaseFutex(&Engine->AssetFutex);

    PushWindowEnd(Ui, &Window);
  }

  {
    v2 AssetListWindowDim = {{350.f, 1200.f}};
    local_persist window_layout Window = WindowLayout("Disk Assets", DefaultWindowBasis(*Ui->ScreenDim, AssetListWindowDim), AssetListWindowDim);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &Window);
      maybe_file_traversal_node ClickedFileNode = PlatformTraverseDirectoryTree(CSz("models"), EngineDrawFileNodesHelper, u64(&Window) );
    PushWindowEnd(Ui, &Window);

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

    if (EngineDebug->SelectedAsset.FileNode.Type)
    {
      local_persist window_layout AssetViewWindow =
        WindowLayout("Asset View", {}, {}, window_layout_flags(WindowLayoutFlag_StartupAlign_Right|WindowLayoutFlag_Default));
      PushWindowStart(Ui, &AssetViewWindow);


      PushTableStart(Ui);
        auto AssetSpawnModeRadioGroup = RadioButtonGroup_asset_spawn_mode(Ui, &AssetViewWindow, "asset_spawn_mode_radio_group");

        maybe_asset_ptr MaybeAsset = GetAssetPtr(Engine, &EngineDebug->SelectedAsset);

        if (MaybeAsset.Tag)
        {
          asset *Asset = MaybeAsset.Value;

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
                SyncGpuBuffersImmediate(Engine, &Model->Meshes);

                render_entity_to_texture_group *RTTGroup = &Engine->RTTGroup;
                if (ModelIndex >= TotalElements(&Editor->AssetThumbnails))
                {
                  // TODO(Jesse): Where to allocate these?
                  texture *T = MakeTexture_RGBA(V2i(256), (u32*)0, Engine->Memory, CSz("Thumbnail"));
                  asset_thumbnail Thumb = { T, {} };
                  StandardCamera(&Thumb.Camera, 10000.0f, 100.0f, 0.f, {});

                  Push(&Editor->AssetThumbnails, &Thumb);
                }

                asset_thumbnail *Thumb = GetPtr(&Editor->AssetThumbnails, ModelIndex);
                texture *Texture = Thumb->Texture;
                camera  *ThumbCamera  = &Thumb->Camera;

                interactable_handle B = PushButtonStart(Ui, UiId(&AssetViewWindow, "asset_texture_viewport", Thumb) );
                  u32 Index = StartColumn(Ui);
                    if (ModelIndex == EngineDebug->ModelIndex) { PushRelativeBorder(Ui, V2(256), UI_WINDOW_BEZEL_DEFAULT_COLOR*1.8f, V4(2.f)); }
                    PushTexturedQuad(Ui, Texture, V2(Texture->Dim), zDepth_Text);
                    PushForceAdvance(Ui, V2(8, 0));
                  EndColumn(Ui, Index);
                PushButtonEnd(Ui);

                v3 ModelCenterpointOffset = Model->Dim/-2.f;
                if (EngineDebug->ResetAssetNodeView)
                {
                  AssetViewWindow.Scroll = {};
                  AssetViewWindow.CachedScroll = {};
                  f32 SmallObjectCorrectionFactor = 350.f/Length(ModelCenterpointOffset);
                  ThumbCamera->DistanceFromTarget = LengthSq(ModelCenterpointOffset)*0.50f + SmallObjectCorrectionFactor;
                  UpdateGameCamera(World, {}, 0.f, {}, ThumbCamera, 0.f);
                  RenderToTexture(Engine, Thumb, Model, {});
                }


                v2 MouseDP = {};
                r32 CameraZDelta = {};
                if (Pressed(Ui, &B))
                {
                  EngineDebug->ModelIndex = ModelIndex;

                  if (Input->LMB.Pressed) {MouseDP = GetMouseDelta(Plat)*2.f; }
                  if (Input->RMB.Pressed) { CameraZDelta += GetMouseDelta(Plat).y*2.f; }
                  UpdateGameCamera(World, MouseDP, CameraZDelta, {}, ThumbCamera, 0.f);
                  RenderToTexture(Engine, Thumb, Model, {});
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
                      SetupGBufferShader(Graphics);
                      v3 Basis = GetRenderP(Engine, EntityOrigin) + V3(0.f, 0.f, AssetHalfDim.z);
                      /* v3 Basis = V3(0,0,20); */
                      DrawLod(GetEngineResources(), &Graphics->gBuffer->gBufferShader, &Model->Meshes, 0.f, Basis);
                      TeardownGBufferShader(Graphics);
                    }


                    // Put asset into world
                    //
                    if ( Input->Space.Clicked )
                    {
                      world_update_op_shape_params_asset AssetUpdateShape =
                      {
                        EngineDebug->SelectedAsset,
                        EngineDebug->ModelIndex,
                        Canonicalize(World, EntityOrigin - V3(AssetHalfDim.xy, 0.f))
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
                          QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Additive, &Shape, {}, World->Memory);
                        } break;

                        case AssetSpawnMode_Entity:
                        {
                          entity *E = TryGetFreeEntityPtr(Engine->EntityTable);
                          Assert(E);
                          SpawnEntity(E, &EngineDebug->SelectedAsset, ModelIndex, EntityBehaviorFlags_Default, 0, &AssetUpdateShape.Origin, Model->Dim/2.f);
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

          }
        }
      PushTableEnd(Ui);

      PushWindowEnd(Ui, &AssetViewWindow);
    }
  }
}

link_internal void
DoEngineDebug(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  ui_toggle_button_group EditorButtonGroup =
    ToggleButtonGroup_engine_debug_view_mode(Ui, 0, "engine_debug_view_mode");

  Editor->EngineDebugViewModeToggleBits = EditorButtonGroup.ToggleBits;

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
    DoWorldEditor(Engine);
  }

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_WorldChunks))
  {
    local_persist window_layout WorldChunkWindow = WindowLayout("World Chunks");
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
        WorldChunkWindow.Title = FSz("World Chunk : (%p)", EngineDebug->PickedChunk);
        /* DebugUi(Engine, CSz("PickedChunk"), EngineDebug->PickedChunk ); */
        DoEditorUi(Ui, &WorldChunkWindow, EngineDebug->PickedChunk, CSz("PickedChunk"));
      }

    PushWindowEnd(Ui, &WorldChunkWindow);
  }

#if 1
  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_Textures))
  {
    v2 DefaultTextureDim = V2(250);

    auto Flags = window_layout_flags(WindowLayoutFlag_StartupAlign_Bottom|WindowLayoutFlag_StartupSize_InferWidth);
    local_persist window_layout TexturesWindow = WindowLayout("Textures", {}, V2(0.f, DefaultTextureDim.y + 30.f), Flags);
    PushWindowStart(Ui, &TexturesWindow);

    s32 xAdvance = 15;

    s32 AtColumn = 0;
    IterateOver(&Engine->Stdlib.AllTextures, TexturePtr, TextureIndex)
    {
      texture *Texture = *TexturePtr;

      v2 Dim = Min(V2(Texture->Dim), DefaultTextureDim);

      PushTableStart(Ui);

        PushColumn(Ui, Texture->DebugName, ColumnRenderParam_LeftAlign);
        PushNewRow(Ui);

        u32 StartOuter = StartColumn(Ui);
          PushTableStart(Ui);
              if (Texture->Slices)
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

      PushTableEnd(Ui);

      if ( (GetIndex(&TextureIndex)+1) % 6 == 0)
      {
        PushNewRow(Ui);
      }
    }


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
    local_persist window_layout RenderSettingsWindow = WindowLayout("Graphics Settings", WindowLayoutFlag_StartupAlign_Right);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &RenderSettingsWindow);
      DoEditorUi(Ui, &RenderSettingsWindow, Settings, CSz("Graphics Settings"));
    PushWindowEnd(Ui, &RenderSettingsWindow);
  }

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_EngineDebug))
  {
    v2 WindowDim = {{1200.f, 250.f}};

    {
      local_persist window_layout Window = WindowLayout("Engine Debug", WindowLayoutFlag_StartupAlign_Right);
      PushWindowStart(Ui, &Window);
        PushTableStart(Ui);
        DoEditorUi(Ui, &Window, EngineDebug, CSz("Engine Debug"));
        PushTableEnd(Ui);
        /* DoEditorUi(Ui, &EngineDebug->UiDebug, CSz("UI Debug")); */
        /* DoEditorUi(Ui, &EngineDebug->Render,  CSz("Graphics Debug")); */
      PushWindowEnd(Ui, &Window);
    }
    {
      local_persist window_layout Window = WindowLayout("Engine");
      PushWindowStart(Ui, &Window);
        PushTableStart(Ui);
        DoEditorUi(Ui, &Window, Engine, CSz("Engine Resources"));
        PushTableEnd(Ui);
      PushWindowEnd(Ui, &Window);
    }

    DoGraphicsDebugWindow(Engine);

  }

  /* Debug_DrawTextureToDebugQuad(&Engine->RTTGroup.DebugShader); */

  if (ToggledOn(&EditorButtonGroup, EngineDebugViewMode_Assets))
  {
    DoAssetWindow(Engine);
  }
}
