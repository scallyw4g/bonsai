
link_internal b32
InitEditor(level_editor *Editor)
{
  b32 Result = True;
  Editor->Memory = AllocateArena();

  Editor->AssetThumbnails.Memory = Editor->Memory;

  {
    Editor->NoiseEditor.PreviewThumbnail.Texture = MakeTexture_RGB(V2i(512), 0, CSz("NoisePreviewTexture"));
    StandardCamera(&Editor->NoiseEditor.PreviewThumbnail.Camera, 10000.f, 1000.f, 30.f);
  }

  return Result;
}

link_internal b32
HardResetEditor(level_editor *Editor)
{
  DeleteTexture(&Editor->NoiseEditor.PreviewThumbnail.Texture);

  IterateOver(&Editor->AssetThumbnails, Thumb, Index)
  {
    DeleteTexture(&Thumb->Texture);
  }

  // @editor_chunk_memory_question
  //
  // I guess here we can actually just not worry about clearing it because
  // it'll get realloc'd in the update path ..?  Seems fine..
  /* DeallocateWorldChunk(&Editor->NoiseEditor.Chunk, &GetEngineResources()->MeshFreelist); */

  VaporizeArena(Editor->Memory);

  // NOTE(Jesse): There are some default values in the perlin params that we want to reset to
  *Editor = {};
  /* Clear(Editor); */

  b32 Result = InitEditor(Editor);
  return Result;
}

poof(block_array_c(asset_thumbnail, {8}))
#include <generated/block_array_c_asset_thumbnail_688856411.h>

link_weak ui_debug *
GetUiDebug()
{
  Assert(Global_EngineResources);
  return &Global_EngineResources->EngineDebug.UiDebug;
}

link_internal void
DebugSlider(renderer_2d *Ui, window_layout *Window, r32 *Value, cs Name, r32 Min, r32 Max, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Generic);
    PushTableStart(Ui);
      if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

      auto Range = Max-Min;
      r32 PercFilled = ((*Value)-Min)/Range;

      r32 Width = 100.f;

      if (Value)
      {
        cs ValueText = CS(*Value);
        v2 TextDim = GetDim(GetDrawBounds(ValueText, &DefaultStyle));

        v2 Offset = V2(Width/2.f-TextDim.x/2.f, 0.f);

        Text(Ui, ValueText, &DefaultStyle, TextRenderParam_NoAdvanceLayout, Offset);
      }

      interactable_handle BargraphButton = PushButtonStart(Ui, UiId(Window, "debug_slider", Value));
        PushSliderBar(Ui, PercFilled, UI_WINDOW_BEZEL_DEFAULT_COLOR_SATURATED, UI_WINDOW_BEZEL_DEFAULT_COLOR_MUTED, Width); // Value marker
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
DoEditorUi(renderer_2d *Ui, window_layout *Window, r32 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, Name, &DefaultUiRenderParams_Blank); }

  u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
      if (Value)
      {
        if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value))) { *Value = *Value - 1.f; }
          DebugSlider(Ui, Window, Value, {}, MinValue, MaxValue);
        if (Button(Ui, CSz("+"), UiId(Window, "increment", Value))) { *Value = *Value + 1.f; }
      }
      else
      {
        PushColumn(Ui, CSz("(null)"));
      }
    PushTableEnd(Ui);
  EndColumn(Ui, Start);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, b8 *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Checkbox)
{
  UNPACK_UI_RENDER_PARAMS(Params);

  interactable_handle ButtonHandle = PushButtonStart(Ui, UiId(Window, "toggle", Value), Style);

    PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Generic);

    if (Value)
    {
      if (*Value)
      {
        PushUntexturedQuad(Ui, V2(2.f, 2.f), V2(Params->Style->Font.Size.y)-4.f, zDepth_Border, &Global_DefaultCheckboxForeground, DefaultCheckboxPadding, QuadRenderParam_NoAdvance );
      }
    }
    else
    {
      PushColumn(Ui, CSz("(null)"), Params);
    }

    PushUntexturedQuad(Ui, {}, V2(Params->Style->Font.Size.y), zDepth_Text, &Global_DefaultCheckboxBackground, DefaultCheckboxPadding, QuadRenderParam_Default );



  PushButtonEnd(Ui);

  if (Clicked(Ui, &ButtonHandle))
   { *Value = !(*Value); }
}


poof(do_editor_ui_for_primitive_type({s64 u64 s32 u32 s16 u16 s8 u8}));
#include <generated/do_editor_ui_for_scalar_type_688724926.h>

poof(do_editor_ui_for_vector_type({v4i v4 v3i v3 v2i v2 Quaternion}));
#include <generated/do_editor_ui_for_vector_type_688873645.h>



poof(do_editor_ui_for_compound_type(perlin_noise_params))
#include <generated/do_editor_ui_for_compound_type_perlin_noise_params.h>
poof(do_editor_ui_for_compound_type(voronoi_noise_params))
#include <generated/do_editor_ui_for_compound_type_voronoi_noise_params.h>



poof(do_editor_ui_for_container(v3_cursor))
#include <generated/do_editor_ui_for_container_v3_cursor.h>

poof(do_editor_ui_for_compound_type(ui_id))
#include <generated/do_editor_ui_for_compound_type_ui_id.h>

poof(do_editor_ui_for_compound_type(ui_toggle))
#include <generated/do_editor_ui_for_compound_type_ui_toggle.h>

link_internal cs
CS(ui_toggle_hashtable_iterator &Iter)
{
  return CSz("(iterator)");
}

poof(do_editor_ui_for_container(ui_toggle_hashtable))
#include <generated/do_editor_ui_for_container_ui_toggle_hashtable.h>

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, cs *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  Value ?
    PushColumn(Ui, *Value, EDITOR_UI_FUNCTION_INSTANCE_NAMES) :
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, cp *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, &Value->WorldP, CSz("WorldP"));
  DoEditorUi(Ui, Window, &Value->Offset, CSz("Offset"));
}

#if DO_EDITOR_UI_FOR_ENTITY_TYPE
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_ARGUMENTS);
#endif

poof(string_and_value_tables(particle_spawn_type))
#include <generated/string_and_value_tables_particle_spawn_type.h>

poof(string_and_value_tables(file_traversal_type))
#include <generated/string_and_value_tables_file_traversal_node.h>

poof(string_and_value_tables(tone_mapping_type))
#include <generated/string_and_value_tables_tone_mapping_type.h>

poof(string_and_value_tables(engine_debug_view_mode))
#include <generated/string_and_value_tables_engine_debug_view_mode.h>


poof(do_editor_ui_for_enum(entity_state))
#include <generated/do_editor_ui_for_enum_entity_state.h>

poof(do_editor_ui_for_enum(entity_behavior_flags))
#include <generated/do_editor_ui_for_enum_entity_behavior_flags.h>

poof(do_editor_ui_for_enum(tone_mapping_type))
#include <generated/do_editor_ui_for_enum_tone_mapping_type.h>



poof(do_editor_ui_for_compound_type(world))
#include <generated/do_editor_ui_for_compound_type_world.h>

poof(do_editor_ui_for_compound_type(lighting_settings))
#include <generated/do_editor_ui_for_compound_type_lighting_settings.h>

poof(do_editor_ui_for_compound_type(render_settings))
#include <generated/do_editor_ui_for_compound_type_render_settings.h>

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
DoEditorUi(renderer_2d *Ui, window_layout *Window, geo_u3d **ElementP, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (ElementP)
  {
    DoEditorUi(Ui, Window, *ElementP, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

// @dirty_entity_P_format_hack
//
link_internal void
DoEditorUi_entity_P(renderer_2d *Ui, window_layout *Window, entity *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Column)
{
  DoEditorUi(Ui, Window, &Element->P.WorldP, CSz("WorldP"), Params);
  DoEditorUi(Ui, Window, &Element->P.Offset, CSz("Offset"), Params, 0.f, 32.f);
}

poof(do_editor_ui_for_compound_type(entity_id))
#include <generated/do_editor_ui_for_compound_type_entity_id.h>

poof(do_editor_ui_for_compound_type(entity))
#include <generated/do_editor_ui_for_compound_type_entity.h>

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, void *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Column)
{
  if (Name) { PushColumn(Ui, CS(Name), Params); }
  Value ?
    PushColumn(Ui, FSz("0x%x",umm(Value)), &DefaultUiRenderParams_Column) :
    PushColumn(Ui, CSz("(null)"), &DefaultUiRenderParams_Column);
  PushNewRow(Ui);
}


poof(do_editor_ui_for_container(entity_ptr_block_array))
#include <generated/do_editor_ui_for_container_entity_ptr_block_array.h>

poof(do_editor_ui_for_compound_type(world_chunk))
#include <generated/do_editor_ui_for_compound_type_world_chunk.h>


poof(do_editor_ui_for_compound_type(plane))
#include <generated/do_editor_ui_for_compound_type_plane.h>

poof(do_editor_ui_for_compound_type(frustum))
#include <generated/do_editor_ui_for_compound_type_frustum.h>

poof(do_editor_ui_for_compound_type(camera))
#include <generated/do_editor_ui_for_compound_type_camera.h>

poof(do_editor_ui_for_compound_type(texture))
#include <generated/do_editor_ui_for_compound_type_texture.h>

poof(do_editor_ui_for_compound_type(asset_thumbnail))
#include <generated/do_editor_ui_for_compound_type_asset_thumbnail.h>

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shader_uniform *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %s", Element->Name), FSz("> %s", Element->Name), UiId(Window, "toggle shader_uniform", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      DoEditorUi(Ui, Window, &Element->Type, CSz("shader_uniform_type Type"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      switch (Element->Type)
      {
        InvalidCase(ShaderUniform_Undefined);

        case ShaderUniform_M4:        { DoEditorUi(Ui, Window, Element->M4, CSz("M4"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_V2:        { DoEditorUi(Ui, Window, Element->V2, CSz("V2"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_V3:        { DoEditorUi(Ui, Window, Element->V3, CSz("V3"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_S32:       { DoEditorUi(Ui, Window, Element->S32, CSz("S32"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_U32:       { DoEditorUi(Ui, Window, Element->U32, CSz("U32"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_R32:       { DoEditorUi(Ui, Window, Element->R32, CSz("R32"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_Texture:   { DoEditorUi(Ui, Window, Element->Texture, CSz("Texture"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_Light:     { DoEditorUi(Ui, Window, Element->Light, CSz("Light"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_Camera:    { DoEditorUi(Ui, Window, Element->Camera, CSz("Camera"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
      }

      DoEditorUi(Ui, Window, &Element->ID, CSz("s32 ID"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushTableEnd(Ui);
      PushNewRow(Ui);

      DoEditorUi(Ui, Window, Element->Next, CSz("shader_uniform Next"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    }
  }
  else
  {
    PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}

poof(do_editor_ui_for_compound_type(shader))
#include <generated/do_editor_ui_for_compound_type_shader.h>

poof(do_editor_ui_for_compound_type(render_buffers_2d))
#include <generated/do_editor_ui_for_compound_type_render_buffers_2d.h>

poof(do_editor_ui_for_compound_type(renderer_2d))
#include <generated/do_editor_ui_for_compound_type_renderer_2d.h>

poof(do_editor_ui_for_compound_type(game_lights))
#include <generated/do_editor_ui_for_compound_type_game_lights.h>

poof(do_editor_ui_for_compound_type(lighting_render_group))
#include <generated/do_editor_ui_for_compound_type_lighting_render_group.h>

poof(do_editor_ui_for_compound_type(graphics))
#include <generated/do_editor_ui_for_compound_type_graphics.h>

poof(do_editor_ui_for_compound_type(render_debug))
#include <generated/do_editor_ui_for_compound_type_render_debug.h>

poof(do_editor_ui_for_compound_type(ui_debug))
#include <generated/do_editor_ui_for_compound_type_ui_debug.h>

poof(do_editor_ui_for_compound_type(engine_debug))
#include <generated/do_editor_ui_for_compound_type_engine_debug.h>

poof(do_editor_ui_for_enum(engine_debug_view_mode))
#include <generated/do_editor_ui_for_enum_engine_debug_view_mode.h>

poof(do_editor_ui_for_compound_type(level_editor))
#include <generated/do_editor_ui_for_compound_type_level_editor.h>

poof(do_editor_ui_for_enum(resolution_setting))
#include <generated/do_editor_ui_for_enum_resolution_setting.h>

poof(do_editor_ui_for_enum(shader_language_setting))
#include <generated/do_editor_ui_for_enum_shader_language_setting.h>

poof(do_editor_ui_for_compound_type(graphics_settings))
#include <generated/do_editor_ui_for_compound_type_graphics_settings.h>

poof(do_editor_ui_for_compound_type(engine_settings))
#include <generated/do_editor_ui_for_compound_type_engine_settings.h>

poof(do_editor_ui_for_compound_type(engine_resources))
#include <generated/do_editor_ui_for_compound_type_engine_resources.h>


link_internal rect3i
ModifySelectionAABB(rect3 *SelectionRegion, v3i UpdateVector, face_index Face, world_edit_selection_mode Mode)
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
ApplyEditToRegion(engine_resources *Engine, rect3 *SelectionAABB, world_edit_mode WorldEditMode)
{
  world_update_op_shape Shape = {
    .Type = type_world_update_op_shape_params_rect,
    .world_update_op_shape_params_rect.P0 = SelectionAABB->Min,
    .world_update_op_shape_params_rect.P1 = SelectionAABB->Max,
  };

  QueueWorldUpdateForRegion(Engine, WorldEditMode, &Shape, Engine->Editor.SelectedColorIndex, Engine->WorldUpdateMemory);
}


link_internal v3
ConstrainUpdateVector(v3 UpdateVector, face_index Face, world_edit_selection_mode SelectionMode)
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
                        world_edit_selection_mode SelectionMode,
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
DoSelectedVoxelDebugWindow(engine_resources *Engine, cp VoxelCP)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  voxel *V = TryGetVoxelPointer(World, VoxelCP);


  v3 SimP = Floor(GetSimSpaceP(World, VoxelCP));
#if VOXEL_DEBUG_COLOR
  if (V)
  {
    DEBUG_DrawSimSpaceVectorAt(Engine, SimP, Normalize(V->DebugColor)* 40.f,  GREEN, 0.25f);
    DEBUG_DrawSimSpaceVectorAt(Engine, SimP, Normalize(V->DebugColor)*-40.f, YELLOW, 0.25f);
    DEBUG_HighlightVoxel(Engine, SimP, YELLOW, DEFAULT_LINE_THICKNESS*2.f);
  }
#endif


  {
    local_persist window_layout Window = WindowLayout("Voxel Debug Window", V2(150.f, 150.f));
    PushWindowStart(Ui, &Window);

    if (V)
    {
      DoEditorUi(Ui, &Window, &V->Flags, CSz("Voxel Flags"));
      PushNewRow(Ui);

      DoEditorUi(Ui, &Window, &VoxelCP, CSz("CP"));

#if VOXEL_DEBUG_COLOR
      DoEditorUi(Ui, &Window, &V->DebugNoiseValue, CSz("Noise Value"));
      PushNewRow(Ui);

      DoEditorUi(Ui, &Window, &V->DebugColor, CSz("DebugColor"));

      r32 DotP = Dot(V->DebugColor, V3(0,0,1));
      DoEditorUi(Ui, &Window, &DotP, CSz("Dot against V3(0,0,1)"));
      PushNewRow(Ui);
      PushNewRow(Ui);


      world_chunk *ThisChunk = GetWorldChunkFromHashtable(World, VoxelCP.WorldP);

      PushTableStart(Ui);
        PushNewRow(Ui);
        PushColumn(Ui, CSz("Contributed"));
        PushColumn(Ui, CSz(" "));
        PushColumn(Ui, CSz(" "));
        PushColumn(Ui, CSz("Expected"));
        PushNewRow(Ui);

        PushColumn(Ui, CSz("To Normal"));
        PushColumn(Ui, CSz("Offset"));
        PushColumn(Ui, CSz("NoiseValue"));
        PushColumn(Ui, CSz("NoiseValue"));
        PushColumn(Ui, CSz(""));
        PushNewRow(Ui);
        v3i Dim = V3i(3,3,3);
        for (s32 dz = -1; dz < 2; ++dz)
        {
          for (s32 dy = -1; dy < 2; ++dy)
          {
            for (s32 dx = -1; dx < 2; ++dx)
            {
              /* if (dz == 0 && dy == 0 && dx == 0) continue; // ? */

              s32 NormalIndex = GetIndex(VoxelCP.Offset+V3(dx,dy,dz)+1, ThisChunk->Dim+2);
              s32 NoiseIndex = GetIndex(VoxelCP.Offset+V3(dx,dy,dz)+2, ThisChunk->Dim+4);
              v3 ExpectedNormalValue = V3(f32_MAX);
              r32 ExpectedNoiseValue = f32_MAX;
              if (ThisChunk->NoiseValues) { ExpectedNoiseValue = ThisChunk->NoiseValues[NoiseIndex]; }
              if (ThisChunk->NormalValues) { ExpectedNormalValue = ThisChunk->NormalValues[NormalIndex]; }

              voxel *dV = TryGetVoxelPointer(World, Canonicalize(World, VoxelCP + V3(dx,dy,dz)));

              ui_style *Style = &DefaultStyle;

              b32 ValueContributedToNormal = False;
              if (dV && dV->DebugNoiseValue-dz > Truncate(V->DebugNoiseValue))
              {
                Style = &DefaultSelectedStyle;
                ValueContributedToNormal = True;
              }

              {
                if (ValueContributedToNormal)
                {
                  PushColumn(Ui, CSz("+"));
                }
                else
                {
                  PushColumn(Ui, CSz(" "));
                }

                PushColumn(Ui, FSz("(%d %d %d)", dx, dy, dz), Style);
                if (dV) { PushColumn(Ui, CS(dV->DebugNoiseValue)); }
                else    { PushColumn(Ui, CSz("(INVALID NOISE INDEX)")); }

                if (dV && ExpectedNoiseValue != dV->DebugNoiseValue) { Style = &Global_DefaultErrorStyle; DEBUG_HighlightVoxel(Engine, SimP+V3(dx,dy,dz), RED); }
                else                                                 { Style = &DefaultStyle; }

                if (ExpectedNoiseValue < f32_MAX) { PushColumn(Ui, FSz("(%.2f)", r64(ExpectedNoiseValue)), Style); }
                else                              { PushColumn(Ui, FSz("(THIS CHUNK (%p) DID NOT HAVE VALID NOISE VALUE POINTER)", ThisChunk)); }

                if (dV && ExpectedNormalValue != dV->DebugColor) { Style = &Global_DefaultErrorStyle; DEBUG_HighlightVoxel(Engine, SimP+V3(dx,dy,dz), PINK); }
                else                                             { Style = &DefaultStyle; }

                if (ExpectedNormalValue.x < f32_MAX) { PushColumn(Ui, FSz("(%.2f,%.2f,%.2f)", r64(ExpectedNormalValue.x), r64(ExpectedNormalValue.y), r64(ExpectedNormalValue.z) ), Style); }
                else                                 { PushColumn(Ui, FSz("(THIS CHUNK (%p) DID NOT HAVE VALID NORMAL VALUE POINTER)", ThisChunk)); }


                PushNewRow(Ui);
              }
            }
            PushNewRow(Ui);
          }
          PushNewRow(Ui);
        }
      PushTableEnd(Ui);
#endif

    }
    else
    {
      PushColumn(Ui, CSz("(null)"));
    }
    PushWindowEnd(Ui, &Window);
  }
}

#if 1
link_internal interactable_handle
RenderAndInteractWithThumbnailTexture(renderer_2d *Ui, window_layout *Window, const char* InteractionString, asset_thumbnail *Thumb)
{
  texture *Texture = &Thumb->Texture;
  camera  *ThumbCamera  = &Thumb->Camera;

  interactable_handle B = PushButtonStart(Ui, UiId(Window, InteractionString, Cast(void*, Thumb)) );
    u32 Index = StartColumn(Ui);
      PushTexturedQuad(Ui, Texture, V2(Texture->Dim), zDepth_Text);
    EndColumn(Ui, Index);
  PushButtonEnd(Ui);

  v2 MouseDP = {};
  r32 CameraZDelta = {};
  if (Pressed(Ui, &B))
  {
    Ui->RequestedForceCapture = True;
    v2 MouseDelta = GetMouseDelta(&GetEngineResources()->Stdlib.Plat);
    if (Ui->Input->LMB.Pressed) { MouseDP = MouseDelta*2.f; }
    if (Ui->Input->RMB.Pressed) { CameraZDelta += MouseDelta.y*2.f; }
    UpdateGameCamera(GetWorld(), MouseDP, CameraZDelta, {}, ThumbCamera, 0.f);
  }

  /* if (Ui->Input->LMB.Pressed == False && Hover(Ui, &B)) { PushTooltip(Ui, ToString(Texture)); } */

  return B;
}
#endif


link_internal picked_voxel_position
MapWorldEditModeToHighlightVoxelForSingleSelection(world_edit_mode WorldEditMode)
{
  picked_voxel_position Result = {};

  switch (WorldEditMode)
  {
    case WorldEdit_Mode_Disabled:
    case WorldEdit_Mode_Attach:
    {
      Result = PickedVoxel_LastEmpty;
    } break;

    case WorldEdit_Mode_Paint:
    case WorldEdit_Mode_Remove:
    {
      Result = PickedVoxel_FirstFilled;
    } break;
  }

  // 0 is a valid result
  /* Assert(Result); */
  return Result;
}

link_internal void
DoBrushSettingsWindow(engine_resources *Engine, world_edit_mode WorldEditMode, world_edit_tool WorldEditTool, world_edit_brush_type WorldEditBrushType)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  local_persist window_layout Window = WindowLayout("Brush Settings", WindowLayoutFlag_Align_Right);
  PushWindowStart(Ui, &Window);
  switch (WorldEditTool)
  {
    case  WorldEdit_Tool_Disabled:
    case  WorldEdit_Tool_Select:
    case  WorldEdit_Tool_Eyedropper:
    case  WorldEdit_Tool_BlitEntity:
    case  WorldEdit_Tool_StandingSpots:
    {
    } break;

    case WorldEdit_Tool_Brush:
    {
      if (WorldEditBrushType == WorldEdit_BrushType_Noise)
      {
        noise_params *Params     = &Editor->NoiseEditor.Params;
        noise_params *PrevParams = &Editor->NoiseEditor.PrevParams;

        DoEditorUi(Ui, &Window, &Params->Type, CSz("Noise Type"), &DefaultUiRenderParams_Generic);
        switch (Params->Type)
        {
          case NoiseType_None: {} break;

          case NoiseType_Perlin:
          {
            perlin_noise_params *PerlinParams = &Editor->NoiseEditor.Params.PerlinParams;

            DoEditorUi(Ui, &Window, PerlinParams, CSz("Perlin"));

            if (Editor->SelectionClicks)
            {
              v3 SelectionMinP = GetSimSpaceP(World, Editor->SelectionRegion.Min);
              v3 SelectionMaxP = GetSimSpaceP(World, Editor->SelectionRegion.Max);

              Params->ChunkSize = V3i(SelectionMaxP-SelectionMinP);

              world_chunk *DestChunk = &Editor->NoiseEditor.Chunk;
              if (DestChunk->Dim != Params->ChunkSize)
              {
                // TODO(Jesse): Figure out exactly how this works.  We can't allocate from the Editor
                // memory pool because the goemetry buffers get freed to a freelist, and the editor memory
                // pool gets cleared on game reload
                //
                // @editor_chunk_memory_question
                //
                /* DeallocateWorldChunk(DestChunk, MeshFreelist); */
                AllocateWorldChunk(DestChunk, {}, Params->ChunkSize, Editor->Memory);
              }

              if (!AreEqual(Params, PrevParams))
              {
                *PrevParams = *Params;
                DestChunk->Flags = Chunk_Queued;
                InitializeChunkWithNoise( Noise_Perlin3D, GetThreadLocalState(ThreadLocal_ThreadIndex), DestChunk, DestChunk->Dim, {}, s32(PerlinParams->Period), s32(PerlinParams->Amplitude), s32(PerlinParams->Threshold), MeshBit_None, ChunkInitFlag_Noop, 0);
                SyncGpuBuffersImmediate(Engine, &DestChunk->Meshes);
              }

              RenderToTexture(Engine, &Editor->NoiseEditor.PreviewThumbnail, &DestChunk->Meshes, V3(Params->ChunkSize)/-2.f);
              RenderAndInteractWithThumbnailTexture(Ui, &Window, "noise preview interaction", &Editor->NoiseEditor.PreviewThumbnail);
            }
            else
            {
              PushColumn(Ui, CSz("Make a selection to use Noise Brush"));
            }

          } break;

          case NoiseType_Voronoi:
          {
            DoEditorUi(Ui, &Window, &Editor->NoiseEditor.Params.VoronoiParams, CSz("Voronoi"));
          } break;
        }
      }
    } break;
  }
  PushWindowEnd(Ui, &Window);
}


link_internal void
DoWorldEditor(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  world_edit_mode WorldEditMode = {};
  world_edit_tool WorldEditTool = {};
  world_edit_brush_type WorldEditBrushType = {};

  picked_voxel_position HighlightVoxel = PickedVoxel_FirstFilled;
  ui_toggle_button_group WorldEditModeRadioGroup = {};

  ui_toggle_button_group WorldEditToolButtonGroup = {};
  ui_toggle_button_group WorldEditModeButtonGroup = {};
  ui_toggle_button_group WorldEditBrushTypeButtonGroup = {};

  {
    local_persist window_layout Window = WindowLayout("World Edit");
    PushWindowStart(Ui, &Window);

    PushTableStart(Ui);
      ui_render_params Params = DefaultUiRenderParams_Generic;
      ui_element_reference CurrentRef = {};

      {
        WorldEditToolButtonGroup = DoEditorUi(Ui, &Window, &WorldEditTool, CSz("Tool"), &Params, ToggleButtonGroupFlags_DrawVertical);
        CurrentRef = WorldEditToolButtonGroup.UiRef;
      }

      if (WorldEditTool == WorldEdit_Tool_Brush)
      {
        Params.RelativePosition.Position   = Position_RightOf;
        Params.RelativePosition.RelativeTo = CurrentRef;
        WorldEditBrushTypeButtonGroup = DoEditorUi(Ui, &Window, &WorldEditBrushType, CSz("Brush Type"), &Params, ToggleButtonGroupFlags_DrawVertical);
        CurrentRef = WorldEditBrushTypeButtonGroup.UiRef;
      }

      if (WorldEditTool == WorldEdit_Tool_Brush)
      {
        Params.RelativePosition.Position   = Position_RightOf;
        Params.RelativePosition.RelativeTo = CurrentRef;
        WorldEditModeButtonGroup = DoEditorUi(Ui, &Window, &WorldEditMode, CSz("Mode"), &Params, ToggleButtonGroupFlags_DrawVertical);
        CurrentRef = WorldEditModeButtonGroup.UiRef;
      }

    PushTableEnd(Ui);


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


        interactable_handle ColorPickerButton = PushButtonStart(Ui, UiId(&Window, "ColorPicker", Cast(void*, u64(ColorIndex))) );
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


        if ( (ColorIndex+1) % 16 == 0 ) { PushNewRow(Ui); }
      }
    PushTableEnd(Ui);
    PushWindowEnd(Ui, &Window);
  }




  //
  //
  // Do selection stuff
  //
  //


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

    // TODO(Jesse): Use pre-computed ray
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
        world_edit_selection_mode SelectionMode = {};
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
    DEBUG_DrawSimSpaceAABB(Engine, &SelectionAABB, BaseColor, Thickness);
  }




  //
  //
  // Do tool selection actions (what happens when we change a tool selection)
  //
  //

  if (WorldEditToolButtonGroup.AnyElementClicked)
  {
    switch (WorldEditTool)
    {
      case WorldEdit_Tool_Disabled:
      case WorldEdit_Tool_Brush:
      case WorldEdit_Tool_Eyedropper:
      case WorldEdit_Tool_BlitEntity:
      case WorldEdit_Tool_StandingSpots:
      { } break;

      case WorldEdit_Tool_Select:
      {
        ResetSelection(Editor);
      } break;
    }
  }

  if (WorldEditModeButtonGroup.AnyElementClicked)
  {
    switch (WorldEditMode)
    {
      case WorldEdit_Mode_Disabled:
      case WorldEdit_Mode_Paint:
      case WorldEdit_Mode_Attach:
      case WorldEdit_Mode_Remove:
      { } break;
    }
  }


  if (WorldEditTool == WorldEdit_Tool_Brush)
  {
    DoBrushSettingsWindow(Engine, WorldEditMode, WorldEditTool, WorldEditBrushType);
  }

  //
  //
  // Edit tool interactions in the world
  //
  //

  if ( UiCapturedMouseInput(Ui) == False &&
       UiHoveredMouseInput(Ui)  == False  )
  {
    switch (WorldEditTool)
    {
      case WorldEdit_Tool_Disabled:

      case WorldEdit_Tool_Brush:
      {
        switch (WorldEditBrushType)
        {
          case WorldEdit_BrushType_Disabled:
          {} break;

          case WorldEdit_BrushType_Entity:
          case WorldEdit_BrushType_Asset:
          {
            if (Input->LMB.Clicked)
            {
              if (EngineDebug->SelectedAsset.FileNode.Type)
              {
                cp EntityOrigin = Canonical_Position(&Engine->MousedOverVoxel.Value);
                EntityOrigin.Offset = Round(EntityOrigin.Offset);

                maybe_asset_ptr MaybeAsset = GetAssetPtr(Engine, &EngineDebug->SelectedAsset);
                if (MaybeAsset.Tag)
                {
                  asset *Asset = MaybeAsset.Value;
                  model *Model = GetPtr(&Asset->Models, EngineDebug->ModelIndex);
                  if (Model)
                  {
                    v3 AssetHalfDim = V3(Model->Dim)/2.f;
                    world_update_op_shape_params_asset AssetUpdateShape =
                    {
                      EngineDebug->SelectedAsset,
                      EngineDebug->ModelIndex,
                      Canonicalize(World, EntityOrigin - V3(AssetHalfDim.xy, 0.f))
                    };

                    if (WorldEditBrushType == WorldEdit_BrushType_Asset)
                    {
                      world_update_op_shape Shape =
                      {
                        type_world_update_op_shape_params_asset,
                        .world_update_op_shape_params_asset = AssetUpdateShape,
                      };
                      QueueWorldUpdateForRegion(Engine, WorldEdit_Mode_Attach, &Shape, {}, Engine->WorldUpdateMemory);
                    }
                    else if (WorldEditBrushType == WorldEdit_BrushType_Entity)
                    {
                      entity *E = TryGetFreeEntityPtr(Engine->EntityTable);
                      if (E)
                      {
                        SpawnEntity(E, &EngineDebug->SelectedAsset, EngineDebug->ModelIndex, EntityBehaviorFlags_Default, 0, &AssetUpdateShape.Origin, Model->Dim/2.f);
                      }
                    }
                    else
                    {
                      InvalidCodePath();
                    }
                  }
                }
              }
            }
          } break;

          case WorldEdit_BrushType_Single:
          {
            // NOTE(Jesse): This is kind of an optimization to jump directly to
            // rebuilding the chunk mesh instead of doing all the hoop-jumping
            // related to editing.  I'm not really sure if it's worth the extra
            // complexity here, and/or the slight bit of jank it introduces.  I
            // expect that once I serialize the world edits this should go away
            // and we can just collapse world edits automatically in the edit thread.
            //
            // When my laptop is unplugged running on battery power, this is _much_ faster.
            HighlightVoxel = MapWorldEditModeToHighlightVoxelForSingleSelection(WorldEditMode);
            if (WorldEditMode == WorldEdit_Mode_Paint)
            {

              b32 DoPaint = Input->LMB.Pressed;
              if (DoPaint)
              {
                Ui->RequestedForceCapture = True;;
                if (Engine->MousedOverVoxel.Tag)
                {
                  if (voxel *V = GetVoxelPointer(&Engine->MousedOverVoxel.Value, PickedVoxel_FirstFilled))
                  {
                    V->Color = Engine->Editor.SelectedColorIndex;
                    QueueChunkForMeshRebuild(&Plat->LowPriority, Engine->MousedOverVoxel.Value.Chunks[PickedVoxel_FirstFilled].Chunk);
                  }
                }
              }
            }
            else
            {
              b32 DoPaint = Input->LMB.Clicked;
              if (DoPaint)
              {
                Ui->RequestedForceCapture = True;
                if (Engine->MousedOverVoxel.Tag)
                {
                  v3 P0 = Floor(GetSimSpaceP(World, &Engine->MousedOverVoxel.Value, HighlightVoxel));
                  rect3 AABB = RectMinMax(P0, P0+1.f);
                  ApplyEditToRegion(Engine, &AABB, WorldEditMode);
                  /* QueueWorldUpdateForRegion(Engine, WorldEdit_Mode_Attach, &Shape, Editor->SelectedColorIndex, Engine->WorldUpdateMemory); */
                }
              }
            }
          } break;

          case WorldEdit_BrushType_Noise:
          {
            if (WorldEditMode && Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
            {
              world_chunk *Chunk = &Editor->NoiseEditor.Chunk;
              chunk_data D = {Chunk->Flags, Chunk->Dim, Chunk->Voxels, Chunk->VoxelLighting};
              world_update_op_shape_params_chunk_data ChunkDataShape = { D, GetSimSpaceP(World, Editor->SelectionRegion.Min) };

              world_update_op_shape Shape =
              {
                type_world_update_op_shape_params_chunk_data,
                .world_update_op_shape_params_chunk_data = ChunkDataShape,
              };
              QueueWorldUpdateForRegion(Engine, WorldEditMode, &Shape, Editor->SelectedColorIndex, Engine->WorldUpdateMemory);
            }
          } break;

          case WorldEdit_BrushType_Selection:
          {
            if (WorldEditMode && Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
            {
              ApplyEditToRegion(Engine, &SelectionAABB, WorldEditMode);
            }
          } break;
        }
      } break;

      case WorldEdit_Tool_Select:
      {
        if (Input->LMB.Clicked)
        { switch (Editor->SelectionClicks)
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

      case WorldEdit_Tool_Eyedropper:
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

      case WorldEdit_Tool_BlitEntity:
      {
        entity *SelectedEntity = GetEntity(EntityTable, EngineDebug->SelectedEntity);
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
              QueueWorldUpdateForRegion(Engine, WorldEdit_Mode_Attach, &Shape, {}, Engine->WorldUpdateMemory);
#endif
            }
          }
        }
      } break;

      case WorldEdit_Tool_StandingSpots:
      {
        NotImplemented;
#if 0
        if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
        {
          world_update_op_shape Shape = {
            .Type = type_world_update_op_shape_params_rect,
            .world_update_op_shape_params_rect.P0 = SelectionAABB.Min,
            .world_update_op_shape_params_rect.P1 = SelectionAABB.Max,
          };
          QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_StandingSpots, &Shape, Editor->SelectedColorIndex, Engine->WorldUpdateMemory);
        }
#endif
      } break;

    }
  }





  if (Input->Ctrl.Pressed || Input->Shift.Pressed) { Ui->RequestedForceCapture = True; }

  if (Input->Ctrl.Pressed && Input->S.Clicked) { RadioSelect(&WorldEditModeRadioGroup, WorldEdit_Tool_Select); ResetSelection(Editor); }

  /* if (Clicked(&WorldEditModeRadioGroup, CSz("Select"))) { ResetSelection(Editor); } */

  /* if (Input->Ctrl.Pressed && Input->F.Clicked) { ResetSelectionIfIncomplete(Editor); RadioSelect(&WorldEditModeRadioGroup, WorldEdit_Tool_FillSelection); } */

  if (Input->Ctrl.Pressed && Input->E.Clicked) { ResetSelectionIfIncomplete(Editor); RadioSelect(&WorldEditModeRadioGroup, WorldEdit_Tool_Eyedropper); }

  if (Editor->SelectionClicks == 2)
  {
    if (Input->Ctrl.Pressed && Input->D.Clicked) { ApplyEditToRegion(Engine, &SelectionAABB, WorldEdit_Mode_Remove); }

    if (Input->Ctrl.Pressed && Input->C.Clicked) { Editor->CopyRegion = Editor->SelectionRegion; }

    if (Input->Ctrl.Pressed && Input->V.Clicked)
    {
      v3 CopyDim = GetDim(World, Editor->CopyRegion);
      s32 VoxelCount = s32(Volume(CopyDim));
      Leak("voxel *V = Allocate(voxel, Engine->Memory, VoxelCount)");
      voxel *V = Allocate(voxel, Engine->WorldUpdateMemory, VoxelCount);

      Leak("");
      world_chunk_ptr_buffer Chunks = GatherChunksOverlappingArea(World, Editor->CopyRegion, Engine->WorldUpdateMemory);
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
      QueueWorldUpdateForRegion(Engine, WorldEdit_Mode_Attach, &Shape, {}, Engine->WorldUpdateMemory);
    }
  }

#if VOXEL_DEBUG_COLOR
  {
    cp DebugVoxel = {};
    if (Editor->SelectionClicks > 0)
    {
      DebugVoxel = Editor->SelectionRegion.Min;
    }
    else if (Engine->MousedOverVoxel.Tag)
    {
      DebugVoxel = Canonical_Position(&Engine->MousedOverVoxel.Value, PickedVoxel_FirstFilled);
    }

    DoSelectedVoxelDebugWindow(Engine, DebugVoxel);
  }
#endif

  if (Engine->MousedOverVoxel.Tag)
  {
    v3 SimP = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, HighlightVoxel));
    DEBUG_HighlightVoxel( Engine, SimP, RED, 0.075f);
  }
}
