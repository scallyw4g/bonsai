

link_internal void
DoLevelEditor(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  v2 WindowDim = {{250.f, 1200.f}};
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
    {CSz("Add"),     False,False},
    {CSz("Remove"),  False,False},
    {CSz("Paint"),   False,False},
  };

  ui_element_toggle_button_group ButtonGroup = {
    .Buttons = Buttons,
    .Count = ArrayCount(Buttons),
    .Flags = ui_element_toggle_button_group_flags(ToggleButtonGroupFlags_RadioButtons | ToggleButtonGroupFlags_DrawVertical),
  };

  PushTableStart(Ui, Position_RightOf, ColorTable);
    DrawToggleButtonGroup(Ui, &ButtonGroup);
  PushTableEnd(Ui);

  if (Clicked(&ButtonGroup, CSz("Select")))
  {
    Editor->SelectRegionCleared = True;
  }

  if (ToggledOn(&ButtonGroup, CSz("Select")))
  {
    if (Input->LMB.Pressed)
    {
      if (Editor->SelectRegionCleared)
      {
        Editor->SelectionRegionMin = Engine->MousedOverVoxel;
        Editor->SelectRegionCleared = False;

        if (Editor->SelectionRegionMin.PickedChunk.tChunk != f32_MAX)
        {
          v3 MinP = GetAbsoluteP(&Editor->SelectionRegionMin);
          v3 MaxP = MinP+1.f;

          untextured_3d_geometry_buffer OutlineAABB = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
          v3 Offset = V3(0.001f);
          DEBUG_DrawAABB( &OutlineAABB,
                          GetRenderP(World->ChunkDim, MinP-Offset, Camera),
                          GetRenderP(World->ChunkDim, MinP+V3(1.f)+Offset, Camera),
                          RED, 0.08f);
        }


      }
      else
      {
        Editor->SelectionRegionMax = Engine->MousedOverVoxel;

        if ( Editor->SelectionRegionMin.PickedChunk.tChunk != f32_MAX &&
             Editor->SelectionRegionMax.PickedChunk.tChunk != f32_MAX )
        {
          v3 MinP = GetAbsoluteP(&Editor->SelectionRegionMin);
          v3 MaxP = GetAbsoluteP(&Editor->SelectionRegionMax);

          untextured_3d_geometry_buffer OutlineAABB = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
          v3 Offset = V3(0.001f);
          DEBUG_DrawAABB( &OutlineAABB,
                          GetRenderP(World->ChunkDim, MinP-Offset, Camera),
                          GetRenderP(World->ChunkDim, MaxP+Offset, Camera),
                          RED, 0.08f);
        }

      }

    }
  }

  if (ToggledOn(&ButtonGroup, CSz("Add")))
  {
  }

  if (ToggledOn(&ButtonGroup, CSz("Remove")))
  {
  }

  if (ToggledOn(&ButtonGroup, CSz("Paint")))
  {
    if (Input->LMB.Pressed)
    {
      voxel *V = GetVoxelPointer(&Engine->MousedOverVoxel);

      if (V)
      {
        V->Color = SafeTruncateU8(Engine->Editor.SelectedColorIndex);
        QueueChunkForMeshRebuild(&Plat->HighPriority, Engine->MousedOverVoxel.PickedChunk.Chunk);
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
