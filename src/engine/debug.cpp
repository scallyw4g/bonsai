
link_internal void
DoEntityWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

#if 1
  {
    window_layout *EntityWindow = GetOrCreateWindow(Ui, "All Entities");

    PushWindowStart(Ui, EntityWindow);
      PushTableStart(Ui);

        RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
        {
          entity *Entity = EntityTable[EntityIndex];
          cs Label = FSz("(%d) (%S) (%S)", EntityIndex, ToString(Entity->State), Entity->AssetId.FileNode.Name);
          DoEditorUi(Ui, EntityWindow, Entity, Label, u32(Hash(EntityWindow)) );
        }
      PushTableEnd(Ui);

    PushWindowEnd(Ui, EntityWindow);
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
        DrawEntityCollisionVolume(Entity, &GpuMap->Buffer, Graphics, World->ChunkDim, RGB_YELLOW);

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
    DrawEntityCollisionVolume(SelectedEntity, &GpuMap->Buffer, Graphics, World->ChunkDim, RGB_WHITE);

    window_layout *EntityWindow = GetOrCreateWindow(Ui, "Entity Details", WindowLayoutFlag_Align_Right);

    PushWindowStart(Ui, EntityWindow);
      PushTableStart(Ui);
        DoEditorUi(Ui, EntityWindow, SelectedEntity, FSz("SelectedEntity (%d)", SelectedEntity->Id.Index), u32(Hash(EntityWindow)) );
        PushNewRow(Ui);
      PushTableEnd(Ui);
    PushWindowEnd(Ui, EntityWindow);


#if 0
    aabb EntityAABB = GetSimSpaceAABB(World, SelectedEntity);

    if (Engine->MaybeMouseRay.Tag)
    {
      ray *Ray = &Engine->MaybeMouseRay.Ray;
      aabb_intersect_result AABBTest = Intersect(EntityAABB, Ray);

      auto Face = AABBTest.Face;
      if (Face)
      {
        HighlightFace(Engine, AABBTest.Face, EntityAABB, 0.f, RGB_GREEN, 0.15f);

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
#endif
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

link_internal void
FilenameModal(void *VoidEngine)
{
  engine_resources *Engine = Cast(engine_resources*, VoidEngine);
  UNPACK_ENGINE_RESOURCES(Engine);

  Text(Ui, CSz("Callback"));
}

link_internal void
RenderMeshPreviewIntoWorld(engine_resources *Engine, gpu_mapped_element_buffer *Mesh, v3 Dim, b32 Selected)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  if ( Engine->MousedOverVoxel.Tag )
  {
    cp EntityOrigin = Canonical_Position(&Engine->MousedOverVoxel.Value);
    EntityOrigin.Offset = Round(EntityOrigin.Offset);

    if ( !UiHoveredMouseInput(Ui) && Selected )
    {
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
        /* SetupGBufferShader(Graphics, GetApplicationResolution(&Engine->Settings)); */

        PushBonsaiRenderCommandSetupShader(HiRenderQ, BonsaiRenderCommand_ShaderId_gBuffer);

        v3 AssetHalfDim = Dim/2.f;
        v3 Basis = GetRenderP(Engine, EntityOrigin) + V3(0.f, 0.f, AssetHalfDim.z);
        NotImplemented;
        /* DrawLod_Async(HiRenderQ, GetEngineResources(), &Graphics->gBuffer->gBufferShader, Mesh, Basis, Quaternion(), V3(1)); */

        PushBonsaiRenderCommandTeardownShader(HiRenderQ, BonsaiRenderCommand_ShaderId_gBuffer);
      }

    }
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
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Engine);


  // TODO(Jesse): Rework with DoEditorUi ?
  ui_toggle_button_group EditorButtonGroup =
    ToggleButtonGroup_engine_debug_view_mode(Ui, 0, CS(""), &Engine->EngineDebug.ViewMode, &DefaultUiRenderParams_Column);

  u64 CellsGenerated = EngineDebug->CellsGenerated;

  if (CellsGenerated)
  {
    r64 CyclesPerCell = EngineDebug->ChunkGenCyclesElapsed / CellsGenerated;
    Text(Ui, CSz("CyclesPerCell : "));
    Text(Ui, CS(CyclesPerCell));
    PushNewRow(Ui);
  }

  r64 ChunkGenSeconds = EngineDebug->ChunkGenTimeElapsedMS / 1000.0;
  if (ChunkGenSeconds != 0.0)
  {
    r64 CellsPerSecond = (CellsGenerated/ChunkGenSeconds);

    Text(Ui, CSz("CellsPerSecond : "));
    Text(Ui, CS(CellsPerSecond));
    PushNewRow(Ui);
  }

  engine_debug_view_mode ViewMode = Engine->EngineDebug.ViewMode;

  /* Editor->EngineDebugViewModeToggleBits = EditorButtonGroup.ToggleBits; */

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
    window_layout *WorldChunkWindow = GetOrCreateWindow(Ui, "World Chunks");
    auto *PickedNode = EngineDebug->PickedNode;
    auto *PickedChunk = PickedNode ? PickedNode->Chunk : 0;

    WorldChunkWindow->Title = PickedChunk ?
      FSz("World Chunk : (%p)", PickedChunk) :
      CSz("World Chunk");

    PushWindowStart(Ui, WorldChunkWindow);

      if ( Clicked(&EditorButtonGroup, CSz("WorldChunks")) ||
           Button(Ui, CSz("PickNewChunk"), UiId(WorldChunkWindow, "PickWorldChunkButton", 0ull) ) )
      {
        EngineDebug->PickedChunkState = PickedChunkState_Hover;
      }

      /* if (Button(Ui, CSz("RebuildMesh"), UiId(WorldChunkWindow, "RebuildMesh WorldChunkWindow", 0ull)) ) */
      /* { */
      /*   MakeFaceMasks_NoExteriorFaces(PickedChunk->Occupancy, PickedChunk->xOccupancyBorder, PickedChunk->FaceMasks, PickedChunk->Voxels, PickedChunk->Dim, {}, PickedChunk->Dim); */
      /*   QueueChunkForMeshRebuild(&Plat->LowPriority, PickedChunk); */
      /* } */
      /* PushNewRow(Ui); */

      if (EngineDebug->PickedChunkState == PickedChunkState_Hover)
      {
        if (Engine->MousedOverVoxel.Tag)
        {
          EngineDebug->PickedNode = Engine->MousedOverVoxel.Value.Chunks[PickedVoxel_FirstFilled].Node;
          if (Input->LMB.Clicked) { EngineDebug->PickedChunkState = PickedChunkState_None; }
        }
      }

      if (EngineDebug->PickedNode)
      {
        DoEditorUi(Ui, WorldChunkWindow, EngineDebug->PickedNode, {}, u32(Hash(WorldChunkWindow)));
      }

    PushWindowEnd(Ui, WorldChunkWindow);
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

      auto Button = ToggleButtonStart(Ui, UiId(&TexturesWindow, u64(Texture), u64(0)));
        PushColumn(Ui, FSz("(%u) %S (%dx%d) Slices(%u) Channels(%u) IsDepthTexture(%b)", Texture->ID, Texture->DebugName, Texture->Dim.x, Texture->Dim.y, Texture->Slices, Texture->Channels, Texture->IsDepthTexture), UiElementAlignmentFlag_LeftAlign); PushNewRow(Ui);
      ToggleButtonEnd(Ui);

      if (ToggledOn(Ui, &Button)) { Dim = V2(Texture->Dim) * 8.f; }

        u32 StartOuter = StartColumn(Ui);
          PushTableStart(Ui);
              if (Texture->Slices > 1)
              {
                RangeIterator_t(u32, SliceIndex, Texture->Slices)
                {
                    u32 Start = StartColumn(Ui);
                      PushTexturedQuad(Ui, Texture, s32(SliceIndex), Dim, zDepth_Text);
                      PushForceAdvance(Ui, V2(xAdvance, 0));
                    EndColumn(Ui, Start);

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
    window_layout *RenderSettingsWindow = GetOrCreateWindow(Ui, "Graphics Settings", window_layout_flags(WindowLayoutFlag_Default | WindowLayoutFlag_Align_Right));

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, RenderSettingsWindow);
      DoEditorUi(Ui, RenderSettingsWindow, Settings, {}, u32(Hash(RenderSettingsWindow)));
    PushWindowEnd(Ui, RenderSettingsWindow);
  }

  if (ViewMode & EngineDebugViewMode_EngineDebug)
  {
    v2 WindowDim = {{1200.f, 250.f}};

    {
      window_layout *Window = GetOrCreateWindow(Ui, "Engine Debug", WindowLayoutFlag_Align_Right);
      PushWindowStart(Ui, Window);
        DoEditorUi(Ui, Window, EngineDebug, {}, u32(Hash(Window)) );
      PushWindowEnd(Ui, Window);
    }
    {
      window_layout *Window = GetOrCreateWindow(Ui, "Engine");
      PushWindowStart(Ui, Window);
        DoEditorUi(Ui, Window, Engine, {}, u32(Hash(Window)));
      PushWindowEnd(Ui, Window);
    }

    DoGraphicsDebugWindow(Engine);

    /* DoWorldEditDebugWindow(Engine); */
  }

}

