

link_internal void
DoLevelEditor(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  v2 WindowDim = {{325.f, 1200.f}};
  local_persist window_layout Window = WindowLayout("Edit", DefaultWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);
  PushWindowStart(Ui, &Window);

  ui_element_reference ColorTable = PushTableStart(Ui);
    RangeIterator(ColorIndex, s32(u8_MAX)+1 )
    {
      v3 Color = GetColorData(DefaultPalette, u32(ColorIndex)).rgb;
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
        Engine->Editor.SelectedColorIndex = ColorIndex;
      }

      if (Engine->Editor.SelectedColorIndex == ColorIndex)
      {
        v3 SelectedBorderColor = V3(0.9f);
        PushBorder(Ui, RectMinMax(Engine->Editor.SelectedColorSquare.MinP+Padding.xy, Engine->Editor.SelectedColorSquare.MinP+Padding.xy+QuadDim), SelectedBorderColor, V4(BorderDim));
      }

      if ( (ColorIndex+1) % 8 == 0 ) { PushNewRow(Ui); }
    }
  PushTableEnd(Ui);



  local_persist ui_element_toggle_button Buttons[] = {
    {CSz("Select"),  False,False},
    {CSz("Fill"),    False,False},
    {CSz("Add"),     False,False},
    {CSz("Remove"),  False,False},
    {CSz("Paint"),   False,False},
  };

  ui_element_toggle_button_group ButtonGroup = {
    .Buttons = Buttons,
    .Count = ArrayCount(Buttons),
    .Flags = ui_element_toggle_button_group_flags(ToggleButtonGroupFlags_RadioButtons | ToggleButtonGroupFlags_DrawVertical),
  };

  DrawToggleButtonGroup(Ui, &ButtonGroup, Position_RightOf, ColorTable);

  if ( Editor->SelectionRegion[0].PickedChunk.Chunk )
  {
    v3 P0 = GetAbsoluteP(&Editor->SelectionRegion[0]);
    v3 P1 = P0;

    r32 Thickness = 0.075f;
    u8 Color = WHITE;
    if (Editor->SelectionRegion[1].PickedChunk.Chunk)
    {
      P1 = GetAbsoluteP(&Editor->SelectionRegion[1]);

      Thickness = 0.15f;
      u8 Green = WHITE;
    }
    else
    {
      if (Engine->MousedOverVoxel.PickedChunk.Chunk)
      {
        P1 = GetAbsoluteP(&Engine->MousedOverVoxel);
      }
    }

    v3 MinP = Min(P0, P1);
    v3 MaxP = Max(P0, P1) + V3(1);

    untextured_3d_geometry_buffer OutlineAABB = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
    v3 Offset = V3(0.001f);
    DEBUG_DrawAABB( &OutlineAABB,
                    GetRenderP(World->ChunkDim, MinP-Offset, Camera),
                    GetRenderP(World->ChunkDim, MaxP+Offset, Camera),
                    Color, Thickness);
  }


  if (ToggledOn(&ButtonGroup, CSz("Select")))
  {
    if (Input->LMB.Clicked)
    {
      switch (Editor->SelectPendingClicks)
      {
        case 2:
        {
          Editor->SelectPendingClicks -= 1;
          Editor->SelectionRegion[0] = Engine->MousedOverVoxel;
        } break;

        case 1:
        {
          Editor->SelectPendingClicks -= 1;
          Editor->SelectionRegion[1] = Engine->MousedOverVoxel;
        } break;
      }

    }

  }

  if (Clicked(&ButtonGroup, CSz("Select")))
  {
    Editor->SelectPendingClicks = 2;
    Editor->SelectionRegion[0] = {};
    Editor->SelectionRegion[1] = {};
  }


  if (ToggledOn(&ButtonGroup, CSz("Add")))
  {
    if (Input->LMB.Clicked)
    {
      cp P0 = Canonical_Position(&Editor->SelectionRegion[0]);
      cp P1 = Canonical_Position(&Editor->SelectionRegion[1]);

      picked_voxel Ignored = {};
      world_update_op_shape Shape = {
        .Type = type_world_update_op_shape_params_rect,
        .world_update_op_shape_params_rect.P0 = P0,
        .world_update_op_shape_params_rect.P1 = P1,
      };
      QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Additive, &Shape, SafeTruncateU8(Editor->SelectedColorIndex), 0.f, Engine->Memory);
    }
  }

  if (ToggledOn(&ButtonGroup, CSz("Remove")))
  {
  }

  if (ToggledOn(&ButtonGroup, CSz("Paint")))
  {
    if (Input->LMB.Pressed)
    {
      voxel *V = GetVoxelPointer(&Engine->MousedOverVoxel, PickedVoxel_FirstFilled);

      if (V)
      {
        V->Color = SafeTruncateU8(Engine->Editor.SelectedColorIndex);
        QueueChunkForMeshRebuild(&Plat->LowPriority, Engine->MousedOverVoxel.PickedChunk.Chunk);
      }
    }
  }

  if (Input->LMB.Clicked)
  {
    switch (Editor->Mode)
    {
      case LevelEditMode_None:
      case LevelEditMode_Select:
      case LevelEditMode_Add:
      case LevelEditMode_Remove:
      case LevelEditMode_Paint:
        {
        } break;
    }
  }





  PushWindowEnd(Ui, &Window);

}
