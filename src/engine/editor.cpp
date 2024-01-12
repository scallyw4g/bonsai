poof(block_array_c(asset_thumbnail, {8}))
#include <generated/block_array_c_asset_thumbnail_688856411.h>

link_weak ui_debug *
GetUiDebug()
{
  Assert(Global_EngineResources);
  return &Global_EngineResources->EngineDebug.UiDebug;
}

link_internal void
DebugSlider(renderer_2d *Ui, r32 *Value, cs Name, r32 Min, r32 Max, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
      if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
      /* PushColumn(Ui, CS(*Value)); */

      auto Range = Max-Min;
      r32 PercFilled = ((*Value)-Min)/Range;

      r32 Width = 125.f;
      interactable_handle BargraphButton = PushButtonStart(Ui, UiId("DebugSlider", Value));
        PushBargraph(Ui, PercFilled, V3(0.75f), V3(0.4f), Width);
      PushButtonEnd(Ui);

      v2 Offset = {};
      if (Pressed(Ui, &BargraphButton, &Offset))
      {
        r32 NewPerc = Clamp01(Offset.x / Width);
        r32 NewValue = (Range*NewPerc) + Min;
        *Value = NewValue;
      }
    PushTableEnd(Ui);
  EndColumn(Ui, Start);
}

link_internal void
DoEditorUi(renderer_2d *Ui, r32 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

        DebugSlider(Ui, Value, {}, 0.f, 32.f, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }
      else
      {
        PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }
    PushTableEnd(Ui);
  EndColumn(Ui, Start);
  /* PushNewRow(Ui); */
}

link_internal void
DoEditorUi(renderer_2d *Ui, b8 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Button(Ui, CS(Name), UiId(Value, "toggle"), EDITOR_UI_FUNCTION_INSTANCE_NAMES )) { *Value = !(*Value); }

  counted_string Display = Value ?
       *Value ? CSz("True") : CSz("False") :
       CSz("(null)");

  PushColumn(Ui, Display, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}


poof(do_editor_ui_for_scalar_type({s64 u64 s32 u32 s16 u16 s8 u8}));
#include <generated/do_editor_ui_for_scalar_type_688724926.h>

poof(do_editor_ui_for_vector_type({v4i v4 v3i v3 v2 Quaternion}));
#include <generated/do_editor_ui_for_vector_type_688873645.h>

link_internal void
DoEditorUi(renderer_2d *Ui, cs *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  Value ?
    PushColumn(Ui, *Value, EDITOR_UI_FUNCTION_INSTANCE_NAMES) :
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, cp *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, &Value->WorldP, CSz("v3i WorldP"));
  DoEditorUi(Ui, &Value->Offset, CSz("v3 Offset"));
}



#if DO_EDITOR_UI_FOR_ENTITY_TYPE
link_internal void
DoEditorUi(renderer_2d *Ui, entity_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_ARGUMENTS);
#endif

poof(string_and_value_tables(particle_spawn_type))
#include <generated/string_and_value_tables_particle_spawn_type.h>

poof(string_and_value_tables(file_traversal_type))
#include <generated/string_and_value_tables_file_traversal_node.h>





poof(do_editor_ui_for_compound_type(physics))
#include <generated/do_editor_ui_for_compound_type_physics.h>

poof(do_editor_ui_for_compound_type(aabb))
#include <generated/do_editor_ui_for_compound_type_aabb.h>

poof(do_editor_ui_for_compound_type(random_series))
#include <generated/do_editor_ui_for_compound_type_random_series.h>

poof(do_editor_ui_for_enum(particle_spawn_type))
#include <generated/do_editor_ui_for_enum_particle_spawn_type.h>

poof(do_editor_ui_for_compound_type(particle))
#include <generated/do_editor_ui_for_compound_type_particle.h>

poof(do_editor_ui_for_compound_type(particle_system))
#include <generated/do_editor_ui_for_compound_type_particle_system.h>

poof(do_editor_ui_for_compound_type(keyframe))
#include <generated/do_editor_ui_for_compound_type_keyframe.h>

poof(do_editor_ui_for_compound_type(animation))
#include <generated/do_editor_ui_for_compound_type_animation.h>

poof(do_editor_ui_for_compound_type(vertex_material))
#include <generated/do_editor_ui_for_compound_type_vertex_material.h>

poof(do_editor_ui_for_compound_type(bonsai_futex))
#include <generated/do_editor_ui_for_compound_type_bonsai_futex.h>

poof(do_editor_ui_for_compound_type(untextured_3d_geometry_buffer))
#include <generated/do_editor_ui_for_compound_type_untextured_3d_geometry_buffer.h>

link_internal void
DoEditorUi(renderer_2d *Ui, geo_u3d **ElementP, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (ElementP)
  {
    DoEditorUi(Ui, *ElementP, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  }
  else
  {
    PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}

poof(do_editor_ui_for_enum(chunk_flag))
#include <generated/do_editor_ui_for_enum_chunk_flag.h>

poof(do_editor_ui_for_compound_type(voxel))
#include <generated/do_editor_ui_for_compound_type_voxel.h>

poof(do_editor_ui_for_compound_type(voxel_lighting))
#include <generated/do_editor_ui_for_compound_type_voxel_lighting.h>

poof(do_editor_ui_for_compound_type(chunk_data))
#include <generated/do_editor_ui_for_compound_type_chunk_data.h>

poof(do_editor_ui_for_compound_type(vox_data))
#include <generated/do_editor_ui_for_compound_type_vox_data.h>

poof(do_editor_ui_for_compound_type(gpu_element_buffer_handles))
#include <generated/do_editor_ui_for_compound_type_gpu_element_buffer_handles.h>

poof(do_editor_ui_for_compound_type(lod_element_buffer))
#include <generated/do_editor_ui_for_compound_type_lod_element_buffer.h>

poof(do_editor_ui_for_compound_type(model))
#include <generated/do_editor_ui_for_compound_type_model.h>

poof(do_editor_ui_for_container(model_buffer))
#include <generated/do_editor_ui_for_container_model_buffer.h>

poof(do_editor_ui_for_enum(file_traversal_type))
#include <generated/do_editor_ui_for_enum_file_traversal_type.h>

poof(do_editor_ui_for_compound_type(file_traversal_node))
#include <generated/do_editor_ui_for_compound_type_file_traversal_node.h>

// NOTE(Jesse): Had to hack this slightly because the asset_load_state on Enitity is marked volatile
/* poof(do_editor_ui_for_enum(asset_load_state)) */
#include <generated/do_editor_ui_for_enum_asset_load_state.h>

poof(do_editor_ui_for_compound_type(asset_id))
#include <generated/do_editor_ui_for_compound_type_asset_id.h>

poof(do_editor_ui_for_compound_type(asset))
#include <generated/do_editor_ui_for_compound_type_asset.h>

poof(do_editor_ui_for_compound_type(collision_event))
#include <generated/do_editor_ui_for_compound_type_collision_event.h>

poof(do_editor_ui_for_compound_type(entity_position_info))
#include <generated/do_editor_ui_for_compound_type_entity_position_info.h>

poof(do_editor_ui_for_compound_type(entity))
#include <generated/do_editor_ui_for_compound_type_entity.h>

link_internal void
DoEditorUi(renderer_2d *Ui, void *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
  Value ?
    PushColumn(Ui, FSz("0x%x",umm(Value)), EDITOR_UI_FUNCTION_INSTANCE_NAMES) :
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  PushNewRow(Ui);
}


poof(do_editor_ui_for_container(entity_ptr_block_array))
#include <generated/do_editor_ui_for_container_entity_ptr_block_array.h>

poof(do_editor_ui_for_compound_type(world_chunk))
#include <generated/do_editor_ui_for_compound_type_world_chunk.h>

poof(do_editor_ui_for_compound_type(render_debug))
#include <generated/do_editor_ui_for_compound_type_render_debug.h>

poof(do_editor_ui_for_compound_type(ui_debug))
#include <generated/do_editor_ui_for_compound_type_ui_debug.h>

poof(do_editor_ui_for_compound_type(engine_debug))
#include <generated/do_editor_ui_for_compound_type_engine_debug.h>



link_internal rect3i
ModifySelectionAABB(rect3 *SelectionRegion, v3i UpdateVector, face_index Face, selection_mode Mode)
{
  rect3i Result = Rect3i(SelectionRegion);
  Assert(SelectionRegion->Min <= SelectionRegion->Max);

  switch (Mode)
  {
    InvalidCase(SelectionMode_Noop);

    case SelectionMode_Resize:
    {
      switch (Face)
      {
        InvalidCase(FaceIndex_None);

        case FaceIndex_Top:
        case FaceIndex_Right:
        case FaceIndex_Front:
        {
          Result.Max += UpdateVector;
        } break;

        case FaceIndex_Bot:
        case FaceIndex_Back:
        case FaceIndex_Left:
        {
          Result.Min += UpdateVector;
        } break;
      }
    } break;

    case SelectionMode_TranslateLinear:
    case SelectionMode_TranslatePlanar:
    {
      Result.Max += UpdateVector;
      Result.Min += UpdateVector;
    } break;
  }

  auto MinP = Min(Result.Min, Result.Max);
  auto MaxP = Max(Result.Min, Result.Max);

  Result.Min = MinP;
  Result.Max = MaxP;
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
DoDeleteRegion(engine_resources *Engine, rect3 *AABB)
{
  world_update_op_shape Shape = {
    .Type = type_world_update_op_shape_params_rect,
    .world_update_op_shape_params_rect.P0 = AABB->Min,
    .world_update_op_shape_params_rect.P1 = AABB->Max,
  };
  QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Subtractive, &Shape, Engine->Editor.SelectedColorIndex, Engine->Memory);
}


link_internal v3
ConstrainUpdateVector(v3 UpdateVector, face_index Face, selection_mode SelectionMode)
{
  v3 Result = UpdateVector;

  switch (SelectionMode)
  {
    case SelectionMode_Noop: { } break;

    case SelectionMode_TranslateLinear:
    case SelectionMode_Resize:
    {
      v3 Constraint = Abs(NormalForFace(Face));
      Result *= Constraint;
    } break;

    case SelectionMode_TranslatePlanar:
    {
      v3 Constraint = Abs(NormalForFace(Face)-1.f);
      Result *= Constraint;
    } break;
  }

  return Result;
}

link_internal void
HighlightFace(engine_resources *Engine, face_index Face, aabb SelectionAABB, r32 InsetWidth, u8 HiColor, r32 HiThickness)
{
  v3 SelectionAABBRadius = GetRadius(&SelectionAABB);
  switch (Face)
  {
    InvalidCase(FaceIndex_None);

    case FaceIndex_Top:
    {
      v3 HighlightInset = V3(InsetWidth, InsetWidth, 0.f);
      v3 MinHiP = SelectionAABB.Min + (SelectionAABBRadius*V3(0.f, 0.f, 2.f)) + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
    } break;

    case FaceIndex_Bot:
    {
      v3 HighlightInset = V3(InsetWidth, InsetWidth, 0.f);
      v3 MinHiP = SelectionAABB.Min + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - (SelectionAABBRadius*V3(0.f, 0.f, 2.f)) - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
    } break;

    case FaceIndex_Left:
    {
      v3 HighlightInset = V3(0.f, InsetWidth, InsetWidth);
      v3 MinHiP = SelectionAABB.Min + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - (SelectionAABBRadius*V3(2.f, 0.f, 0.f)) - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
    } break;

    case FaceIndex_Right:
    {
      v3 HighlightInset = V3(0.f, InsetWidth, InsetWidth);
      v3 MinHiP = SelectionAABB.Min + (SelectionAABBRadius*V3(2.f, 0.f, 0.f)) + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
    } break;

    case FaceIndex_Back:
    {
      v3 HighlightInset = V3(InsetWidth, 0.f, InsetWidth);
      v3 MinHiP = SelectionAABB.Min + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - (SelectionAABBRadius*V3(0.f, 2.f, 0.f)) - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
    } break;

    case FaceIndex_Front:
    {
      v3 HighlightInset = V3(InsetWidth, 0.f, InsetWidth);
      v3 MinHiP = SelectionAABB.Min + (SelectionAABBRadius*V3(0.f, 2.f, 0.f)) + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, HiColor, HiThickness );
    } break;
  }

}

link_internal rect3i
DoSelectonModification( engine_resources *Engine,
                        ray *MouseRay,
                        selection_mode SelectionMode,
                        selection_modification_state *SelectionState,
                        aabb SelectionAABB )
{
  UNPACK_ENGINE_RESOURCES(Engine);
  rect3i Result  = Rect3i(SelectionAABB);

  v3 Normal = {};
  v3 PlaneN = {};

  switch (SelectionMode)
  {
    case SelectionMode_Noop: { return Result; }

    case SelectionMode_TranslateLinear:
    case SelectionMode_Resize:
    {
      Normal   = NormalForFace(SelectionState->ClickedFace);
      v3 PerpN = Cross(Normal, Camera->Front);
      PlaneN   = Cross(Normal, PerpN);
    } break;

    case SelectionMode_TranslatePlanar:
    {
      Normal = NormalForFace(SelectionState->ClickedFace);
      PlaneN = Normal;
    } break;
  }

  f32 tRay = {};
  if (Intersect(PlaneN, SelectionState->ClickedP[0], MouseRay->Origin, MouseRay->Dir, &tRay))
  {
    v3 PlaneIntersect = MouseRay->Origin + (MouseRay->Dir*tRay);
    DEBUG_HighlightVoxel(Engine, PlaneIntersect, RED);
    if (Input->LMB.Pressed)
    {
      SelectionState->ClickedP[1] = PlaneIntersect;
    }
  }


  v3 RoughUpdateVector = (SelectionState->ClickedP[1] - SelectionState->ClickedP[0]);

  v3 UpdateVector = ConstrainUpdateVector(RoughUpdateVector, SelectionState->ClickedFace, SelectionMode);
          Result  = ModifySelectionAABB(&SelectionAABB, V3i(UpdateVector), SelectionState->ClickedFace, SelectionMode);

  {
    DEBUG_HighlightVoxel(Engine, SelectionState->ClickedP[0], RED);
    DEBUG_HighlightVoxel(Engine, SelectionState->ClickedP[1], BLUE);
    DEBUG_DrawSimSpaceVectorAt(Engine, SelectionState->ClickedP[0], UpdateVector, GREEN);
  }

  // Draw selection modification region
  //
  rect3 Draw = Rect3(&Result);
  DEBUG_DrawSimSpaceAABB(Engine, &Draw, GREEN, 0.1f);

  Assert(Result.Min <= Result.Max);

  return Result;
}

link_internal void
DoLevelEditor(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  /* v2 WindowDim = {{325.f, 1200.f}}; */
  local_persist window_layout Window = WindowLayout("World Edit");

  PushWindowStart(Ui, &Window);

  ui_toggle_button_group WorldEditModeRadioGroup = RadioButtonGroup_world_edit_mode(Ui, umm("world_edit_mode_radio_group"), ToggleButtonGroupFlags_DrawVertical, {}, {}, {}, &DefaultStyle, V4(0, 0, 0, 16));

  v3_cursor *Palette = GetColorPalette();
  s32 PaletteColors = s32(AtElements(Palette));
  /* Info("Global_ColorPaletteAt %d", Global_ColorPaletteAt); */
  ui_element_reference ColorTable = PushTableStart(Ui);
    RangeIterator_t(u16, ColorIndex, PaletteColors)
    {
      v3 Color = GetColorData(u32(ColorIndex));
      ui_style Style = FlatUiStyle(Color);

      v2 QuadDim = V2(22);
      v4 Padding = V4(1);
      v3 BorderColor = V3(1.0f);

      if (Engine->Editor.SelectedColorIndex == ColorIndex)
      {
        f32 BorderDim = 2.f;
        PushRelativeBorder(Ui, QuadDim, BorderColor, V4(BorderDim));
      }

      if (Engine->Editor.HoverColorIndex == ColorIndex)
      {
        f32 BorderDim = 1.f;
        PushRelativeBorder(Ui, QuadDim, BorderColor, V4(BorderDim));
      }


      interactable_handle ColorPickerButton = PushButtonStart(Ui, (umm)"ColorPicker" ^ (umm)(ColorIndex+12657674));
        PushUntexturedQuad(Ui, {}, QuadDim, zDepth_Text, &Style, Padding );
      PushButtonEnd(Ui);


      if (Hover(Ui, &ColorPickerButton))
      {
        f32 BorderDim = 1.f;
        PushRelativeBorder(Ui, V2(-1.f,1.f)*QuadDim, BorderColor, V4(BorderDim));

        PushTooltip(Ui, FSz("%d (%.2f, %.2f, %.2f)", ColorIndex, r64(Color.x), r64(Color.y), r64(Color.z)) );
      }

      if (Clicked(Ui, &ColorPickerButton))
      {
        Engine->Editor.SelectedColorIndex  = ColorIndex;
      }


      if ( (ColorIndex+1) % 8 == 0 ) { PushNewRow(Ui); }
    }
  PushTableEnd(Ui);


  rect3 SelectionAABB = {};
  aabb_intersect_result AABBTest = {};

  if (Editor->SelectionClicks)
  {
    r32 Thickness = 0.10f;

    if (SelectionIncomplete(Editor->SelectionClicks))
    {
      if (Engine->MousedOverVoxel.Tag)
      {
        auto MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value);
        MouseP.Offset = Floor(MouseP.Offset);

        cp MinP = Min(Editor->SelectionBase, MouseP);
        cp MaxP = Max(Editor->SelectionBase, MouseP) + V3(1.f);
        /* Assert(MinP <= MaxP); */
        Editor->SelectionRegion = RectMinMax(MinP, MaxP);
      }
    }
    else { Thickness = 0.20f; }

    {
      v3 SelectionMinP = GetSimSpaceP(World, Editor->SelectionRegion.Min);
      v3 SelectionMaxP = GetSimSpaceP(World, Editor->SelectionRegion.Max);
      SelectionAABB = AABBMinMax(SelectionMinP, SelectionMaxP);
    }

    maybe_ray MaybeRay = ComputeRayFromCursor(Engine, &gBuffer->ViewProjection, Camera, World->ChunkDim);
    if (MaybeRay.Tag == Maybe_Yes)
    {
      ray Ray = MaybeRay.Ray;

      /* Ray.Origin = GetSimSpaceP(World, Canonical_Position(World->ChunkDim, Ray.Origin, {})); */
      AABBTest = Intersect(SelectionAABB, &Ray);

      face_index Face = AABBTest.Face;
      /* PushColumn(Ui, CS(Face)); */
      /* PushNewRow(Ui); */

      if (Face)
      {
        /* r32 InsetWidth = 0.25f; */
        r32 InsetWidth  = 0.f;
        u8  HiColor     = GREEN;
        r32 HiThickness = Thickness*1.2f;

        HighlightFace(Engine, Face, SelectionAABB, InsetWidth, HiColor, HiThickness);

        if ( Input->LMB.Clicked && (Input->Ctrl.Pressed || Input->Shift.Pressed) )
        {
          v3 PlaneBaseP = Ray.Origin + (AABBTest.t*Ray.Dir);
          Editor->Selection.ClickedFace = Face;
          Editor->Selection.ClickedP[0] = PlaneBaseP;
        }

      }

      if (Editor->Selection.ClickedFace)
      {
        selection_mode SelectionMode = {};
        if (Input->Shift.Pressed && Input->Ctrl.Pressed)
        {
          SelectionMode = SelectionMode_TranslateLinear;
        }
        else if (Input->Shift.Pressed)
        {
          SelectionMode = SelectionMode_Resize;
        }
        else if (Input->Ctrl.Pressed)
        {
          SelectionMode =  SelectionMode_TranslatePlanar;
        }

        /* if (SelectionMode) { Ui->RequestedForceCapture = True; } */

        rect3i ModifiedSelection = DoSelectonModification(Engine, &Ray, SelectionMode, &Editor->Selection, SelectionAABB);

        if (!Input->LMB.Pressed)
        {
          // Make ModifiedSelection permanent
          Editor->SelectionRegion = SimSpaceToCanonical(World, &ModifiedSelection);
          Editor->Selection.ClickedFace = FaceIndex_None;
        }
      }
    }

    // Draw selection box
    //

    u8 BaseColor = WHITE;
    /* v3 P0 = GetRenderP(Engine, Editor->SelectionRegion.Min); */
    /* v3 P1 = GetRenderP(Engine, Editor->SelectionRegion.Max); */
    /* DEBUG_DrawAABB(Engine, P0, P1, BaseColor, Thickness); */

    DEBUG_DrawSimSpaceAABB(Engine, &SelectionAABB, BaseColor, Thickness);
  }


  {
    /* DoEditorUi(Ui, &Editor->SelectionRegion, "SelectionRegion", &DefaultStyle, {}, {}); */
    /* DoEditorUi(Ui, &World->Center,           "WorldCenter",     &DefaultStyle, {}, {}); */
    /* Info("%S", ToString(WorldEditMode)); */
  }


  world_edit_mode WorldEditMode = {};
  GetRadioEnum(&WorldEditModeRadioGroup, &WorldEditMode);
  picked_voxel_position HighlightVoxel = PickedVoxel_FirstFilled;

  if (!UiCapturedMouseInput(Ui))
  {
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
                auto MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value);
                MouseP.Offset = Floor(MouseP.Offset);
                Editor->SelectionBase = MouseP;
              }
            } break;

            case 1:
            {
              Editor->SelectionClicks += 1;
            } break;

          }
        }
      } break;

      case WorldEditMode_Eyedropper:
      {
        if (Engine->MousedOverVoxel.Tag)
        {
          auto MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value);
          voxel *V = GetVoxelPointer(&Engine->MousedOverVoxel.Value, PickedVoxel_FirstFilled);

          Engine->Editor.HoverColorIndex = V->Color;

          if (Input->LMB.Clicked)
          {
            Info("Selecting Color (%S)", CS(V->Color));
            Engine->Editor.SelectedColorIndex = V->Color;
          }
        }
        else
        {
          Engine->Editor.HoverColorIndex = INVALID_COLOR_INDEX;
        }
      } break;

      case WorldEditMode_PaintSelection:
      {
        if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
        {
          world_update_op_shape Shape = {
            .Type = type_world_update_op_shape_params_rect,
            .world_update_op_shape_params_rect.P0 = SelectionAABB.Min,
            .world_update_op_shape_params_rect.P1 = SelectionAABB.Max,
          };
          QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Paint, &Shape, Editor->SelectedColorIndex, Engine->Memory);
        }
      } break;

      case WorldEditMode_FillSelection:
      {
        if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
        {
          world_update_op_shape Shape = {
            .Type = type_world_update_op_shape_params_rect,
            .world_update_op_shape_params_rect.P0 = SelectionAABB.Min,
            .world_update_op_shape_params_rect.P1 = SelectionAABB.Max,
          };
          QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Additive, &Shape, Editor->SelectedColorIndex, Engine->Memory);
        }
      } break;

      case WorldEditMode_DeleteSelection:
      {
        if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
        {
          DoDeleteRegion(Engine, &SelectionAABB);
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

            world_update_op_shape Shape = {
              .Type = type_world_update_op_shape_params_rect,
              .world_update_op_shape_params_rect.P0 = P0,
              .world_update_op_shape_params_rect.P1 = P0+1,
            };
            QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Additive, &Shape, Editor->SelectedColorIndex, Engine->Memory);
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

            world_update_op_shape Shape = {
              .Type = type_world_update_op_shape_params_rect,
              .world_update_op_shape_params_rect.P0 = P0,
              .world_update_op_shape_params_rect.P1 = P0+1,
            };
            QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Subtractive, &Shape, Editor->SelectedColorIndex, Engine->Memory);
          }
        }
      } break;

      case WorldEditMode_PaintSingle:
      {
        if (Input->LMB.Pressed)
        {
          if (Engine->MousedOverVoxel.Tag)
          {
            Ui->RequestedForceCapture = True;;
            voxel *V = GetVoxelPointer(&Engine->MousedOverVoxel.Value, PickedVoxel_FirstFilled);

            if (V)
            {
              V->Color = Engine->Editor.SelectedColorIndex;
              QueueChunkForMeshRebuild(&Plat->LowPriority, Engine->MousedOverVoxel.Value.Chunks[PickedVoxel_FirstFilled].Chunk);
            }
          }
        }
      } break;

      case WorldEditMode_BlitEntity:
      {
        entity *SelectedEntity = EngineDebug->SelectedEntity;
        if (SelectedEntity)
        {
          aabb EntityAABB = GetSimSpaceAABB(World, SelectedEntity);
          if (Engine->MaybeMouseRay.Tag)
          {
            ray *Ray = &Engine->MaybeMouseRay.Ray;
            aabb_intersect_result IntersectionResult = Intersect(EntityAABB, Ray);
            if (Input->LMB.Clicked && IntersectionResult.Face)
            {
#if 1
              world_update_op_shape_params_asset AssetUpdateShape =
              {
                SelectedEntity->AssetId,
                SelectedEntity->ModelIndex,
                SelectedEntity->P,
              };

              world_update_op_shape Shape =
              {
                type_world_update_op_shape_params_asset,
                .world_update_op_shape_params_asset = AssetUpdateShape,
              };
              QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Additive, &Shape, {}, World->Memory);
#endif
            }
          }
        }
      } break;

      case WorldEditMode_RecomputeStandingSpots:
      {
        if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
        {
          world_update_op_shape Shape = {
            .Type = type_world_update_op_shape_params_rect,
            .world_update_op_shape_params_rect.P0 = SelectionAABB.Min,
            .world_update_op_shape_params_rect.P1 = SelectionAABB.Max,
          };
          QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_RecomputeStandingSpots, &Shape, Editor->SelectedColorIndex, Engine->Memory);
        }
      } break;
    }

  }

  PushWindowEnd(Ui, &Window);



  if (Input->Ctrl.Pressed || Input->Shift.Pressed) { Ui->RequestedForceCapture = True; }

  if (Input->Ctrl.Pressed && Input->S.Clicked) { RadioSelect(&WorldEditModeRadioGroup, WorldEditMode_Select); ResetSelection(Editor); }

  if (Clicked(&WorldEditModeRadioGroup, CSz("Select"))) { ResetSelection(Editor); }

  if (Input->Ctrl.Pressed && Input->F.Clicked) { ResetSelectionIfIncomplete(Editor); RadioSelect(&WorldEditModeRadioGroup, WorldEditMode_FillSelection); }

  if (Input->Ctrl.Pressed && Input->E.Clicked) { ResetSelectionIfIncomplete(Editor); RadioSelect(&WorldEditModeRadioGroup, WorldEditMode_Eyedropper); }

  if (Editor->SelectionClicks == 2)
  {
    if (Input->Ctrl.Pressed && Input->D.Clicked) { DoDeleteRegion(Engine, &SelectionAABB); }

    if (Input->Ctrl.Pressed && Input->C.Clicked) { Editor->CopyRegion = Editor->SelectionRegion; }

    if (Input->Ctrl.Pressed && Input->V.Clicked)
    {
      v3 CopyDim = GetDim(World, Editor->CopyRegion);
      s32 VoxelCount = s32(Volume(CopyDim));
      Leak("voxel *V = Allocate(voxel, Engine->Memory, VoxelCount)");
      voxel *V = Allocate(voxel, Engine->Memory, VoxelCount);

      Leak("");
      world_chunk_ptr_buffer Chunks = GatherChunksOverlappingArea(World, Editor->CopyRegion, Engine->Memory);
      GatherVoxelsOverlappingArea(World, GetSimSpaceRect3i(World, Editor->CopyRegion), &Chunks, V, VoxelCount);

      chunk_data D = {};
      D.Dim = V3i(CopyDim);
      D.Voxels = V;

      world_update_op_shape_params_chunk_data ChunkDataShape = { D, GetSimSpaceP(World, Editor->SelectionRegion.Min) };

      world_update_op_shape Shape =
      {
        type_world_update_op_shape_params_chunk_data,
        .world_update_op_shape_params_chunk_data = ChunkDataShape,
      };
      QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_Additive, &Shape, {}, World->Memory);
    }
  }


  //
  // Highlight moused over voxel
  //
  if (Engine->MousedOverVoxel.Tag)
  {
#if 0
    {
      v3 SimP = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, HighlightVoxel));
      DEBUG_HighlightVoxel( Engine, SimP, RED, 0.075f);

      voxel *V = GetVoxelPointer(&Engine->MousedOverVoxel.Value, PickedVoxel_FirstFilled);


      local_persist window_layout Window = WindowLayout("DEBUG DELETE ME", V2(150.f, 150.f));
      PushWindowStart(Ui, &Window);

      v3 Tangent, Bitangent, Normal;
      CalculateTBN(V->Derivs, &Tangent, &Bitangent, &Normal);

      /* DEBUG_DrawSimSpaceVectorAt(Engine, SimP+V3(0,0,5), Normalize(Tangent)*20.f,     RED, 0.75f); */
      /* DEBUG_DrawSimSpaceVectorAt(Engine, SimP+V3(0,0,5), Normalize(Bitangent)*20.f, GREEN, 0.75f); */
      /* DEBUG_DrawSimSpaceVectorAt(Engine, SimP+V3(0,0,5), Normalize(Normal)*20.f,     BLUE, 0.75f); */

      /* DEBUG_DrawSimSpaceVectorAt(Engine, SimP, Normalize(tangent)*20.f,     RED, 0.75f); */
      /* DEBUG_DrawSimSpaceVectorAt(Engine, SimP, Normalize(bitangent)*20.f, GREEN, 0.75f); */
      /* DEBUG_DrawSimSpaceVectorAt(Engine, SimP, Normalize(normal)*20.f,     BLUE, 0.75f); */


      DEBUG_DrawSimSpaceVectorAt(Engine, SimP, Normalize(V->DebugColor)* 40.f,  GREEN, 0.75f);
      DEBUG_DrawSimSpaceVectorAt(Engine, SimP, Normalize(V->DebugColor)*-40.f, YELLOW, 0.75f);
      /* DEBUG_DrawSimSpaceVectorAt(Engine, SimP, Normalize(V->Derivs)*-20.f, YELLOW, 0.75f); */

      DoEditorUi(Ui, &V->Derivs, "derivs");
      DoEditorUi(Ui, &V->DebugColor, "DebugColor");

      r32 DotP = Dot(V->DebugColor, V3(0,0,1));
      DoEditorUi(Ui, &DotP, "Dot");

      v3 AbsP = GetAbsoluteP(&Engine->MousedOverVoxel.Value);
      DoEditorUi(Ui, &AbsP, "AbsP");

      PushWindowEnd(Ui, &Window);

    }
#else
    {
      v3 SimP = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, PickedVoxel_FirstFilled));
      DEBUG_HighlightVoxel( Engine, SimP, RED, 0.075f);
    }
    /* { */
    /*   v3 SimP = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, PickedVoxel_LastEmpty)); */
    /*   DebugLine("%f %f %f", SimP.x, SimP.y, SimP.z); */
    /*   DEBUG_HighlightVoxel( Engine, SimP, BLUE, 0.075f); */
    /* } */
#endif
  }
}
