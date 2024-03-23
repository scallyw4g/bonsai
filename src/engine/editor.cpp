//
// TODO(Jeses): Move to header
link_internal rect3
GetSelectionRect(world *World, level_editor *Editor)
{
  v3 SelectionMinP = GetSimSpaceP(World, Editor->SelectionRegion.Min);
  v3 SelectionMaxP = GetSimSpaceP(World, Editor->SelectionRegion.Max);

  rect3 Result = RectMinMax(SelectionMinP, SelectionMaxP);
  return Result;
}

//
// TODO(Jeses): Move to header
link_internal v3i
GetSelectionDim(world *World, level_editor *Editor)
{
  v3 SelectionMinP = GetSimSpaceP(World, Editor->SelectionRegion.Min);
  v3 SelectionMaxP = GetSimSpaceP(World, Editor->SelectionRegion.Max);

  v3i Result = V3i(SelectionMaxP - SelectionMinP);
  return Result;
}

// TODO(Jesse): Should these live somewhere else?  @engine_draw_file_nodes_helpers
link_internal maybe_file_traversal_node
EngineDrawFileNodesFilteredHelper(file_traversal_node Node, u64 Params)
{
  engine_resources *Engine = GetEngineResources();
  filtered_file_traversal_helper_params *HelperParams = ReinterpretCast(filtered_file_traversal_helper_params*, Params);
  maybe_file_traversal_node Result = DrawFileNodes(&Engine->Ui, Node, HelperParams);
  return Result;
}

// @engine_draw_file_nodes_helpers
link_internal b32 DefaultFileFilter(file_traversal_node *Node) { return True; }

// @engine_draw_file_nodes_helpers
link_internal maybe_file_traversal_node
EngineDrawFileNodesHelper(file_traversal_node Node, u64 Window)
{
  engine_resources *Engine = GetEngineResources();
  filtered_file_traversal_helper_params HelperParams = {(window_layout*)Window, DefaultFileFilter};
  maybe_file_traversal_node Result = DrawFileNodes(&Engine->Ui, Node, &HelperParams);
  return Result;
}


link_internal b32
InitEditor(level_editor *Editor)
{
  b32 Result = True;
  Editor->Memory = AllocateArena();

  Editor->AssetThumbnails.Memory = Editor->Memory;

  /* Editor->Shape.Settings.Type = BrushLayerType_Shape; */
  /* Editor->Noise.Settings.Type = BrushLayerType_Noise; */

  return Result;
}

link_internal b32
HardResetEditor(level_editor *Editor)
{
  // TODO(Jesse)(leak): Delete textures allocated to visualize layered noise brushes?
  // @hard_reset_texture_memory

  /* if (Editor->Shape.Preview.Thumbnail.Texture.ID) */
  /* { */
  /*   DeleteTexture(&Editor->Shape.Preview.Thumbnail.Texture); */
  /* } */

  /* if (Editor->Noise.Preview.Thumbnail.Texture.ID) */
  /* { */
  /*   DeleteTexture(&Editor->Noise.Preview.Thumbnail.Texture); */
  /* } */

  IterateOver(&Editor->AssetThumbnails, Thumb, Index)
  {
    DeleteTexture(&Thumb->Texture);
  }

  // @editor_chunk_memory_question
  //
  // I guess here we can actually just not worry about clearing it because
  // it'll get realloc'd in the update path ..?  Seems fine..
  /* DeallocateWorldChunk(&Editor->Noise.Chunk, &GetEngineResources()->MeshFreelist); */

  VaporizeArena(Editor->Memory);

  // NOTE(Jesse): There are some default values in noise params that we want to reset to
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

poof(do_editor_ui_for_compound_type(perlin_noise_params))
#include <generated/do_editor_ui_for_compound_type_perlin_noise_params.h>

poof(do_editor_ui_for_compound_type(voronoi_noise_params))
#include <generated/do_editor_ui_for_compound_type_voronoi_noise_params.h>

poof(do_editor_ui_for_compound_type(world_update_op_shape_params_sphere))
#include <generated/do_editor_ui_for_compound_type_world_update_op_shape_params_sphere.h>
poof(do_editor_ui_for_compound_type(world_update_op_shape_params_rect))
#include <generated/do_editor_ui_for_compound_type_world_update_op_shape_params_rect.h>
poof(do_editor_ui_for_enum(shape_type))
#include <generated/do_editor_ui_for_enum_shape_type.h>
poof(do_editor_ui_for_compound_type(shape_layer))
#include <generated/do_editor_ui_for_compound_type_shape_layer.h>
poof(do_editor_ui_for_compound_type(brush_settings))
#include <generated/do_editor_ui_for_compound_type_brush_settings.h>


poof(do_editor_ui_for_container(v3_cursor))
#include <generated/do_editor_ui_for_container_v3_cursor.h>

poof(do_editor_ui_for_compound_type(ui_id))
#include <generated/do_editor_ui_for_compound_type_ui_id.h>

poof(do_editor_ui_for_compound_type(ui_toggle))
#include <generated/do_editor_ui_for_compound_type_ui_toggle.h>

link_internal cs
CS(ui_toggle_hashtable_iterator &Iter)
{
  return CSz("(TODO @put_name_here)");
}

poof(do_editor_ui_for_container(ui_toggle_hashtable))
#include <generated/do_editor_ui_for_container_ui_toggle_hashtable.h>




poof(do_editor_ui_for_compound_type(input_event))
#include <generated/do_editor_ui_for_compound_type_input_event.h>

poof(do_editor_ui_for_compound_type(input))
#include <generated/do_editor_ui_for_compound_type_input.h>

poof(do_editor_ui_for_compound_type(platform))
#include <generated/do_editor_ui_for_compound_type_platform.h>

poof(do_editor_ui_for_compound_type(bonsai_stdlib))
#include <generated/do_editor_ui_for_compound_type_bonsai_stdlib.h>










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

poof(do_editor_ui_for_compound_type(chunk_thumbnail))
#include <generated/do_editor_ui_for_compound_type_chunk_thumbnail.h>

poof(do_editor_ui_for_compound_type(noise_layer))
#include <generated/do_editor_ui_for_compound_type_noise_layer.h>

poof(do_editor_ui_for_compound_type(brush_layer))
#include <generated/do_editor_ui_for_compound_type_brush_layer.h>

poof(do_editor_ui_for_compound_type(layered_brush_editor))
#include <generated/do_editor_ui_for_compound_type_layered_brush_editor.h>


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

    case SelectionMode_ResizeAllAxies:
    {
      Result.Max += UpdateVector;
      Result.Min -= UpdateVector;
    } break;

    case SelectionMode_ResizeBothLinearAxies:
    {
      Result.Max += UpdateVector;
      Result.Min -= UpdateVector;
    } break;

    case SelectionMode_ResizeSingleLinearAxis:
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
ApplyEditToRegion(engine_resources *Engine, rect3 *SelectionAABB, u16 ColorIndex, world_edit_mode WorldEditMode, world_edit_mode_modifier Modifier)
{
  world_edit_shape Shape = {
    .Type = type_world_update_op_shape_params_rect,
    .world_update_op_shape_params_rect.Region = *SelectionAABB
  };

  QueueWorldUpdateForRegion(Engine, WorldEditMode, Modifier, &Shape, ColorIndex, Engine->WorldUpdateMemory);
}


link_internal v3
ConstrainUpdateVector(v3 UpdateVector, face_index Face, world_edit_selection_mode SelectionMode)
{
  v3 Result = UpdateVector;

  switch (SelectionMode)
  {
    case SelectionMode_Noop: { } break;

    case SelectionMode_ResizeAllAxies:
    {
      v3 Constraint = NormalForFace(Face);
      v3 SingleChannel = UpdateVector*Constraint;
      r32 MaxChannel = Max(Max(SingleChannel.r, SingleChannel.b), SingleChannel.g);
      if (MaxChannel == 0) { MaxChannel = Min(Min(SingleChannel.r, SingleChannel.g), SingleChannel.b); }
      Result = V3(MaxChannel);
    } break;

    case SelectionMode_ResizeBothLinearAxies:
    {
      v3 Constraint = NormalForFace(Face);
      Result *= Constraint;
    } break;

    case SelectionMode_TranslateLinear:
    case SelectionMode_ResizeSingleLinearAxis:
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
    case SelectionMode_ResizeSingleLinearAxis:
    case SelectionMode_ResizeBothLinearAxies:
    case SelectionMode_ResizeAllAxies:
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

  // TODO(Jesse)(leak): How do we deallocate these on hard reset?
  // @hard_reset_texture_memory
  if (Texture->ID == 0)
  {
    *Texture = MakeTexture_RGB(V2i(256), 0, CSz("NoisePreviewTexture"));
    StandardCamera(ThumbCamera, 10000.f, 500.f, 30.f);
  }

  interactable_handle B = PushButtonStart(Ui, UiId(Window, InteractionString, Cast(void*, Thumb)) );
    u32 Index = StartColumn(Ui);
      PushTexturedQuad(Ui, Texture, V2(Texture->Dim)*1.5f, zDepth_Text);
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
  }
  UpdateGameCamera(GetWorld(), MouseDP, CameraZDelta, {}, ThumbCamera, 0.f);

  /* if (Ui->Input->LMB.Pressed == False && Hover(Ui, &B)) { PushTooltip(Ui, ToString(Texture)); } */

  return B;
}
#endif


link_internal v3
GetHotVoxelForEditMode(engine_resources *Engine, world_edit_mode WorldEditMode)
{
  picked_voxel_position Pos = PickedVoxel_FirstFilled;

  switch (WorldEditMode)
  {
    case WorldEdit_Mode_Disabled: {} break;
    case WorldEdit_Mode_Attach:
    {
      Pos = PickedVoxel_LastEmpty;
    } break;

    case WorldEdit_Mode_Paint:
    case WorldEdit_Mode_Remove:
    {
      Pos = PickedVoxel_FirstFilled;
    } break;
  }

  v3 Result = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, Pos));
  return Result;
}

link_internal v3
GetHotVoxelForFlood(engine_resources *Engine, world_edit_mode WorldEditMode, world_edit_mode_modifier Modifier)
{
  v3 Result = {};
  picked_voxel_position Pos = {};

  if (Modifier == WorldEdit_Modifier_Flood)
  {
    switch (WorldEditMode)
    {
      case WorldEdit_Mode_Disabled: {} break;
      case WorldEdit_Mode_Attach:
      {
        Pos = PickedVoxel_FirstFilled;
      } break;

      case WorldEdit_Mode_Paint:
      case WorldEdit_Mode_Remove:
      {
        Pos = PickedVoxel_LastEmpty;
      } break;
    }

    Result = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, Pos));
  }

  return Result;
}

link_internal v3i
GetShapeDim(shape_layer *Layer)
{
  v3i Result = {};
  switch (Layer->Type)
  {
    case ShapeType_None: { } break;
    case ShapeType_Sphere:
    {
      // NOTE(Jesse): This can't have a +1 because that dialates it outside the selection region.
      Result = V3i(Layer->Sphere.Radius*2.f);
    } break;

    case ShapeType_Rect:
    {
      Result = V3i(GetDim(Layer->Rect.Region));
    } break;
  }

  return Result;
}

link_internal v3i
GetRequiredDimForLayer(v3i SelectionDim, brush_layer *Layer)
{
  v3i Request = GetDim(Layer->Settings.Offset);
  switch (Layer->Settings.Type)
  {
    case BrushLayerType_Noise:
    {
      Request += SelectionDim;
    } break;

    case BrushLayerType_Shape:
    {
      Request += GetShapeDim(&Layer->Settings.Shape);
    } break;
  }

  v3i Result = Max(V3i(0), Request);
  return Result;
}

// NOTE(Jesse): This is a little wacky, and I actually don't quite know why the
// preview chunk dimension is primal for noise layers, but .. there you go.
link_internal v3i
GetLayerDim(brush_layer *Layer)
{
  v3i Result = GetDim(Layer->Settings.Offset);
  switch (Layer->Settings.Type)
  {
    case BrushLayerType_Shape:
    {
      Result += GetShapeDim(&Layer->Settings.Shape);
    } break;

    case BrushLayerType_Noise:
    {
      Result += Layer->Preview.Chunk.Dim;
    } break;
  }
  return Result;
}

link_internal b32
CheckForChangesAndUpdate_ThenRenderToPreviewTexture(engine_resources *Engine, brush_layer *Layer)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  brush_settings *Settings     = &Layer->Settings;
  brush_settings *PrevSettings = &Layer->PrevSettings;

  v3i SelectionDim     = GetSelectionDim(World, Editor);
  v3i RequiredLayerDim = GetRequiredDimForLayer(SelectionDim, Layer);

  b32 ReallocChunk     = Editor->SelectionChanged || Layer->Preview.Chunk.Dim != RequiredLayerDim;
  b32 UpdateVoxels     = ReallocChunk || !AreEqual(Settings, PrevSettings);

  *PrevSettings = *Settings;



  world_chunk *Chunk = &Layer->Preview.Chunk;
  if (ReallocChunk)
  {
    // TODO(Jesse)(leak): Figure out exactly how this works.  We can't allocate from the Editor
    // memory pool because the goemetry buffers get freed to a freelist, and the editor memory
    // pool gets cleared on game reload
    //
    // @editor_chunk_memory_question
    //
    /* DeallocateWorldChunk(Chunk, MeshFreelist); */
    DeallocateGpuBuffers(Chunk);
    AllocateWorldChunk(Chunk, {}, RequiredLayerDim, Editor->Memory);
  }

  if (UpdateVoxels)
  {
    Info("Detected changes to settings, updating voxels.");
    switch (Settings->Type)
    {
      case BrushLayerType_Shape:
      {
        shape_layer *Shape = &Settings->Shape;
        switch (Shape->Type)
        {
          case ShapeType_None: {} break;

          case ShapeType_Rect:
          {
            Shape->Rect.Region = RectMinDim({}, V3(SelectionDim));
            ApplyBrushLayer(Engine, Layer, Chunk, Settings->Offset.Min);
            FinalizeChunkInitialization(Chunk);
            QueueChunkForMeshRebuild(&Plat->LowPriority, Chunk);
          } break;

          case ShapeType_Sphere:
          {
            // NOTE(Jesse): Constrain maximum sphere radius to minimum selection dimension
            r32 MaxSphereRadius = Min(Min(SelectionDim.x, SelectionDim.y), SelectionDim.z)/2.f;
            Shape->Sphere.Radius = MaxSphereRadius;
            Shape->Sphere.Location = Canonical_Position(V3(MaxSphereRadius), {});

            ApplyBrushLayer(Engine, Layer, Chunk, Settings->Offset.Min);
            FinalizeChunkInitialization(Chunk);
            QueueChunkForMeshRebuild(&Plat->LowPriority, Chunk);
          } break;
        }
      } break;

      case BrushLayerType_Noise:
      {
        noise_layer *Noise = &Settings->Noise;
        chunk_init_callback NoiseFunc = {};

        generic_noise_params NoiseParams = {};
        void *UserData = {};

        NoiseParams.Color     = Settings->Color;
        switch (Noise->Type)
        {
          case NoiseType_White:
          {
            NoiseFunc             = Terrain_WhiteNoise;
            NoiseParams.Threshold = Noise->Perlin.Threshold;
            NoiseParams.Period    = Noise->Perlin.Period;
            NoiseParams.Amplitude = Noise->Perlin.Amplitude;
          } break;

          case NoiseType_Perlin:
          {
            NoiseFunc             = Terrain_Perlin3D;
            NoiseParams.Threshold = Noise->Perlin.Threshold;
            NoiseParams.Period    = Noise->Perlin.Period;
            NoiseParams.Amplitude = Noise->Perlin.Amplitude;
          } break;

          case NoiseType_Voronoi:
          {
            NoiseFunc             = Terrain_Voronoi3D;
            NoiseParams.Threshold = Noise->Voronoi.Threshold;
            NoiseParams.Period    = Noise->Voronoi.Period;
            NoiseParams.Amplitude = Noise->Voronoi.Amplitude;
            UserData = Cast(void*, &Noise->Voronoi);
          } break;
        }

        Assert(NoiseFunc);

        Chunk->Flags = Chunk_Queued;

        InitializeChunkWithNoise( NoiseFunc,
                                  GetThreadLocalState(ThreadLocal_ThreadIndex),
                                  Chunk,
                                  Cast(void*, &NoiseParams),
                                  ChunkInitFlag_Noop,
                                  UserData,
                                  True,
                                  Settings->NoiseBasisOffset + V3i(GetAbsoluteP(Editor->SelectionRegion.Min, GetWorldChunkDim())));
      } break;
    }

  }


  SyncGpuBuffersImmediate(Engine, &Chunk->Meshes);
  if (Layer->Preview.Thumbnail.Texture.ID) //  NOTE(Jesse): Avoid spamming a warning to console
  {
    RenderToTexture(Engine, &Layer->Preview.Thumbnail, &Chunk->Meshes, V3(Chunk->Dim)/-2.f);
  }

  return UpdateVoxels;
}

link_internal void
BrushSettingsForShapeBrush(engine_resources *Engine, window_layout *Window, shape_layer *Layer)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  DoEditorUi(Ui, Window, &Layer->Type, CSz("ShapeType"));

  v3 SelectionDim = GetDim(GetSelectionRect(World, Editor));
  switch (Layer->Type)
  {
    case ShapeType_None: { } break;

    case ShapeType_Rect:
    {
      DoEditorUi(Ui, Window, &Layer->Rect, CSz("Settings"));
    } break;

    case ShapeType_Sphere:
    {
      DoEditorUi(Ui, Window, &Layer->Sphere, CSz("Settings"));
    } break;
  }

}

link_internal void
BrushSettingsForNoiseBrush(engine_resources *Engine, window_layout *Window, noise_layer *Layer, chunk_thumbnail *Preview)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  PushTableStart(Ui);
    if (SelectionComplete(Editor->SelectionClicks))
    {
      DoEditorUi(Ui, Window, &Layer->Type, CSz("NoiseType"), &DefaultUiRenderParams_Generic);
      PushTableStart(Ui); // TODO(Jesse): Necessary?
        switch (Layer->Type)
        {
          case NoiseType_White:
          {
            DoEditorUi(Ui, Window, &Layer->White, CSz("Settings"));
          } break;

          case NoiseType_Perlin:
          {
            DoEditorUi(Ui, Window, &Layer->Perlin, CSz("Settings"));
          } break;

          case NoiseType_Voronoi:
          {
            DoEditorUi(Ui, Window, &Layer->Voronoi, CSz("Settings"));
          } break;
        }
      PushTableEnd(Ui);
    }
    else
    {
      PushColumn(Ui, CSz("Make a selection to use Noise Brush"));
      PushNewRow(Ui);
    }
  PushTableEnd(Ui);
}

link_internal void
DoSettingsForBrush(engine_resources *Engine, brush_layer *Layer, window_layout *Window)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  brush_settings *Settings = &Layer->Settings;

  OPEN_INDENT_FOR_TOGGLEABLE_REGION();

  PushNewRow(Ui);
  DoEditorUi(Ui, Window, &Settings->Type, CSz("Type"), &DefaultUiRenderParams_Generic);


  switch (Layer->Settings.Type)
  {
    case BrushLayerType_Noise:
    {
      BrushSettingsForNoiseBrush(Engine, Window, &Settings->Noise, &Layer->Preview);
    } break;

    case BrushLayerType_Shape:
    {
      BrushSettingsForShapeBrush(Engine, Window, &Settings->Shape);
    } break;
  }


  // TODO(Jesse): do enum selector for Mode/Modifier/iterations
  DoEditorUi(Ui, Window, &Settings->Mode,       CSz("Mode"));
  DoEditorUi(Ui, Window, &Settings->Modifier,   CSz("Modifier"));
  if (Settings->Modifier == WorldEdit_Modifier_Surface || Settings->Modifier == WorldEdit_Modifier_Flood)
  {
    DoEditorUi(Ui, Window, &Settings->Iterations, CSz("Iterations"));
    PushNewRow(Ui); // Primitives require a new row.. I forget why, but there's a good reason.
  }

  DoEditorUi(Ui, Window, &Settings->Offset,           CSz("Dilation"));
  DoEditorUi(Ui, Window, &Settings->NoiseBasisOffset, CSz("Basis"), &DefaultUiRenderParams_Generic);

  DoEditorUi(Ui, Window, &Settings->Invert,           CSz("Invert"), &DefaultUiRenderParams_Generic);
  PushNewRow(Ui);

  // NOTE(Jesse): These are only stricly necessary if Modifier is Flood or Surface .. do we care?
  /* Settings->Offset.Min = Min(V3i(-Settings->Iterations), Settings->Offset.Min); */
  /* Settings->Offset.Max = Max(V3i( Settings->Iterations), Settings->Offset.Max); */

  {
    ui_style Style = UiStyleFromLightestColor(GetColorData(Settings->Color));
    PushUntexturedQuad(Ui, {}, V2(Global_Font.Size.y), zDepth_Text, &Style, DefaultGenericPadding);
    if (Button(Ui, CSz("Set Color"), UiId(Window, "set color interaction", Cast(void*, Settings)))) { Settings->Color = Engine->Editor.SelectedColorIndex; }
    PushNewRow(Ui);
  }

  PushTableStart(Ui);
    RenderAndInteractWithThumbnailTexture(Ui, Window, "noise preview interaction", &Layer->Preview.Thumbnail);
    PushNewRow(Ui);
  PushTableEnd(Ui);
  CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
}

link_internal void
ApplyBrushLayer(engine_resources *Engine, brush_layer *Layer, world_chunk *DestChunk, v3i SmallestMinOffset)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  brush_settings *Settings = &Layer->Settings;

  world_edit_mode              Mode = Settings->Mode;
  world_edit_mode_modifier Modifier = Settings->Modifier;


  rect3i UpdateBounds = {{}, DestChunk->Dim};

  world_edit_shape Shape = {};
  switch (Settings->Type)
  {
    case BrushLayerType_Shape:
    {
      Shape.Type = world_update_op_shape_type(Settings->Shape.Type);

      switch (Settings->Shape.Type)
      {
        case ShapeType_None: { } break;

        case ShapeType_Sphere:
        {
          Shape.world_update_op_shape_params_sphere = Settings->Shape.Sphere;
          Shape.world_update_op_shape_params_sphere.Location.Offset = ((DestChunk->Dim/2.f) - V3(SmallestMinOffset));
        } break;

        case ShapeType_Rect:
        {
          Shape.world_update_op_shape_params_rect = Settings->Shape.Rect;

          Shape.world_update_op_shape_params_rect.Region.Min -= V3(SmallestMinOffset);
          Shape.world_update_op_shape_params_rect.Region.Max -= V3(SmallestMinOffset);
        } break;
      }

    } break;

    case BrushLayerType_Noise:
    {
      noise_layer     *Noise = &Settings->Noise;
      world_chunk  *SrcChunk = &Layer->Preview.Chunk;
      v3i       SrcOffsetMin = Settings->Offset.Min;

      v3i DestRelativeMinCorner = (-1*SmallestMinOffset) + SrcOffsetMin;

      chunk_data D = {SrcChunk->Flags, SrcChunk->Dim, SrcChunk->Voxels, SrcChunk->VoxelLighting};
      world_update_op_shape_params_chunk_data ChunkDataShape = { D, V3(DestRelativeMinCorner) };

      Assert(SrcChunk->Dim <= DestChunk->Dim);

      Shape.Type = type_world_update_op_shape_params_chunk_data;
      Shape.world_update_op_shape_params_chunk_data = ChunkDataShape;
    } break;
  }

  v3 SimFloodOrigin = V3(0);
  u16 ColorIndex = Layer->Settings.Color;

  s32 Iterations = Settings->Iterations;
  if (Iterations > 1) { Info("%d", Iterations); }
  RangeIterator(IterIndex, Iterations)
  {
    work_queue_entry_update_world_region Job = WorkQueueEntryUpdateWorldRegion(Mode, Modifier, SimFloodOrigin, &Shape, ColorIndex, {}, {}, {}, 0);
    ApplyUpdateToRegion(GetThreadLocalState(ThreadLocal_ThreadIndex), &Job, UpdateBounds, DestChunk, Layer->Settings.Invert);
    DestChunk->FilledCount = MarkBoundaryVoxels_MakeExteriorFaces( DestChunk->Voxels, DestChunk->Dim, {{}}, DestChunk->Dim );
  }

}

link_internal v3i
GetSmallestMinOffset(layered_brush_editor *LayeredBrush, v3i *LargestLayerDim)
{
  v3i SmallestMinOffset = V3i(s32_MAX);

  brush_layer *Layers =  LayeredBrush->Layers;
  RangeIterator(LayerIndex, LayeredBrush->LayerCount)
  {
    brush_layer *Layer = Layers + LayerIndex;

    if (LargestLayerDim) { v3i LayerDim = GetLayerDim(Layer); *LargestLayerDim = Max(LayerDim, *LargestLayerDim); }
    SmallestMinOffset = Min(Layer->Settings.Offset.Min, SmallestMinOffset);
  }

  return SmallestMinOffset;
}


link_internal cs
GetFilenameForBrush(cs Name, s32 Version = 0)
{
  cs Result;

  if (EndsWith(Name, CSz(".brush"))) { Name.Count -= 6; }

  if (Version)
  {
    Result = FCS(CSz("brushes/%S.%d.brush\0"), Name, Version);
  }
  else
  {
    Result = FCS(CSz("brushes/%S.brush\0"), Name);
  }

  return Result;
}

link_internal void
SaveBrush(layered_brush_editor *LayeredBrush, const char *FilenameZ)
{
  u8_cursor_block_array OutputStream = BeginSerialization();
  Serialize(&OutputStream, LayeredBrush);

  if (FinalizeSerialization(&OutputStream, FilenameZ) == False)
  {
    SoftError("Unable to serialize brush (%s) to file (%s).", LayeredBrush->NameBuf, FilenameZ);
  }
  else
  {
    ZeroMemory(LayeredBrush->NameBuf, NameBuf_Len);
    cs BrushNameBuf = CS(LayeredBrush->NameBuf, NameBuf_Len);

    cs BrushBasename = Basename(CS(FilenameZ));
    CopyString(&BrushBasename, &BrushNameBuf);
  }
}


link_internal void
BrushSettingsForLayeredBrush(engine_resources *Engine, window_layout *BrushSettingsWindow)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  layered_brush_editor *LayeredBrush = &Editor->LayeredBrushEditor;
  brush_layer          *Layers             =  LayeredBrush->Layers;

  cs BrushNameBuf = CS(LayeredBrush->NameBuf, NameBuf_Len);


  {
    PushWindowStart(Ui, BrushSettingsWindow);
    memory_arena *Tran = GetTranArena();

    if (LayeredBrush->LayerCount)
    {
      if (Button(Ui, CSz("Save"), UiId(BrushSettingsWindow, "brush save", 0u)))
      {
        cs BrushFilepath = GetFilenameForBrush(CS(LayeredBrush->NameBuf));
        SaveBrush(LayeredBrush, BrushFilepath.Start);
      }

      if (Button(Ui, CSz("Duplicate"), UiId(BrushSettingsWindow, "brush dup", 0u)))
      {
        cs_buffer Pieces = Split( CS(LayeredBrush->NameBuf), '.', Tran);

        if (Pieces.Count > 2)
        {
          cs BrushNameString = Pieces.Start[0];
          cs VersionString   = Pieces.Start[Pieces.Count-2];

          s32 VersionNumber;
          if ( ParseInteger(VersionString, &VersionNumber) )
          {
            cs BrushFilepath = GetFilenameForBrush(BrushNameString, VersionNumber);
            while (FileExists(BrushFilepath.Start))
            {
              ++VersionNumber;
              BrushFilepath = GetFilenameForBrush(BrushNameString, VersionNumber);
            }

            SaveBrush(LayeredBrush, BrushFilepath.Start);
          }
        }
        else
        {
          cs BrushFilepath = GetFilenameForBrush(CS(LayeredBrush->NameBuf), 1);
          SaveBrush(LayeredBrush, BrushFilepath.Start);
        }
      }
    }


    ui_id ImportToggleId = UiId(BrushSettingsWindow, "brush import", 0u);
    if (ToggleButton(Ui, CSz("Import"), CSz("Import"), ImportToggleId))
    {
      PushNewRow(Ui);

      filtered_file_traversal_helper_params HelperParams = {BrushSettingsWindow, 0};
      maybe_file_traversal_node ClickedFileNode = PlatformTraverseDirectoryTreeUnordered(CSz("brushes"), EngineDrawFileNodesFilteredHelper, u64(&HelperParams) );

      if (ClickedFileNode.Tag)
      {
        cs Filename = Concat(ClickedFileNode.Value.Dir, CSz("/"), ClickedFileNode.Value.Name, Tran);
        u8_cursor Bytes = BeginDeserialization(Filename, Tran);
        if (Deserialize(&Bytes, &Editor->LayeredBrushEditor, Tran) == False)
        {
          SoftError("While deserializing brush (%S).", Filename);
        }
        FinalizeDeserialization(&Bytes);

        // NOTE(Jesse): This has to happen after deserialization cause some
        // brushes got saved out with a name, which gets read back in..
        ZeroMemory(LayeredBrush->NameBuf, NameBuf_Len);
        CopyString(&ClickedFileNode.Value.Name, &BrushNameBuf);

        SetToggleButton(Ui, ImportToggleId, False);
      }
    }
    else
    {

      if (Button(Ui, CSz("New"), UiId(BrushSettingsWindow, "brush new", 0u)))
      {
        ZeroMemory(LayeredBrush->NameBuf, NameBuf_Len);

        cs Src = CSz("_untitled.brush");
        CopyString(&Src, &BrushNameBuf);

        LayeredBrush->LayerCount = 1;
        RangeIterator(LayerIndex, MAX_BRUSH_LAYERS)
        {
          brush_layer *Layer = Layers + LayerIndex;
          Layer->Settings = {};
        }
      }

      if (LayeredBrush->LayerCount)
      {
        PushNewRow(Ui);
        PushNewRow(Ui);

        {
          ui_id TextBoxId = UiId(BrushSettingsWindow, "name_buf_textbox", LayeredBrush->NameBuf);
          cs NameBuf = CS(LayeredBrush->NameBuf);
          TextBox(Ui, CSz("BrushName"), NameBuf, NameBuf_Len, TextBoxId);
          PushNewRow(Ui);

          DoEditorUi(Ui, BrushSettingsWindow, &LayeredBrush->LayerCount, CSz("Layer Count"), &DefaultUiRenderParams_Generic);
          // Clamp LayerCount to (1,MAX_BRUSH_LAYERS) once it's set
          LayeredBrush->LayerCount = Max(LayeredBrush->LayerCount, 1);
          LayeredBrush->LayerCount = Min(LayeredBrush->LayerCount, MAX_BRUSH_LAYERS);
          PushNewRow(Ui);
          PushNewRow(Ui);
        }

        {
          DoEditorUi(Ui, BrushSettingsWindow, &Editor->Params.Mode,     CSz("Mode"),     &DefaultUiRenderParams_Generic);
          DoEditorUi(Ui, BrushSettingsWindow, &Editor->Params.Modifier, CSz("Modifier"), &DefaultUiRenderParams_Generic);
          PushNewRow(Ui);
        }

        {
          DoEditorUi(Ui, BrushSettingsWindow, &LayeredBrush->SeedBrushWithSelection, CSz("SeedBrushWithSelection"), &DefaultUiRenderParams_Generic);
          PushNewRow(Ui);

          DoEditorUi(Ui, BrushSettingsWindow, &LayeredBrush->BrushFollowsCursor,      CSz("BrushFollowsCursor"),      &DefaultUiRenderParams_Generic);
          PushNewRow(Ui);
          PushNewRow(Ui);
        }

      }
    }
    PushWindowEnd(Ui, BrushSettingsWindow);
  }

  {
  }


  b32 AnyBrushSettingsUpdated = False;
  {
    RangeIterator(LayerIndex, LayeredBrush->LayerCount)
    {
      brush_layer *Layer = Layers + LayerIndex;
      AnyBrushSettingsUpdated |= CheckForChangesAndUpdate_ThenRenderToPreviewTexture(Engine, Layer);
    }
  }


  {
    local_persist window_layout LayersWindow = WindowLayout("Layers", WindowLayoutFlag_Align_Right);
    PushWindowStart(Ui, &LayersWindow);

    {

      {
        b32 ReorderUp         = False;
        b32 ReorderDown       = False;
        b32 Duplicate         = False;
        b32 Delete            = False;
        s32 EditLayerIndex = 0;
        PushTableStart(Ui);
        RangeIterator(LayerIndex, LayeredBrush->LayerCount)
        {
          brush_layer *Layer = Layers + LayerIndex;

          if (ToggleButton(Ui, FSz("v Layer %d", LayerIndex), FSz("> Layer %d", LayerIndex), UiId(BrushSettingsWindow, "brush_layer toggle interaction", Layer)))
          {
            if (Button(Ui, CSz("Up"), UiId(BrushSettingsWindow, "layer_reorder_up", Layer)))
            {
              ReorderUp = True;
              EditLayerIndex = LayerIndex;
            }

            if (Button(Ui, CSz("Down"), UiId(BrushSettingsWindow, "layer_reorder_down", Layer)))
            {
              ReorderDown = True;
              EditLayerIndex = LayerIndex;
            }

            if (Button(Ui, CSz("Dup"), UiId(BrushSettingsWindow, "layer_duplicate", Layer)))
            {
              Duplicate = True;
              EditLayerIndex = LayerIndex;
            }

            if (Button(Ui, CSz("Del"), UiId(BrushSettingsWindow, "layer_delete", Layer)))
            {
              Delete = True;
              EditLayerIndex = LayerIndex;
            }



            DoSettingsForBrush(Engine, Layer, BrushSettingsWindow);
          }
          PushNewRow(Ui);
        }
        PushTableEnd(Ui);

        if (ReorderUp)
        {
          if (EditLayerIndex > 0)
          {
            brush_layer *Layer = Layers + EditLayerIndex;
            brush_layer Tmp = Layers[EditLayerIndex-1];
            Layers[EditLayerIndex-1].Settings = Layer->Settings;
            Layer->Settings = Tmp.Settings;
          }
        }

        if (ReorderDown)
        {
          if (LayeredBrush->LayerCount)
          {
            if (EditLayerIndex < LayeredBrush->LayerCount-1)
            {
              brush_layer *Layer = Layers + EditLayerIndex;
              brush_layer Tmp = Layers[EditLayerIndex+1];
              Layers[EditLayerIndex+1].Settings = Layer->Settings;
              Layer->Settings = Tmp.Settings;
            }
          }
        }

        if (Duplicate)
        {
          if (LayeredBrush->LayerCount < MAX_BRUSH_LAYERS)
          {
            LayeredBrush->LayerCount += 1;

            // Shuffle layers forward.  This conveniently duplicates the EditLayerIndex
            RangeIteratorReverseRange(LayerIndex, MAX_BRUSH_LAYERS, EditLayerIndex+1)
            {
              Layers[LayerIndex].Settings = Layers[LayerIndex-1].Settings;
            }
          }
        }

        if (Delete)
        {
          if (LayeredBrush->LayerCount < MAX_BRUSH_LAYERS)
          {
            // Shuffle layers backwards, overwriting EditLayerIndex
            RangeIteratorRange(LayerIndex, MAX_BRUSH_LAYERS, EditLayerIndex+1)
            {
              Layers[LayerIndex-1].Settings = Layers[LayerIndex].Settings;
            }

            LayeredBrush->LayerCount -= 1;
          }
        }
      }

      {
        world_chunk *Root_LayeredBrushPreview = &LayeredBrush->Preview.Chunk;
        //
        // TODO(Jesse)(async, speed): It would be kinda nice if this ran async..
        if (AnyBrushSettingsUpdated)
        {
          // First find the largest total dimension of all the layers, and the
          // largest negative minimum offset.  We need this min offset such
          // that we can position layers relative to it.  By doing this, we
          // implicitly take into account the size of the selection, but the
          // computation is simpler than if we used the selection box directly
          // .. I claim ..
          v3i LargestLayerDim = GetSelectionDim(World, Editor);
          v3i SmallestMinOffset = GetSmallestMinOffset(LayeredBrush, &LargestLayerDim);



          // Clear the voxels if the size didn't change, otherwise realloc
          if (Root_LayeredBrushPreview->Dim == LargestLayerDim)
          {
            // TODO(Jesse): Actually necessary??  I think maybe not
            ClearChunkVoxels(Root_LayeredBrushPreview->Voxels, Root_LayeredBrushPreview->Dim);
          }
          else
          {
            // @editor_chunk_memory_question
            AllocateWorldChunk(Root_LayeredBrushPreview, {}, LargestLayerDim, Editor->Memory);
          }

          if (LayeredBrush->SeedBrushWithSelection)
          {
            if (SelectionComplete(Editor->SelectionClicks))
            {
              world_chunk_ptr_buffer Chunks = GatherChunksOverlappingArea(World, Editor->CopyRegion, Engine->WorldUpdateMemory);
              world_chunk Seed = GatherVoxelsOverlappingArea(Engine, Editor->SelectionRegion, GetTranArena());
              CopyChunkOffset(&Seed, Seed.Dim, Root_LayeredBrushPreview, Root_LayeredBrushPreview->Dim, -1*SmallestMinOffset);
            }
          }

          RangeIterator(LayerIndex, LayeredBrush->LayerCount)
          {
            brush_layer *Layer = Layers + LayerIndex;
            ApplyBrushLayer(Engine, Layer, Root_LayeredBrushPreview, SmallestMinOffset);
          }

          FinalizeChunkInitialization(Root_LayeredBrushPreview);
          QueueChunkForMeshRebuild(&Plat->LowPriority, Root_LayeredBrushPreview);
        }

        SyncGpuBuffersImmediate(Engine, &Root_LayeredBrushPreview->Meshes);
        /* RenderToTexture(Engine, &LayeredBrush->Preview.Thumbnail, &Root_LayeredBrushPreview->Meshes, Root_LayeredBrushPreview->Dim/-2.f); */
      }
    }

    PushWindowEnd(Ui, &LayersWindow);
  }
}

link_internal void
DoBrushSettingsWindow(engine_resources *Engine, world_edit_tool WorldEditTool, world_edit_brush_type WorldEditBrushType)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  local_persist window_layout Window = WindowLayout("Brush Settings", WindowLayoutFlag_Align_Bottom);
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
      switch (WorldEditBrushType)
      {
        case WorldEdit_BrushType_Disabled:  {} break;
        case WorldEdit_BrushType_Selection: {} break;
        case WorldEdit_BrushType_Single:    {} break;
        case WorldEdit_BrushType_Entity:    {} break;

        case WorldEdit_BrushType_Asset:
        {
          PushWindowStart(Ui, &Window);
            DoEditorUi(Ui, &Window, &Editor->Params.Mode,     CSz("Mode"),     &DefaultUiRenderParams_Generic);
            DoEditorUi(Ui, &Window, &Editor->Params.Modifier, CSz("Modifier"), &DefaultUiRenderParams_Generic);
          PushWindowEnd(Ui, &Window);
        } break;


#if 0
        case WorldEdit_BrushType_Noise:
        {
          CheckForChangesAndUpdate_ThenRenderToPreviewTexture(Engine, &Editor->Noise);
          PushWindowStart(Ui, &Window);
            DoSettingsForBrush(Engine, &Editor->Noise, &Window);
          PushWindowEnd(Ui, &Window);
        } break;

        case WorldEdit_BrushType_Shape:
        {
          CheckForChangesAndUpdate_ThenRenderToPreviewTexture(Engine, &Editor->Shape);
          PushWindowStart(Ui, &Window);
            DoSettingsForBrush(Engine, &Editor->Shape, &Window);
          PushWindowEnd(Ui, &Window);
        } break;
#endif

        case WorldEdit_BrushType_Layered:
        {
          BrushSettingsForLayeredBrush(Engine, &Window);
        } break;

      }
    } break;
  }
}

link_internal b32
CurrentToolIs(level_editor *Editor, world_edit_tool Tool, world_edit_brush_type BrushType)
{
  b32 Result = False;
  if (Editor->Tool == Tool)
  {
    Result = True;
    if (Editor->Tool == WorldEdit_Tool_Brush)
    {
      Result = (Editor->BrushType == BrushType);
    }
  }
  return Result;
}

link_internal aabb_intersect_result
EditWorldSelection(engine_resources *Engine, rect3 *SelectionAABB)
{
  UNPACK_ENGINE_RESOURCES(Engine);

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
    else
    {
      Thickness = 0.20f;
    }

    /* { */
      v3 SelectionMinP = GetSimSpaceP(World, Editor->SelectionRegion.Min);
      v3 SelectionMaxP = GetSimSpaceP(World, Editor->SelectionRegion.Max);
      *SelectionAABB = AABBMinMax(SelectionMinP, SelectionMaxP);
    /* } */

    if (CurrentToolIs(Editor, WorldEdit_Tool_Brush, WorldEdit_BrushType_Layered))
    {
      layered_brush_editor *Brush = &Editor->LayeredBrushEditor;
      if (Brush->BrushFollowsCursor)
      {
        v3 SelectionRad = (SelectionMaxP - SelectionMinP)/2.f;

        if (Engine->MousedOverVoxel.Tag)
        {
          cp MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value);

          Editor->SelectionRegion.Min = MouseP - SelectionRad;
          Editor->SelectionRegion.Max = MouseP + SelectionRad;
          Canonicalize(World, &Editor->SelectionRegion.Min);
          Canonicalize(World, &Editor->SelectionRegion.Max);

          Truncate(&Editor->SelectionRegion.Min.Offset);
          Truncate(&Editor->SelectionRegion.Max.Offset);

           SelectionMinP = GetSimSpaceP(World, Editor->SelectionRegion.Min);
           SelectionMaxP = GetSimSpaceP(World, Editor->SelectionRegion.Max);
          *SelectionAABB = AABBMinMax(SelectionMinP, SelectionMaxP);
        }
      }
    }

    {
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

          HighlightFace(Engine, Face, *SelectionAABB, InsetWidth, HiColor, HiThickness);

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

          // Intentionally an el-if chain from most specific, to least.  What's the alternative?
          //
          // Shift is resize
          // Ctrl  is move
          //
          if (Input->Shift.Pressed && Input->Ctrl.Pressed && Input->Alt.Pressed)
          {
            SelectionMode = SelectionMode_ResizeAllAxies;
          }
          else if (Input->Shift.Pressed && Input->Alt.Pressed)
          {
            SelectionMode = SelectionMode_ResizeBothLinearAxies;
          }
          else if (Input->Ctrl.Pressed && Input->Alt.Pressed)
          {
            SelectionMode = SelectionMode_TranslateLinear;
          }
          else if (Input->Shift.Pressed)
          {
            SelectionMode = SelectionMode_ResizeSingleLinearAxis;
          }
          else if (Input->Ctrl.Pressed)
          {
            SelectionMode =  SelectionMode_TranslatePlanar;
          }

          /* Info("%S", ToString(SelectionMode)); */
          /* if (SelectionMode) { Ui->RequestedForceCapture = True; } */

          rect3i ModifiedSelection = DoSelectonModification(Engine, &Ray, SelectionMode, &Editor->Selection, *SelectionAABB);

          if (!Input->LMB.Pressed)
          {
            // If we actually changed the selection region
            rect3cp ProposedSelection = SimSpaceToCanonical(World, &ModifiedSelection);
            /* if (!AreEqual(Editor->SelectionRegion, ProposedSelection)) */
            {
              // Make ModifiedSelection permanent
              Editor->SelectionRegion = ProposedSelection;
              Editor->Selection.ClickedFace = FaceIndex_None;
            }
          }
        }
      }
    }

    // Draw selection box
    //

    u8 BaseColor = WHITE;
    DEBUG_DrawSimSpaceAABB(Engine, SelectionAABB, BaseColor, Thickness);
  }


  // Don't fire selection changed event when dragging selection with selection edit tool
  if (Editor->SelectionClicks != 1)
  {
    if (AreEqual(Editor->SelectionRegion, Editor->PrevSelectionRegion))
    {
      Editor->SelectionChanged = False;
    }
    else
    {
      Editor->SelectionChanged = True;
    }
    Editor->PrevSelectionRegion = Editor->SelectionRegion;
  }




  return AABBTest;
}

link_internal cp
GetSelectionCenterP(world *World, level_editor *Editor)
{
  v3i Dim = GetSelectionDim(World, Editor);
  cp Result = Canonicalize(World, Editor->SelectionRegion.Min + V3(Dim/2));
  return Result;
}

link_internal void
DoWorldEditor(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  // @selection_changed_flag
  //
  aabb SelectionAABB = {};
  aabb_intersect_result AABBTest = EditWorldSelection(Engine, &SelectionAABB);

  ui_toggle_button_group WorldEditToolButtonGroup = {};
  ui_toggle_button_group WorldEditModeButtonGroup = {};
  ui_toggle_button_group WorldEditBrushTypeButtonGroup = {};
  ui_toggle_button_group WorldEditModifierButtonGroup = {};

  {
    local_persist window_layout Window = WindowLayout("World Edit");
    PushWindowStart(Ui, &Window);

    PushTableStart(Ui);
      ui_render_params Params = DefaultUiRenderParams_Button;
      Params.Padding = V4(6, 3, 6, 3);
      ui_element_reference CurrentRef = {};

      {
        WorldEditToolButtonGroup = DoEditorUi(Ui, &Window, &Editor->Tool, CSz("Tool"), &Params, ToggleButtonGroupFlags_DrawVertical);
        CurrentRef = WorldEditToolButtonGroup.UiRef;
      }

      if (Editor->Tool == WorldEdit_Tool_Brush)
      {
        Params.RelativePosition.Position   = Position_RightOf;
        Params.RelativePosition.RelativeTo = CurrentRef;
        WorldEditBrushTypeButtonGroup = DoEditorUi(Ui, &Window, &Editor->BrushType, CSz("Brush Type"), &Params, ToggleButtonGroupFlags_DrawVertical);
        CurrentRef = WorldEditBrushTypeButtonGroup.UiRef;
      }

      /* if (Editor->Tool == WorldEdit_Tool_Select) */
      /* { */
      /*   PushTableStart(Ui, Position_RightOf, CurrentRef); */
      /*     DoEditorUi(Ui, &Window, &Editor->SelectionFollowsCursor, CSz("SelectionFollowsCursor"), &Params); */
      /*   PushTableEnd(Ui); */
      /*   PushNewRow(Ui); */
      /* } */

      /* if (Editor->Tool == WorldEdit_Tool_Brush) */
      /* { */
      /*   Params.RelativePosition.Position   = Position_RightOf; */
      /*   Params.RelativePosition.RelativeTo = CurrentRef; */
      /*   WorldEditModeButtonGroup = DoEditorUi(Ui, &Window, &Editor->Params.Mode, CSz("Mode"), &Params, ToggleButtonGroupFlags_DrawVertical); */
      /*   CurrentRef = WorldEditModeButtonGroup.UiRef; */
      /* } */

      /* if (Editor->Tool == WorldEdit_Tool_Brush) */
      /* { */
      /*   Params = DefaultUiRenderParams_Generic; */
      /*   WorldEditModifierButtonGroup = DoEditorUi(Ui, &Window, &Editor->Params.Modifier, CSz(""), &Params, ToggleButtonGroupFlags_DrawVertical); */
      /* } */

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
  // Do tool selection actions (what happens when we change a tool selection)
  //
  //

  if (WorldEditToolButtonGroup.AnyElementClicked)
  {
    switch (Editor->Tool)
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

  DoBrushSettingsWindow(Engine, Editor->Tool, Editor->BrushType);

  //
  //
  // Edit tool interactions in the world
  //
  //

  if ( UiCapturedMouseInput(Ui) == False &&
       UiHoveredMouseInput(Ui)  == False  )
  {
    switch (Editor->Tool)
    {
      case WorldEdit_Tool_Disabled:

      case WorldEdit_Tool_Brush:
      {
        switch (Editor->BrushType)
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

                    if (Editor->BrushType == WorldEdit_BrushType_Asset)
                    {
                      world_edit_shape Shape =
                      {
                        type_world_update_op_shape_params_asset,
                        .world_update_op_shape_params_asset = AssetUpdateShape,
                      };
                      QueueWorldUpdateForRegion(Engine, WorldEdit_Mode_Attach, Editor->Params.Modifier, &Shape, {}, Engine->WorldUpdateMemory);
                    }
                    else if (Editor->BrushType == WorldEdit_BrushType_Entity)
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
            if (Editor->Params.Mode == WorldEdit_Mode_Paint)
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
                  v3 P0 = GetHotVoxelForEditMode(Engine, Editor->Params.Mode);
                  rect3 AABB = RectMinMax(P0, P0+1.f);
                  ApplyEditToRegion(Engine, &AABB, Engine->Editor.SelectedColorIndex,  Editor->Params.Mode, Editor->Params.Modifier);
                }
              }
            }
          } break;

          /* case WorldEdit_BrushType_Shape: */
          /* case WorldEdit_BrushType_Noise: */
          case WorldEdit_BrushType_Layered:
          {
            if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
            {
              world_chunk *Chunk = 0;
              v3i Offset = V3i(s32_MAX);
              switch (Editor->BrushType)
              {
                InvalidCase(WorldEdit_BrushType_Disabled);
                InvalidCase(WorldEdit_BrushType_Selection);
                InvalidCase(WorldEdit_BrushType_Single);
                InvalidCase(WorldEdit_BrushType_Asset);
                InvalidCase(WorldEdit_BrushType_Entity);

                case WorldEdit_BrushType_Layered:
                {
                  Chunk = &Editor->LayeredBrushEditor.Preview.Chunk;
                  RangeIterator(LayerIndex, Editor->LayeredBrushEditor.LayerCount)
                  {
                    brush_layer *Layer = Editor->LayeredBrushEditor.Layers + LayerIndex;
                    Offset = Min(Layer->Settings.Offset.Min, Offset);
                  }

                } break;

#if 0
                case WorldEdit_BrushType_Noise:
                {
                  Chunk = &Editor->Noise.Preview.Chunk;
                  Offset = Editor->Noise.Settings.Offset.Min;
                } break;

                case WorldEdit_BrushType_Shape:
                {
                  Chunk = &Editor->Shape.Preview.Chunk;

                  switch (Editor->Shape.Settings.Shape.Type)
                  {
                    case ShapeType_None: { InvalidCodePath(); } break;

                    case ShapeType_Sphere:
                    {
                      Offset = V3i(V3(GetSelectionDim(World, Editor))/2.f - V3(Editor->Shape.Settings.Shape.Sphere.Radius));
                    } break;

                    case ShapeType_Rect:
                    {
                      Offset = {};
                    } break;
                  }
                } break;
#endif
              }


              chunk_data D = {Chunk->Flags, Chunk->Dim, Chunk->Voxels, Chunk->VoxelLighting};
              world_update_op_shape_params_chunk_data ChunkDataShape = { D, V3(Offset) + GetSimSpaceP(World, Editor->SelectionRegion.Min) };

              world_edit_shape Shape =
              {
                type_world_update_op_shape_params_chunk_data,
                .world_update_op_shape_params_chunk_data = ChunkDataShape,
              };
              QueueWorldUpdateForRegion(Engine, Editor->Params.Mode, Editor->Params.Modifier, &Shape, Editor->SelectedColorIndex, Engine->WorldUpdateMemory);
            }
          } break;

          case WorldEdit_BrushType_Selection:
          {
            if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
            {
              ApplyEditToRegion(Engine, &SelectionAABB, Engine->Editor.SelectedColorIndex, Editor->Params.Mode, Editor->Params.Modifier);
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

              if (Editor->PreviousTool)
              {
                Editor->Tool = Editor->PreviousTool;
                Editor->PreviousTool = {};
              }
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

            if (Editor->PreviousTool)
            {
              Editor->Tool = Editor->PreviousTool;
              Editor->PreviousTool = {};
            }
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

              world_edit_shape Shape =
              {
                type_world_update_op_shape_params_asset,
                .world_update_op_shape_params_asset = AssetUpdateShape,
              };
              QueueWorldUpdateForRegion(Engine, WorldEdit_Mode_Attach, Editor->Params.Modifier, &Shape, {}, Engine->WorldUpdateMemory);
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
          world_edit_shape Shape = {
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




  //
  //
  // Hotkeys
  //
  //


  if (Input->Ctrl.Pressed || Input->Shift.Pressed) { Ui->RequestedForceCapture = True; }

  if (Input->Ctrl.Pressed && Input->S.Clicked) { Editor->PreviousTool = Editor->Tool; Editor->Tool = WorldEdit_Tool_Select; ResetSelection(Editor); }

  if (Input->Ctrl.Pressed && Input->E.Clicked) { Editor->PreviousTool = Editor->Tool; Editor->Tool = WorldEdit_Tool_Eyedropper; ResetSelectionIfIncomplete(Editor); }

  if (Input->Ctrl.Pressed && Input->G.Clicked) { if (entity *Ghost = GetCameraGhost(Engine)) { Ghost->P = GetSelectionCenterP(World, Editor); } }

  if (Editor->SelectionClicks == 2)
  {
    if (Input->Ctrl.Pressed && Input->D.Clicked) { ApplyEditToRegion(Engine, &SelectionAABB, {}, WorldEdit_Mode_Remove, WorldEdit_Modifier_Default); }

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

      world_edit_shape Shape =
      {
        type_world_update_op_shape_params_chunk_data,
        .world_update_op_shape_params_chunk_data = ChunkDataShape,
      };
      QueueWorldUpdateForRegion(Engine, WorldEdit_Mode_Attach, WorldEdit_Modifier_Default, &Shape, {}, Engine->WorldUpdateMemory);
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
    v3 HotVoxel = GetHotVoxelForEditMode(Engine, Editor->Params.Mode);
    DEBUG_HighlightVoxel( Engine, HotVoxel, RED, 0.075f);
  }
}
