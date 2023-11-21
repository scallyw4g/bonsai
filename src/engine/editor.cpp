
link_internal rect3i
ModifySelectionAABB(rect3 *SelectionRegion, v3 UpdateVector, face_index Face, selection_mode Mode)
{
  rect3i Result = Rect3i(SelectionRegion);

  switch (Face)
  {
    InvalidCase(FaceIndex_None);

    case FaceIndex_Top:
    {
      Result.Max += V3i(0, 0, (s32)Floorf(UpdateVector.z+1.f) );
      if (Mode == SelectionMode_Translate)
      {
        Result.Min += V3i(0, 0, (s32)Floorf(UpdateVector.z+1.f) );
      }
    } break;

    case FaceIndex_Bot:
    {
      Result.Min += V3i(0, 0, (s32)Floorf(UpdateVector.z+1.f) );
      if (Mode == SelectionMode_Translate)
      {
        Result.Max += V3i(0, 0, (s32)Floorf(UpdateVector.z+1.f) );
      }
    } break;

    case FaceIndex_Left:
    {
      Result.Min += V3i((s32)Floorf(UpdateVector.x+1.f), 0, 0 );
      if (Mode == SelectionMode_Translate)
      {
        Result.Max += V3i((s32)Floorf(UpdateVector.x+1.f), 0, 0 );
      }
    } break;

    case FaceIndex_Right:
    {
      Result.Max += V3i((s32)Floorf(UpdateVector.x+1.f), 0, 0 );
      if (Mode == SelectionMode_Translate)
      {
        Result.Min += V3i((s32)Floorf(UpdateVector.x+1.f), 0, 0 );
      }
    } break;

    case FaceIndex_Front:
    {
      Result.Max += V3i(0, (s32)Floorf(UpdateVector.y+1.f), 0 );
      if (Mode == SelectionMode_Translate)
      {
        Result.Min += V3i(0, (s32)Floorf(UpdateVector.y+1.f), 0 );
      }
    } break;

    case FaceIndex_Back:
    {
      Result.Min += V3i(0, (s32)Floorf(UpdateVector.y+1.f), 0 );
      if (Mode == SelectionMode_Translate)
      {
        Result.Max += V3i(0, (s32)Floorf(UpdateVector.y+1.f), 0 );
      }
    } break;
  }

  return Result;
}

link_internal v3
GetMin(v3 *SelectionRegion)
{
  v3 Result = Min(SelectionRegion[0], SelectionRegion[1]);
  return Result;
}

link_internal v3
GetMax(v3 *SelectionRegion)
{
  v3 Result = Max(SelectionRegion[0], SelectionRegion[1]) + 1;
  return Result;
}


link_internal void
DoLevelEditor(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  v2 WindowDim = {{325.f, 1200.f}};
  local_persist window_layout Window = WindowLayout("World Edit", DefaultWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);

  PushWindowStart(Ui, &Window);

  ui_toggle_button_group WorldEditModeRadioGroup = RadioButtonGroup_world_edit_mode(Ui, umm("world_edit_mode_radio_group"), ToggleButtonGroupFlags_DrawVertical, {}, {}, {}, &DefaultStyle, V4(0, 0, 0, 16));

  ui_element_reference ColorTable = PushTableStart(Ui);
    RangeIterator(ColorIndex, s32(u8_MAX)+1 )
    {
      v3 Color = GetColorData(DefaultPalette, u32(ColorIndex));
      ui_style Style = FlatUiStyle(Color);

      v2 QuadDim = V2(22);
      v4 Padding = V4(1);

      interactable_handle ColorPickerButton = PushButtonStart(Ui, (umm)"ColorPicker" ^ (umm)(ColorIndex+12657674));
        PushUntexturedQuad(Ui, {}, QuadDim, zDepth_Text, &Style, Padding );
      PushButtonEnd(Ui);


      r32 BorderDim = -1.f;
      v3 BorderColor = V3(0.f);
      if (Hover(Ui, &ColorPickerButton))
      {
        TriggeredRuntimeBreak(&Input->F12.Clicked);

        BorderColor = V3(0.7f);
        PushBorder(Ui, RectMinMax(Ui->Hover.MinP+Padding.xy, Ui->Hover.MinP+Padding.xy+QuadDim), BorderColor, V4(BorderDim));
      }

      if (Clicked(Ui, &ColorPickerButton))
      {
        BorderColor = V3(1.0f);
        PushBorder(Ui, RectMinMax(Ui->Hover.MinP+Padding.xy, Ui->Hover.MinP+Padding.xy+QuadDim), BorderColor, V4(BorderDim));

        Engine->Editor.SelectedColorSquare = Ui->Clicked;
        Engine->Editor.SelectedColorIndex  = ColorIndex;
      }

      if (Engine->Editor.SelectedColorIndex == ColorIndex)
      {
        v3 SelectedBorderColor = V3(0.9f);
        PushBorder(Ui, RectMinMax(Engine->Editor.SelectedColorSquare.MinP+Padding.xy, Engine->Editor.SelectedColorSquare.MinP+Padding.xy+QuadDim), SelectedBorderColor, V4(BorderDim));
      }

      if ( (ColorIndex+1) % 8 == 0 ) { PushNewRow(Ui); }
    }
  PushTableEnd(Ui);


  aabb_intersect_result AABBTest = {};
  if (Editor->SelectionClicks)
  {
    r32 Thickness = 0.10f;

    if (SelectionIncomplete(Editor->SelectionClicks))
    {
      if (Engine->MousedOverVoxel.Tag)
      {
        auto MouseP = Floor(GetSimSpaceP(World, &Engine->MousedOverVoxel.Value));
        Editor->SelectionRegion = AABBMinMax(GetMin(Editor->SelectionRegion), MouseP);

        /* auto Radius = Abs(SimMouseP - Editor->SelectionRegion.Center); */
        /* auto MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value); */
        /* Editor->SelectionRegion = RectMinMax(Editor->SelectionRegion.Min, MouseP); */
      }
    }
    else { Thickness = 0.20f; }

    /* v3 SelectionMinP = GetSimSpaceP(World, Editor->SelectionRegion.Min); */
    /* v3 SelectionMaxP = GetSimSpaceP(World, Editor->SelectionRegion.Max); */
    /* cp SelectionMinP = Editor->SelectionRegion.Min; */
    /* cp SelectionMaxP = Editor->SelectionRegion.Max; */
    v3 SelectionMinP = GetMin(Editor->SelectionRegion);
    v3 SelectionMaxP = GetMax(Editor->SelectionRegion);

    u8 BaseColor = WHITE;

    rect3 SelectionAABB = AABBMinMax(SelectionMinP,SelectionMaxP); //Editor->SelectionRegion;

    maybe_ray MaybeRay = ComputeRayFromCursor(Plat, &gBuffer->ViewProjection, Camera, World->ChunkDim);
    if (MaybeRay.Tag == Maybe_Yes)
    {
      ray Ray = MaybeRay.Ray;

      AABBTest = Intersect(SelectionAABB, &Ray);
      face_index Face = AABBTest.Face;

      PushColumn(Ui, CS(Face));
      PushNewRow(Ui);
      if (Face)
      {
        /* r32 InsetWidth = 0.25f; */
        r32 InsetWidth  = 0.f;
        r32 HiThickness = Thickness*1.2f;
        u8  HiColor     = GREEN;
        switch (Face)
        {
          InvalidCase(FaceIndex_None);

          case FaceIndex_Top:
          {
            v3 HighlightInset = V3(InsetWidth, InsetWidth, 0.f);
            v3 MinHiP = SelectionMinP + (SelectionAABB.Radius*V3(0.f, 0.f, 2.f)) + HighlightInset;
            v3 MaxHiP = SelectionMaxP - HighlightInset;
            DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
          } break;

          case FaceIndex_Bot:
          {
            v3 HighlightInset = V3(InsetWidth, InsetWidth, 0.f);
            v3 MinHiP = SelectionMinP + HighlightInset;
            v3 MaxHiP = SelectionMaxP - (SelectionAABB.Radius*V3(0.f, 0.f, 2.f)) - HighlightInset;
            DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
          } break;

          case FaceIndex_Left:
          {
            v3 HighlightInset = V3(0.f, InsetWidth, InsetWidth);
            v3 MinHiP = SelectionMinP + HighlightInset;
            v3 MaxHiP = SelectionMaxP - (SelectionAABB.Radius*V3(2.f, 0.f, 0.f)) - HighlightInset;
            DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
          } break;

          case FaceIndex_Right:
          {
            v3 HighlightInset = V3(0.f, InsetWidth, InsetWidth);
            v3 MinHiP = SelectionMinP + (SelectionAABB.Radius*V3(2.f, 0.f, 0.f)) + HighlightInset;
            v3 MaxHiP = SelectionMaxP - HighlightInset;
            DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
          } break;

          case FaceIndex_Back:
          {
            v3 HighlightInset = V3(InsetWidth, 0.f, InsetWidth);
            v3 MinHiP = SelectionMinP + HighlightInset;
            v3 MaxHiP = SelectionMaxP - (SelectionAABB.Radius*V3(0.f, 2.f, 0.f)) - HighlightInset;
            DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
          } break;

          case FaceIndex_Front:
          {
            v3 HighlightInset = V3(InsetWidth, 0.f, InsetWidth);
            v3 MinHiP = SelectionMinP + (SelectionAABB.Radius*V3(0.f, 2.f, 0.f)) + HighlightInset;
            v3 MaxHiP = SelectionMaxP - HighlightInset;
            DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
          } break;
        }


        if (Input->Shift.Pressed && Input->LMB.Clicked)
        {
          v3 PlaneBaseP = Ray.Origin + (AABBTest.t*Ray.Dir);
          Editor->SelectionShiftClickedFace = Face;
          Editor->SelectionShiftClickedP[0] = PlaneBaseP;
        }
      }

      if (Editor->SelectionShiftClickedFace)
      {
        v3 Normal = NormalForFace(Editor->SelectionShiftClickedFace);
        v3 PerpN  = Cross(Normal, Camera->Front);
        v3 PlaneN = Cross(Normal, PerpN);

        f32 tRay = {};
        if (Intersect(PlaneN, Editor->SelectionShiftClickedP[0], Ray.Origin, Ray.Dir, &tRay))
        {
          v3 PlaneIntersect = Ray.Origin + (Ray.Dir*tRay);
          DEBUG_HighlightVoxel(Engine, PlaneIntersect, RED);
          if (Input->LMB.Pressed)
          {
            Editor->SelectionShiftClickedP[1] = PlaneIntersect;
          }
        }

        v3 UpdateVector = Abs(Normal)*(Editor->SelectionShiftClickedP[1] - Editor->SelectionShiftClickedP[0]);

        {
          DEBUG_HighlightVoxel(Engine, Editor->SelectionShiftClickedP[0], RED);
          DEBUG_HighlightVoxel(Engine, Editor->SelectionShiftClickedP[1], BLUE);
          DEBUG_DrawSimSpaceVectorAt(Engine, Editor->SelectionShiftClickedP[0], UpdateVector, GREEN);
        }

        rect3i NewDims = {};

        if (Input->Shift.Pressed)
        {
          Ui->RequestedForceCapture = True;
          if (Input->Ctrl.Pressed)
          {
            NewDims = ModifySelectionAABB(&SelectionAABB, UpdateVector, Editor->SelectionShiftClickedFace, SelectionMode_Translate);
          }
          else
          {
            NewDims = ModifySelectionAABB(&SelectionAABB, UpdateVector, Editor->SelectionShiftClickedFace, SelectionMode_Resize);
          }
        }

        // Draw selection modification region
        //
        v3 P0 = GetRenderP(Engine, V3(NewDims.Min));
        v3 P1 = GetRenderP(Engine, V3(NewDims.Max));
        DEBUG_DrawAABB(Engine, P0, P1, WHITE, 0.1f);

        if (!Input->LMB.Pressed)
        {
          // Make NewDims permanent
          Editor->SelectionRegion = Rect3(&NewDims);
          /* Editor->SelectionRegion = Rect3CP(&NewDims); */
          Editor->SelectionShiftClickedFace = FaceIndex_None;
        }
      }
    }

    // Draw selection box
    //
    DEBUG_DrawSimSpaceAABB(Engine, &SelectionAABB, BaseColor, Thickness);
  }


  if (Clicked(&WorldEditModeRadioGroup, CSz("Select")))
  {
    Editor->SelectionClicks = 0;
    Editor->SelectionRegion  = {};
  }

  world_edit_mode WorldEditMode = {};
  GetRadioEnum(&WorldEditModeRadioGroup, &WorldEditMode);

  {
    /* DoEditorUi(Ui, &Editor->SelectionRegion, "SelectionRegion", &DefaultStyle, {}, {}); */
    /* DoEditorUi(Ui, &World->Center,           "WorldCenter",     &DefaultStyle, {}, {}); */
  }

  /* Info("%S", ToString(WorldEditMode)); */

  picked_voxel_position HighlightVoxel = PickedVoxel_FirstFilled;
  switch (WorldEditMode)
  {
    case WorldEditMode_Select:
    {
      if (Input->LMB.Clicked)
      {
        switch (Editor->SelectionClicks)
        {
          case 0:
          {
            if (Engine->MousedOverVoxel.Tag)
            {
              Editor->SelectionClicks += 1;
              /* auto MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value); */
              /* Editor->SelectionRegion.Min = MouseP; */

              auto MouseP = Floor(GetSimSpaceP(World, &Engine->MousedOverVoxel.Value));
              Editor->SelectionRegion.Center = MouseP;
            }
          } break;

          case 1:
          {
            Editor->SelectionClicks += 1;
          } break;

        }
      }
    } break;

    case WorldEditMode_FillSelection:
    {
      if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
      {
        world_update_op_shape Shape = {
          .Type = type_world_update_op_shape_params_rect,
          .world_update_op_shape_params_rect.P0 = GetMin(Editor->SelectionRegion),
          .world_update_op_shape_params_rect.P1 = GetMax(Editor->SelectionRegion),
          /* .world_update_op_shape_params_rect.Region = Editor->SelectionRegion, */
        };
        QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Additive, &Shape, SafeTruncateU8(Editor->SelectedColorIndex), Engine->Memory);
      }
    } break;

    case WorldEditMode_DeleteSelection:
    {
      if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
      {
        NotImplemented;
        world_update_op_shape Shape = {
          .Type = type_world_update_op_shape_params_rect,
          /* .world_update_op_shape_params_rect.P0 = GetMin(Editor->SelectionRegion), */
          /* .world_update_op_shape_params_rect.P1 = GetMax(Editor->SelectionRegion), */
        };
        QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Subtractive, &Shape, SafeTruncateU8(Editor->SelectedColorIndex), Engine->Memory);
      }
    } break;


    case WorldEditMode_AddSingle:
    {
      HighlightVoxel = PickedVoxel_LastEmpty;
      if (Input->LMB.Clicked)
      {
        if (Engine->MousedOverVoxel.Tag)
        {
          v3 P0 = Floor(GetSimSpaceP(World, &Engine->MousedOverVoxel.Value, HighlightVoxel));

        NotImplemented;
          world_update_op_shape Shape = {
            .Type = type_world_update_op_shape_params_rect,
            /* .world_update_op_shape_params_rect.P0 = P0, */
            /* .world_update_op_shape_params_rect.P1 = P0+1, */
          };
          QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Additive, &Shape, SafeTruncateU8(Editor->SelectedColorIndex), Engine->Memory);
        }
      }
    } break;

    case WorldEditMode_RemoveSingle:
    {
      HighlightVoxel = PickedVoxel_FirstFilled;
      if (Input->LMB.Clicked)
      {
        if (Engine->MousedOverVoxel.Tag)
        {
          v3 P0 = Floor(GetSimSpaceP(World, &Engine->MousedOverVoxel.Value, HighlightVoxel));

          NotImplemented;
          world_update_op_shape Shape = {
            .Type = type_world_update_op_shape_params_rect,
            /* .world_update_op_shape_params_rect.P0 = P0, */
            /* .world_update_op_shape_params_rect.P1 = P0+1, */
          };
          QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Subtractive, &Shape, SafeTruncateU8(Editor->SelectedColorIndex), Engine->Memory);
        }
      }
    } break;

    case WorldEditMode_PaintSingle:
    {
      if (Input->LMB.Pressed)
      {
        if (Engine->MousedOverVoxel.Tag)
        {
          voxel *V = GetVoxelPointer(&Engine->MousedOverVoxel.Value, PickedVoxel_FirstFilled);

          if (V)
          {
            V->Color = SafeTruncateU8(Engine->Editor.SelectedColorIndex);
            QueueChunkForMeshRebuild(&Plat->LowPriority, Engine->MousedOverVoxel.Value.PickedChunk.Chunk);
          }
        }
      }
    } break;
  }

  PushWindowEnd(Ui, &Window);



  if (Editor->SelectionClicks == 2)
  {
    if (Input->Ctrl.Pressed && Input->C.Clicked)
    {
      Editor->CopyRegion = Editor->SelectionRegion;
    }

    if (Input->Ctrl.Pressed && Input->V.Clicked)
    {
      NotImplemented;
      /* v3 CopyDim = GetDim(Editor->CopyRegion); */
      /* s32 VoxelCount = Volume(CopyDim); */
      /* Leak("voxel *V = Allocate(voxel, Engine->Memory, VoxelCount)"); */
      /* voxel *V = Allocate(voxel, Engine->Memory, VoxelCount); */

      /* Leak(""); */
      /* world_chunk_ptr_buffer Chunks = GatherChunksOverlappingArea(World, Editor->CopyRegion, Engine->Memory); */
      /* GatherVoxelsOverlappingArea(World, Rect3i(Editor->CopyRegion), &Chunks, V, VoxelCount); */

      /* chunk_data D = {}; */
      /* D.Dim = V3i(CopyDim); */
      /* D.Voxels = V; */

      /* world_update_op_shape_params_chunk_data ChunkDataShape = { D, GetMin(Editor->SelectionRegion) }; */

      /* world_update_op_shape Shape = */
      /* { */
      /*   type_world_update_op_shape_params_chunk_data, */
      /*   .world_update_op_shape_params_chunk_data = ChunkDataShape, */
      /* }; */
      /* QueueWorldUpdateForRegion(Engine, {}, &Shape, {}, World->Memory); */
    }
  }


  //
  // Highlight moused over voxel
  //
  if (Engine->MousedOverVoxel.Tag)
  {
    v3 SimP = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, HighlightVoxel));
    DEBUG_HighlightVoxel( Engine, SimP, ORANGE, 0.075f);
  }
}
