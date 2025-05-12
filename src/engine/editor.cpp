
link_internal void
LoadBrushFromFile(level_editor *Editor, file_traversal_node *FileNode, memory_arena *TempMemory)
{
  cs Filename = Concat(FileNode->Dir, CSz("/"), FileNode->Name, TempMemory);

  u8_cursor Bytes = BeginDeserialization(Filename, TempMemory);

  world_edit_brush B = {};
  CopyString( FileNode->Name.Start, B.NameBuf, Min(umm(FileNode->Name.Count), umm(NameBuf_Len)));
  Editor->CurrentBrush = Upsert(B, &Editor->LoadedBrushes, &Global_PermMemory);

  if (Deserialize(&Bytes, Editor->CurrentBrush, TempMemory) == False)
  {
    SoftError("While deserializing brush (%S).", Filename);
    *Editor->CurrentBrush = {};
  }
  FinalizeDeserialization(&Bytes);
}

link_internal void
NewLayer(level_editor *Editor)
{
  Editor->CurrentLayer = Push(&Editor->Layers);
  Editor->CurrentLayer->Edits = WorldEditBlockArray(Editor->Memory);

  cs DefaultName = FSz("layer_%d", Editor->NextLayerIndex++);
  CopyString(DefaultName.Start, Editor->CurrentLayer->NameBuf, DefaultName.Count);
}

link_internal b32
InitEditor(level_editor *Editor)
{
  b32 Result = True;
  Editor->Memory = AllocateArena();

  Editor->Layers = WorldEditLayerBlockArray(Editor->Memory);
  Assert(Editor->NextLayerIndex == 0);
  NewLayer(Editor);

  Editor->AssetThumbnails = AssetThumbnailBlockArray(Editor->Memory);

  Editor->LoadedBrushes = Allocate_world_edit_brush_hashtable(128, Editor->Memory);

  file_traversal_node_block_array Nodes = GetLexicographicallySortedListOfFilesInDirectory(CSz("brushes"), GetTranArena());

  IterateOver(&Nodes, Node, NodeIndex)
  {
    LoadBrushFromFile(Editor, Node, GetTranArena());
  }

/*   RangeIterator(LayerIndex, MAX_BRUSH_LAYERS) */
/*   { */
/*     Editor->LayeredBrush.LayerPreviews[LayerIndex].Thumbnail.Texture.Dim = V2i(BRUSH_PREVIEW_TEXTURE_DIM); */
/*   } */
/*   Editor->LayeredBrush.SeedLayer.Thumbnail.Texture.Dim = V2i(BRUSH_PREVIEW_TEXTURE_DIM); */
/*   Editor->LayeredBrush.Preview.Thumbnail.Texture.Dim = V2i(BRUSH_PREVIEW_TEXTURE_DIM); */

  return Result;
}

link_internal b32
HardResetEditor(level_editor *Editor)
{
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
  /* Clear(Editor); */
  *Editor = {};

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


poof(string_and_value_tables(visible_region_size))
#include <generated/string_and_value_tables_visible_region_size.h>
poof(do_editor_ui_for_enum(visible_region_size))
#include <generated/do_editor_ui_for_enum_visible_region_size.h>


poof(do_editor_ui_for_compound_type(white_noise_params))
#include <generated/do_editor_ui_for_compound_type_white_noise_params.h>
poof(do_editor_ui_for_compound_type(perlin_noise_params))
#include <generated/do_editor_ui_for_compound_type_perlin_noise_params.h>
poof(do_editor_ui_for_compound_type(voronoi_noise_params))
#include <generated/do_editor_ui_for_compound_type_voronoi_noise_params.h>
poof(do_editor_ui_for_compound_type(noise_layer))
#include <generated/do_editor_ui_for_compound_type_noise_layer.h>


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

      /* DoEditorUi(Ui, Window, Element->Next, CSz("shader_uniform Next"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
    }
  }
  else
  {
    PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}
poof(do_editor_ui_for_container(shader_uniform_buffer))
#include <generated/do_editor_ui_for_container_struct.h>

poof(do_editor_ui_for_compound_type(shader))
#include <generated/do_editor_ui_for_compound_type_shader.h>

poof(do_editor_ui_for_container(shader_ptr_block_array))
#include <generated/do_editor_ui_for_container_shader_ptr_block_array.h>



poof(do_editor_ui_for_compound_type(work_queue_entry))
#include <generated/do_editor_ui_for_compound_type_work_queue_entry.h>

poof(do_editor_ui_for_compound_type(work_queue))
#include <generated/do_editor_ui_for_compound_type_work_queue.h>

poof(do_editor_ui_for_compound_type(input_event))
#include <generated/do_editor_ui_for_compound_type_input_event.h>

poof(do_editor_ui_for_compound_type(input))
#include <generated/do_editor_ui_for_compound_type_input.h>

poof(do_editor_ui_for_compound_type(platform))
#include <generated/do_editor_ui_for_compound_type_platform.h>

#if BONSAI_DEBUG_SYSTEM_API
poof(do_editor_ui_for_compound_type(debug_state))
#include <generated/do_editor_ui_for_compound_type_debug_state.h>
#endif

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



poof(string_and_value_tables(ui_layer_toolbar_actions))
#include <generated/string_and_value_tables_ui_layer_toolbar_actions.h>
poof(toolbar_for_enum(ui_layer_toolbar_actions))
#include <generated/toolbar_for_enum_ui_layer_toolbar_actions.h>

poof(string_and_value_tables(ui_brush_layer_actions))
#include <generated/string_and_value_tables_enum_ui_brush_layer_actions.h>
poof(toolbar_for_enum(ui_brush_layer_actions))
#include <generated/toolbar_for_enum_ui_brush_layer_actions.h>




poof(do_editor_ui_for_compound_type(brush_layer))
#include <generated/do_editor_ui_for_compound_type_brush_layer.h>

poof(do_editor_ui_for_compound_type(layered_brush))
#include <generated/do_editor_ui_for_compound_type_layered_brush.h>

poof(do_editor_ui_for_compound_type(world_edit_brush))
#include <generated/do_editor_ui_for_compound_type_struct_world_edit_brush.h>

poof(do_editor_ui_for_compound_type(world_edit))
#include <generated/do_editor_ui_for_compound_type_struct_world_edit.h>

poof(do_editor_ui_for_container(world_edit_ptr_block_array))
#include <generated/do_editor_ui_for_compound_type_world_edit_paged_list.h>

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

poof(do_editor_ui_for_compound_type(gpu_mapped_element_buffer))
#include <generated/do_editor_ui_for_compound_type_gpu_mapped_element_buffer.h>

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

poof(do_editor_ui_for_compound_type(octree_node))
#include <generated/do_editor_ui_for_compound_type_octree_node.h>

poof(do_editor_ui_for_compound_type(world))
#include <generated/do_editor_ui_for_compound_type_world.h>

// NOTE(Jesse): Had to hack this slightly because the asset_load_state on Enitity is marked volatile
/* poof(do_editor_ui_for_enum(asset_load_state)) */
#include <generated/do_editor_ui_for_enum_asset_load_state.h>

poof(do_editor_ui_for_compound_type(asset_id))
#include <generated/do_editor_ui_for_compound_type_asset_id.h>

poof(string_and_value_tables(asset_type))
#include <generated/string_and_value_tables_asset_type.h>
poof(do_editor_ui_for_enum(asset_type))
#include <generated/do_editor_ui_for_enum_asset_type.h>

poof(do_editor_ui_for_compound_type(asset))
#include <generated/do_editor_ui_for_compound_type_asset.h>

poof(do_editor_ui_for_compound_type(collision_event))
#include <generated/do_editor_ui_for_compound_type_collision_event.h>

poof(do_editor_ui_for_compound_type(entity_position_info))
#include <generated/do_editor_ui_for_compound_type_entity_position_info.h>






poof(do_editor_ui_for_compound_type(render_buffers_2d))
#include <generated/do_editor_ui_for_compound_type_render_buffers_2d.h>

poof(do_editor_ui_for_compound_type(renderer_2d))
#include <generated/do_editor_ui_for_compound_type_renderer_2d.h>

poof(do_editor_ui_for_compound_type(game_lights))
#include <generated/do_editor_ui_for_compound_type_game_lights.h>

poof(do_editor_ui_for_compound_type(lighting_render_group))
#include <generated/do_editor_ui_for_compound_type_lighting_render_group.h>

poof(do_editor_ui_for_compound_type(g_buffer_render_group))
#include <generated/do_editor_ui_for_compound_type_g_buffer_render_group.h>

poof(do_editor_ui_for_compound_type(terrain_shaping_render_context))
#include <generated/do_editor_ui_for_compound_type_struct_terrain_shaping_render_context.h>

poof(do_editor_ui_for_compound_type(terrain_decoration_render_context))
#include <generated/do_editor_ui_for_compound_type_struct_terrain_decoration_render_context.h>

poof(do_editor_ui_for_compound_type(terrain_finalize_render_context))
#include <generated/do_editor_ui_for_compound_type_struct_terrain_finalize_render_context.h>
  
poof(do_editor_ui_for_compound_type(world_edit_render_context))
#include <generated/do_editor_ui_for_compound_type_struct_world_edit_render_context.h>

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

#if 0
link_internal void
ApplyEditToRegion(engine_resources *Engine, rect3 *SelectionAABB, v3 HSVColor, b32 PersistWhitespace, world_edit_blend_mode WorldEditMode, world_edit_blend_mode_modifier Modifier)
{
  world_edit_shape Shape = {
    .Type = type_world_update_op_shape_params_rect,
    .world_update_op_shape_params_rect.Region = *SelectionAABB
  };

  QueueWorldUpdateForRegion(Engine, WorldEditMode, Modifier, &Shape, HSVColor, PersistWhitespace, Engine->WorldUpdateMemory);
}
#endif


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
HighlightFace(engine_resources *Engine, face_index Face, aabb SelectionAABB, r32 InsetWidth, v3 RGBHighlightColor, r32 HiThickness)
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
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, RGBHighlightColor, HiThickness );
    } break;

    case FaceIndex_Bot:
    {
      v3 HighlightInset = V3(InsetWidth, InsetWidth, 0.f);
      v3 MinHiP = SelectionAABB.Min + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - (SelectionAABBRadius*V3(0.f, 0.f, 2.f)) - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, RGBHighlightColor, HiThickness );
    } break;

    case FaceIndex_Left:
    {
      v3 HighlightInset = V3(0.f, InsetWidth, InsetWidth);
      v3 MinHiP = SelectionAABB.Min + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - (SelectionAABBRadius*V3(2.f, 0.f, 0.f)) - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, RGBHighlightColor, HiThickness );
    } break;

    case FaceIndex_Right:
    {
      v3 HighlightInset = V3(0.f, InsetWidth, InsetWidth);
      v3 MinHiP = SelectionAABB.Min + (SelectionAABBRadius*V3(2.f, 0.f, 0.f)) + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, RGBHighlightColor, HiThickness );
    } break;

    case FaceIndex_Back:
    {
      v3 HighlightInset = V3(InsetWidth, 0.f, InsetWidth);
      v3 MinHiP = SelectionAABB.Min + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - (SelectionAABBRadius*V3(0.f, 2.f, 0.f)) - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, RGBHighlightColor, HiThickness );
    } break;

    case FaceIndex_Front:
    {
      v3 HighlightInset = V3(InsetWidth, 0.f, InsetWidth);
      v3 MinHiP = SelectionAABB.Min + (SelectionAABBRadius*V3(0.f, 2.f, 0.f)) + HighlightInset;
      v3 MaxHiP = SelectionAABB.Max - HighlightInset;
      DEBUG_DrawSimSpaceAABB(Engine, MinHiP, MaxHiP, RGBHighlightColor, HiThickness );
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
    /* DEBUG_HighlightVoxel(Engine, PlaneIntersect, RED); */
    if (Input->LMB.Pressed)
    {
      SelectionState->ClickedP[1] = PlaneIntersect;
    }
  }


  v3 RoughUpdateVector = (SelectionState->ClickedP[1] - SelectionState->ClickedP[0]);

  v3 UpdateVector = ConstrainUpdateVector(RoughUpdateVector, SelectionState->ClickedFace, SelectionMode);
          Result  = ModifySelectionAABB(&SelectionAABB, V3i(UpdateVector), SelectionState->ClickedFace, SelectionMode);

  {
    /* DEBUG_HighlightVoxel(Engine, SelectionState->ClickedP[0], RED); */
    /* DEBUG_HighlightVoxel(Engine, SelectionState->ClickedP[1], BLUE); */
    DEBUG_DrawSimSpaceVectorAt(Engine, SelectionState->ClickedP[0], UpdateVector, RGB_GREEN);
  }

  // Draw selection modification region
  //
  rect3 Draw = Rect3(&Result);
  DEBUG_DrawSimSpaceAABB(Engine, &Draw, RGB_GREEN, 0.1f);

  Assert(Result.Min <= Result.Max);

  return Result;
}

link_internal void
DoSelectedVoxelDebugWindow(engine_resources *Engine, cp VoxelCP)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  voxel *V = TryGetVoxelPointer(World, VoxelCP);
  v3 SimP = Floor(GetSimSpaceP(World, VoxelCP));

  {
    local_persist window_layout Window = WindowLayout("Voxel Debug Window", V2(150.f, 150.f));
    PushWindowStart(Ui, &Window);

    if (V)
    {
      /* DoEditorUi(Ui, &Window, &V->Flags, CSz("Voxel Flags")); */
      /* PushNewRow(Ui); */

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
InteractWithThumbnailTexture(engine_resources *Engine, renderer_2d *Ui, window_layout *Window, const char* InteractionString, asset_thumbnail *Thumb)
{
  texture *Texture = &Thumb->Texture;
  camera  *ThumbCamera  = &Thumb->Camera;

  // TODO(Jesse)(leak): How do we deallocate these on hard reset?
  // @hard_reset_texture_memory
  if (Texture->ID == 0 && Texture->Queued == False)
  {
    Assert(Texture->Dim > V2i(0));
    Texture->Queued = True;
    /* PushBonsaiRenderCommandAllocateTexture(&Engine->Plat->RenderQ, Texture); */
    MakeTexture_RGB_Async(&Engine->Stdlib.Plat.RenderQ, Texture, Texture->Dim, 0, CSz("NoisePreviewTexture"));
    /* *Texture = MakeTexture_RGB(V2i(256), 0, CSz("NoisePreviewTexture")); */
    StandardCamera(ThumbCamera, 10000.f, 500.f, 30.f);
  }

  interactable_handle B = PushButtonStart(Ui, UiId(Window, InteractionString, Cast(void*, Thumb)));
    u32 Index = StartColumn(Ui, &DefaultUiRenderParams_Blank);
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
  }
  UpdateGameCamera(GetWorld(), MouseDP, CameraZDelta, {}, ThumbCamera, 0.f);

  /* if (Ui->Input->LMB.Pressed == False && Hover(Ui, &B)) { PushTooltip(Ui, ToString(Texture)); } */

  return B;
}
#endif


link_internal v3
GetHotVoxelForEditMode(engine_resources *Engine, world_edit_blend_mode WorldEditMode)
{
  picked_voxel_position Pos = PickedVoxel_FirstFilled;

  switch (WorldEditMode)
  {
    case WorldEdit_Mode_Disabled: {} break;
    case WorldEdit_Mode_Threshold:
    case WorldEdit_Mode_Additive:
    {
      Pos = PickedVoxel_LastEmpty;
    } break;

    /* case WorldEdit_Mode_Paint: */
    case WorldEdit_Mode_Subtractive:
    {
      Pos = PickedVoxel_FirstFilled;
    } break;
  }

  v3 Result = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, Pos));
  return Result;
}

link_internal v3
GetHotVoxelForFlood(engine_resources *Engine, world_edit_blend_mode WorldEditMode, world_edit_blend_mode_modifier Modifier)
{
  v3 Result = {};
  picked_voxel_position Pos = {};

  NotImplemented;

#if 0
  if (Modifier == WorldEdit_Modifier_Flood)
  {
    switch (WorldEditMode)
    {
      case WorldEdit_Mode_Disabled: {} break;
      case WorldEdit_Mode_Additive:
      {
        Pos = PickedVoxel_FirstFilled;
      } break;

      /* case WorldEdit_Mode_Paint: */
      case WorldEdit_Mode_Subtractive:
      {
        Pos = PickedVoxel_LastEmpty;
      } break;
    }

    Result = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, Pos));
  }
#endif

  return Result;
}

link_internal v3i
GetShapeDim(shape_layer *Layer)
{
  v3i Result = {};
  switch (Layer->Type)
  {
    case ShapeType_Cylinder: { NotImplemented; } break;
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

      shape_layer *Shape = &Layer->Settings.Shape;
      switch (Shape->Type)
      {
        case ShapeType_Cylinder: { NotImplemented; } break;
        case ShapeType_Sphere:
        {
          Request += V3i(Shape->Sphere.Radius*2.f);
        } break;

        case ShapeType_Rect:
        {
          Request += SelectionDim;
        } break;
      }
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
      Result += GetSelectionDim(GetWorld(), GetLevelEditor()); //Preview->Chunk.Dim;
    } break;
  }
  return Result;
}

link_internal b32
CheckForChangesAndUpdate_ThenRenderToPreviewTexture(engine_resources *Engine, brush_layer *Layer, chunk_thumbnail *Preview)
{
  NotImplemented;
  return False;
#if 0
  UNPACK_ENGINE_RESOURCES(Engine);

  // Leaving this function here as a reference for what used to happen, but migrating the functionality
  NotImplemented;

  brush_settings *Settings     = &Layer->Settings;
  brush_settings *PrevSettings = &Layer->PrevSettings;

  v3i SelectionDim     = GetSelectionDim(World, Editor);
  v3i RequiredLayerDim = GetRequiredDimForLayer(SelectionDim, Layer);

  b32 ReallocChunk     = Editor->Selection.Changed || Preview->Chunk.Dim != RequiredLayerDim;
  b32 SettingsChanged  = !AreEqual(Settings, PrevSettings);
  b32 UpdateVoxels     = ReallocChunk || SettingsChanged;

  *PrevSettings = *Settings;



  world_chunk *Chunk = &Preview->Chunk;
  if (ReallocChunk)
  {
    // TODO(Jesse)(leak, stability): Figure out exactly how this works.  We
    // can't allocate from the Editor memory pool because the goemetry buffers
    // get freed to a freelist, and the editor memory pool gets cleared on game reload
    //
    // @editor_chunk_memory_question
    //
    /* DeallocateWorldChunk(Chunk, MeshFreelist); */
    /* DeallocateGpuBuffers(RenderQ, Chunk); */
    PushDeallocateBuffersCommand(RenderQ, &Chunk->Mesh.Handles);
    AllocateWorldChunk(Chunk, {}, RequiredLayerDim, {}, Editor->Memory);
  }

  if (UpdateVoxels)
  {
    /* Info("Detected changes to settings, updating voxels. ReallocChunk(%b) SettingsChanged(%b)", ReallocChunk, SettingsChanged); */

    switch (Settings->Type)
    {
      case BrushLayerType_Shape:
      {
        shape_layer *Shape = &Settings->Shape;
        switch (Shape->Type)
        {

          case ShapeType_Rect:
          {
            rect3 Region = RectMinDim({}, V3(SelectionDim));

            Region.Min += V3(Settings->Offset.Min);
            Region.Max += V3(Settings->Offset.Max);

            Sanitize(&Region);

            Shape->Rect.Region.Min = Min(Region.Min, Region.Max);
            Shape->Rect.Region.Max = Max(Region.Min, Region.Max);

            Assert(GetDim(Shape->Rect.Region) == V3(RequiredLayerDim));
            Assert(Chunk->Dim == RequiredLayerDim);

          } break;

          case ShapeType_Cylinder: { NotImplemented; } break;
          case ShapeType_Sphere:
          {
            // NOTE(Jesse): Constrain maximum sphere radius to minimum selection dimension
            r32 MaxSphereRadius = Min(Min(SelectionDim.x, SelectionDim.y), SelectionDim.z)/2.f;
            Shape->Sphere.Radius = MaxSphereRadius;
            Shape->Sphere.Location = Canonical_Position(V3(MaxSphereRadius), {});

          } break;
        }

        if (LengthSq(GetShapeDim(Shape)) > 0)
        {
          NotImplemented;
          /* ApplyBrushLayer(Engine, Layer, Preview, Chunk, Settings->Offset.Min); */
          /* FinalizeChunkInitialization(Chunk); */
          /* QueueChunkForMeshRebuild(&Plat->LowPriority, Chunk); */
        }

      } break;

      case BrushLayerType_Noise:
      {
        noise_layer *Noise = &Settings->Noise;
        chunk_init_callback NoiseFunc = {};

        generic_noise_params NoiseParams = {};
        void *UserData = {};

        NoiseParams.RGBColor      = HSVtoRGB(Settings->HSVColor);
        switch (Noise->Type)
        {
          case NoiseType_White:
          {
            NoiseFunc             = Terrain_WhiteNoise;
            NoiseParams.Threshold = Noise->White.Threshold;
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
                                  Settings->NoiseBasisOffset + V3i(GetAbsoluteP(Editor->Selection.Region.Min, GetWorldChunkDim())));
      } break;
    }

    // TODO(Jesse): Do we do this here still?
    NotImplemented;
    /* SyncGpuBuffersAsync(Engine, &Chunk->Meshes); */
  }

  if (Preview->Thumbnail.Texture.ID) //  NOTE(Jesse): Avoid spamming a warning to console
  {
    RenderToTexture_Async(&Plat->RenderQ, Engine, &Preview->Thumbnail, &Chunk->Mesh, V3(Chunk->Dim)/-2.f, 0);
  }

  return UpdateVoxels;
#endif
}

link_internal void
BrushSettingsForShapeBrush(engine_resources *Engine, window_layout *Window, shape_layer *Layer)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  DoEditorUi(Ui, Window, &Layer->Type, CSz("ShapeType"));

  v3 SelectionDim = GetDim(GetSelectionRect(World, Editor));
  switch (Layer->Type)
  {
    case ShapeType_Rect:
    {
      DoEditorUi(Ui, Window, &Layer->Rect, CSz(""));
    } break;

    case ShapeType_Cylinder: { NotImplemented; } break;
    case ShapeType_Sphere:
    {
      DoEditorUi(Ui, Window, &Layer->Sphere, CSz(""));
    } break;
  }

}

link_internal void
BrushSettingsForNoiseBrush(engine_resources *Engine, window_layout *Window, noise_layer *Layer)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  PushTableStart(Ui);
    if (SelectionComplete(Editor->Selection.Clicks))
    {
      DoEditorUi(Ui, Window, &Layer->Type, CSz("NoiseType"), &DefaultUiRenderParams_Generic);
      PushTableStart(Ui); // TODO(Jesse): Necessary?
        switch (Layer->Type)
        {
          case NoiseType_White:
          {
            DoEditorUi(Ui, Window, &Layer->White, CSz(""));
          } break;

          case NoiseType_Perlin:
          {
            DoEditorUi(Ui, Window, &Layer->Perlin, CSz(""));
          } break;

          case NoiseType_Voronoi:
          {
            DoEditorUi(Ui, Window, &Layer->Voronoi, CSz(""));
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

#if 0
link_internal void
DoSettingsForBrushLayer(engine_resources *Engine, brush_layer *Layer, window_layout *Window)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  /* brush_settings *Settings = &Layer->Settings; */


  OPEN_INDENT_FOR_TOGGLEABLE_REGION();

  PushNewRow(Ui);
  DoEditorUi(Ui, Window, &Settings->Type, CSz("Type"), &DefaultUiRenderParams_Generic);


  switch (Layer->Settings.Type)
  {
    case BrushLayerType_Noise:
    {
      BrushSettingsForNoiseBrush(Engine, Window, &Settings->Noise);
    } break;

    case BrushLayerType_Shape:
    {
      BrushSettingsForShapeBrush(Engine, Window, &Settings->Shape);
    } break;
  }


  // TODO(Jesse): do enum selector for Mode/Modifier/iterations
  DoEditorUi(Ui, Window, &Settings->Mode,       CSz("Mode"));
  DoEditorUi(Ui, Window, &Settings->Modifier,   CSz("Modifier"));
  DoEditorUi(Ui, Window, &Settings->ColorMode,   CSz("ColorMode"));
  if (Settings->Modifier == WorldEdit_ValueModifier_Surface) // || Settings->Modifier == WorldEdit_Modifier_Flood)
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
    ui_style Style = UiStyleFromLightestColor(HSVtoRGB(Settings->HSVColor));
    PushUntexturedQuad(Ui, {}, V2(Global_Font.Size.y), zDepth_Text, &Style, DefaultGenericPadding);


    ui_id ColorPickerModalId = UiId(Window, "color modal interaction", Cast(void*, Settings));

    if (Button(Ui, CSz("Set Color"), UiId(Window, "set color interaction", Cast(void*, Settings))))
    {
      ToggleModal(Ui, "Color Picker", ColorPickerModalId);
    }
    PushNewRow(Ui);

    ColorPickerModal(Engine, ColorPickerModalId, &Settings->HSVColor, False);

    PushNewRow(Ui);
  }

  /* PushTableStart(Ui); */
    /* InteractWithThumbnailTexture(Engine, Ui, Window, "noise preview interaction", &Preview->Thumbnail); */
    /* PushNewRow(Ui); */
  /* PushTableEnd(Ui); */
  CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
}
#endif

#if 0
link_internal void
ApplyBrushLayer(engine_resources *Engine, brush_layer *Layer, chunk_thumbnail *Preview, world_chunk *DestChunk, v3i SmallestMinOffset)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  if (LengthSq(DestChunk->Dim) > 0)
  {
    brush_settings *Settings = &Layer->Settings;

    world_edit_blend_mode              Mode = Settings->Mode;
    world_edit_blend_mode_modifier Modifier = Settings->Modifier;


    rect3i UpdateBounds = {{}, DestChunk->Dim};

    world_edit_shape Shape = {};
    switch (Settings->Type)
    {
      case BrushLayerType_Shape:
      {
        Shape.Type = world_update_op_shape_type(Settings->Shape.Type);

        switch (Settings->Shape.Type)
        {
          case ShapeType_Cylinder: { NotImplemented; } break;
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
        world_chunk  *SrcChunk = &Preview->Chunk;
        v3i       SrcOffsetMin = Settings->Offset.Min;

        v3i DestRelativeMinCorner = (-1*SmallestMinOffset) + SrcOffsetMin;

        chunk_data D = {SrcChunk->Flags, SrcChunk->Dim, SrcChunk->Occupancy, SrcChunk->xOccupancyBorder, SrcChunk->FaceMasks, SrcChunk->Voxels, SrcChunk->VoxelLighting};
        world_update_op_shape_params_chunk_data ChunkDataShape = { D, V3(DestRelativeMinCorner) };

        Assert(SrcChunk->Dim <= DestChunk->Dim);

        Shape.Type = type_world_update_op_shape_params_chunk_data;
        Shape.world_update_op_shape_params_chunk_data = ChunkDataShape;
      } break;
    }

    v3 SimFloodOrigin = V3(0);

    s32 Iterations = Settings->Iterations;
    if (Iterations > 1) { Info("%d", Iterations); }
    RangeIterator(IterIndex, Iterations)
    {
      NotImplemented;
#if 0
      work_queue_entry_update_world_region Job = WorkQueueEntryUpdateWorldRegion(Mode, Modifier, SimFloodOrigin, &Shape, HSVtoRGB(Layer->Settings.HSVColor), {}, {}, {}, {}, 0);
      ApplyUpdateToRegion(GetThreadLocalState(ThreadLocal_ThreadIndex), &Job, UpdateBounds, DestChunk, Layer->Settings.Invert);
      DestChunk->FilledCount = MarkBoundaryVoxels_MakeExteriorFaces( DestChunk->Occupancy, DestChunk->Voxels, DestChunk->Dim, {{}}, DestChunk->Dim );
#endif
    }
  }

}
#endif

link_internal v3i
GetSmallestMinOffset(layered_brush *LayeredBrush, v3i *LargestLayerDim)
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
SaveBrush(world_edit_brush *Brush, const char *FilenameZ)
{
  u8_cursor_block_array OutputStream = BeginSerialization();
  Serialize(&OutputStream, Brush);

  if (FinalizeSerialization(&OutputStream, FilenameZ) == False)
  {
    SoftError("Unable to serialize brush (%s) to file (%s).", Brush->NameBuf, FilenameZ);
  }
  else
  {
    ZeroMemory(Brush->NameBuf, NameBuf_Len);
    cs BrushNameBuf = CS(Brush->NameBuf, NameBuf_Len);

    cs BrushBasename = Basename(CS(FilenameZ));
    CopyString(&BrushBasename, &BrushNameBuf);
  }
}


link_internal void
NewBrush(world_edit_brush *Brush)
{
  cs BrushNameBuf = CS(Brush->NameBuf, NameBuf_Len);

  ZeroMemory(Brush->NameBuf, NameBuf_Len);

  cs Src = CSz("_untitled.brush");
  CopyString(&Src, &BrushNameBuf);

  brush_layer *Layers =  Brush->Layered.Layers;
  RangeIterator(LayerIndex, MAX_BRUSH_LAYERS)
  {
    brush_layer *Layer = Layers + LayerIndex;
    Layer->Settings = {};
  }

  Brush->Layered.LayerCount = 1;

  // Initialize PrevSettings so we don't fire a changed event straight away..
  // @prevent_change_event
  CheckSettingsChanged(&Brush->Layered);
}

link_internal cs
GetLayerUiText(brush_layer *Layer, memory_arena *TempMem)
{
  cs LayerType = ToStringPrefixless(Layer->Settings.Type);
  cs SubType = {};
  switch (Layer->Settings.Type)
  {
    case BrushLayerType_Noise: { SubType = ToStringPrefixless(Layer->Settings.Noise.Type); } break;
    case BrushLayerType_Shape: { SubType = ToStringPrefixless(Layer->Settings.Shape.Type); } break;
  }

  return FSz("%S(%S)", LayerType, SubType);
}

link_internal void
DoColorSwatch(renderer_2d *Ui, v2 QuadDim, v3 RGB)
{
  ui_style Style = FlatUiStyle(RGB);
  PushUntexturedQuad(Ui, {}, QuadDim, zDepth_Text, &Style, {} );
  PushNewRow(Ui);
}

link_internal void
DoBrushSettingsWindow(engine_resources *Engine, world_edit_brush *Brush, window_layout *BrushSettingsWindow)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  layered_brush *LayeredBrush = &Brush->Layered;
  b32 IsNewBrush = False;
#if 1
  {
    cs BrushNameBuf = CS(Brush->NameBuf, NameBuf_Len);
    if (Brush->NameBuf[0] == 0 && Brush->Layered.LayerCount == 0)
    {
      NewBrush(Brush);
      IsNewBrush = True;
    }
  }
#endif

  //
  // Brush toolbar buttons
  //
#if 1
  {
    PushWindowStart(Ui, BrushSettingsWindow);
    memory_arena *Tran = GetTranArena();

    if (LayeredBrush->LayerCount)
    {
      if (Button(Ui, CSz("Save"), UiId(BrushSettingsWindow, "brush save", 0u)))
      {
        cs BrushFilepath = GetFilenameForBrush(CS(Brush->NameBuf));
        SaveBrush(Brush, BrushFilepath.Start);
      }

      if (Button(Ui, CSz("Duplicate"), UiId(BrushSettingsWindow, "brush dup", 0u)))
      {
        cs_buffer Pieces = Split( CS(Brush->NameBuf), '.', Tran);

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

            SaveBrush(Brush, BrushFilepath.Start);
          }
        }
        else
        {
          cs BrushFilepath = GetFilenameForBrush(CS(Brush->NameBuf), 1);
          SaveBrush(Brush, BrushFilepath.Start);
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
        LoadBrushFromFile(Editor, &ClickedFileNode.Value, Tran);
        SetToggleButton(Ui, ImportToggleId, False);
      }
    }
    else
    {
      if (Button(Ui, CSz("New"), UiId(BrushSettingsWindow, "brush new", 0u)))
      {
        NewBrush(Brush);
        IsNewBrush = True;
      }

      if (LayeredBrush->LayerCount)
      {
        PushNewRow(Ui);
        PushNewRow(Ui);

        {
          ui_id TextBoxId = UiId(BrushSettingsWindow, "name_buf_textbox", Brush->NameBuf);
          cs NameBuf = CS(Brush->NameBuf);
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
          DoEditorUi(Ui, BrushSettingsWindow, &LayeredBrush->Mode,     CSz("Mode"),     &DefaultUiRenderParams_Generic);
          DoEditorUi(Ui, BrushSettingsWindow, &LayeredBrush->Modifier, CSz("Modifier"), &DefaultUiRenderParams_Generic);
          PushNewRow(Ui);
        }

        {
          DoEditorUi(Ui, BrushSettingsWindow, &LayeredBrush->SeedBrushWithSelection, CSz("SeedBrushWithSelection"), &DefaultUiRenderParams_Checkbox);
          PushNewRow(Ui);

          DoEditorUi(Ui, BrushSettingsWindow, &LayeredBrush->BrushFollowsCursor,      CSz("BrushFollowsCursor"),      &DefaultUiRenderParams_Checkbox);
          PushNewRow(Ui);
          PushNewRow(Ui);
        }

      }

      {
        s32 EditLayerIndex = 0;

        PushTableStart(Ui);

        Ui->UiBrushLayerAction = {};

        brush_layer *Layers = Brush->Layered.Layers;
        RangeIterator(LayerIndex, LayeredBrush->LayerCount)
        {
          brush_layer *Layer = Layers + LayerIndex;

          ui_id ToggleId = UiId(BrushSettingsWindow, "brush_layer toggle interaction", u32(LayerIndex));
          cs LayerDetails = GetLayerUiText(Layer, GetTranArena());

          if (ToggleButton(Ui, FSz("v %d %S", LayerIndex, LayerDetails), FSz("> %d %S", LayerIndex, LayerDetails), ToggleId))
          {
            ui_toggle_button_group Toolbar = PushToolbar(Ui, BrushSettingsWindow, CSz(""), &Ui->UiBrushLayerAction, u64(LayerIndex));
            if (Toolbar.AnyElementClicked)
            {
              EditLayerIndex = LayerIndex;

              if (Ui->UiBrushLayerAction == UiBrushLayerAction_Delete) { SetToggleButton(Ui, ToggleId, False); }

              b32 ThisState = GetToggleState(Ui, ToggleId);

              if (Ui->UiBrushLayerAction == UiBrushLayerAction_MoveUp)
              {
                ui_id NextId = ToggleId;
                NextId.ElementBits -= 1;
                b32 NextState = GetToggleState(Ui, NextId);

                SetToggleButton(Ui, ToggleId, NextState);
                SetToggleButton(Ui, NextId, ThisState);
              }

              if (Ui->UiBrushLayerAction == UiBrushLayerAction_MoveDown)
              {
                ui_id NextId = ToggleId;
                NextId.ElementBits += 1;
                b32 NextState = GetToggleState(Ui, NextId);

                SetToggleButton(Ui, ToggleId, NextState);
                SetToggleButton(Ui, NextId, ThisState);
              }

            }

            OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              DoEditorUi(Ui, BrushSettingsWindow, Layer, {});
            CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
          }
          else
          {
            DoColorSwatch(Ui, V2(20), HSVtoRGB(Layer->Settings.HSVColor));
          }

          if (IsNewBrush && LayerIndex == 0)
          {
            SetToggleButton(Ui, ToggleId, True);
          }

          PushNewRow(Ui);
        }
        PushTableEnd(Ui);

        if (Ui->UiBrushLayerAction == UiBrushLayerAction_MoveUp)
        {
          if (EditLayerIndex > 0)
          {
            brush_layer *Layer = Layers + EditLayerIndex;
            brush_layer Tmp = Layers[EditLayerIndex-1];
            Layers[EditLayerIndex-1].Settings = Layer->Settings;
            Layer->Settings = Tmp.Settings;
          }
        }

        if (Ui->UiBrushLayerAction == UiBrushLayerAction_MoveDown)
        {
          if (LayeredBrush->LayerCount)
          {
            if (EditLayerIndex < LayeredBrush->LayerCount-1)
            {
              Info("SwippySwap");
              brush_layer *Layer = Layers + EditLayerIndex;
              brush_layer Tmp = Layers[EditLayerIndex+1];
              Layers[EditLayerIndex+1].Settings = Layer->Settings;
              Layer->Settings = Tmp.Settings;
            }
          }
        }

        if (Ui->UiBrushLayerAction == UiBrushLayerAction_Duplicate)
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

        if (Ui->UiBrushLayerAction == UiBrushLayerAction_Delete)
        {
          if (LayeredBrush->LayerCount < MAX_BRUSH_LAYERS)
          {
            // Shuffle layers backwards, overwriting EditLayerIndex
            RangeIteratorRange(LayerIndex, MAX_BRUSH_LAYERS, EditLayerIndex+1)
            {
              Assert(LayerIndex >= 0 && LayerIndex < MAX_BRUSH_LAYERS);
              Layers[LayerIndex-1].Settings = Layers[LayerIndex].Settings;
            }

            LayeredBrush->LayerCount -= 1;
          }
        }
      }
    }

  }
#endif


#if 0
  if (SelectionComplete(Editor->Selection.Clicks))
  {
    b32 AnyChanges = False;
    RangeIterator(LayerIndex, LayeredBrush->LayerCount)
    {
      brush_layer *Layer = Layers + LayerIndex;
      chunk_thumbnail *Preview = Previews + LayerIndex;
      AnyChanges |= CheckForChangesAndUpdate_ThenRenderToPreviewTexture(Engine, Layer, Preview);
    }

    if (AnyChanges)
    {
      Editor->RootChunkNeedsNewMesh = True;
      Editor->MostRecentSelectionRegionMin = Editor->SelectionRegion.Min;
    }
  }
#endif




  {
    /* local_persist window_layout LayersWindow = WindowLayout("Layers", WindowLayoutFlag_Align_Right); */
    /* PushWindowStart(Ui, BrushSettingsWindow); */

    // NOTE(Jesse): Debug
#if 0
    if (LayeredBrush->SeedBrushWithSelection)
    {
      PushTableStart(Ui);
          world_chunk *SeedChunk = &LayeredBrush->SeedLayer.Chunk;
          chunk_thumbnail *SeedPreview = &LayeredBrush->SeedLayer;
        if (SeedPreview->Thumbnail.Texture.ID) { RenderToTexture_Async(&Plat->RenderQ, Engine, &SeedPreview->Thumbnail, &SeedChunk->Meshes, V3(SeedChunk->Dim)/-2.f, 0); }
        InteractWithThumbnailTexture(Engine, Ui, BrushSettingsWindow, "seed preview interaction", &Editor->LayeredBrush.SeedLayer.Thumbnail);
        PushNewRow(Ui);
      PushTableEnd(Ui);

      PushTableStart(Ui);
        world_chunk *Root_LayeredBrushPreview = &LayeredBrush->Preview.Chunk;
        if (SeedPreview->Thumbnail.Texture.ID) { RenderToTexture_Async(&Plat->RenderQ, Engine, &LayeredBrush->Preview.Thumbnail, &Root_LayeredBrushPreview->Meshes, V3(Root_LayeredBrushPreview->Dim)/-2.f, 0); }
        InteractWithThumbnailTexture(Engine, Ui, BrushSettingsWindow, "root preview interaction", &Editor->LayeredBrush.Preview.Thumbnail);
        PushNewRow(Ui);
      PushTableEnd(Ui);
    }
#endif

    // NOTE(Jesse): This is old code for maintaining the world edit memory.  We
    // no longer need this, but I'm keeping it around for a bit in case I want to
    // read it for some reason..
#if 0
    {

      {
        world_chunk *Root_LayeredBrushPreview = &LayeredBrush->Preview.Chunk;
        Assert( (Root_LayeredBrushPreview->Flags&Chunk_Queued) == False );

        //
        // TODO(Jesse)(async, speed): It would be kinda nice if this ran async..
        if ( Editor->RootChunkNeedsNewMesh )
        {
          // TODO(Jesse): Shouldn't the LargestLayerDim take into account the largest Offset too..?
          v3i LargestLayerDim = GetSelectionDim(World, Editor) + 2;
          v3i SmallestMinOffset = GetSmallestMinOffset(LayeredBrush, &LargestLayerDim);
          /* if (SmallestMinOffset == V3i(0)) */
          /* {SmallestMinOffset = V3i(1);} */

          // Clear the voxels if the size didn't change, otherwise realloc
          if (Root_LayeredBrushPreview->Dim == LargestLayerDim)
          {
            // TODO(Jesse): Actually necessary??  I think maybe not
            ClearChunkVoxels(Root_LayeredBrushPreview->Voxels, Root_LayeredBrushPreview->Dim);
          }
          else
          {
            // @editor_chunk_memory_question
            AllocateWorldChunk(Root_LayeredBrushPreview, {}, LargestLayerDim, {}, Editor->Memory);
          }

          world_chunk *SeedChunk = &LayeredBrush->SeedLayer.Chunk;
          if (SeedChunk->Dim == LargestLayerDim)
          {
            ClearChunkVoxels(SeedChunk->Voxels, SeedChunk->Dim);
          }
          else
          {
            AllocateWorldChunk(SeedChunk, {}, LargestLayerDim, {}, Editor->Memory);
          }




#if 0
          auto Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);
          if (LayeredBrush->SeedBrushWithSelection)
          {
            if (SelectionComplete(Editor->Selection.Clicks))
            {
              rect3cp GatherRegion = Editor->SelectionRegion;
              GatherRegion.Min.Offset -= V3(1);
              GatherRegion.Max.Offset += V3(1);
              Canonicalize(World, &GatherRegion.Min);
              Canonicalize(World, &GatherRegion.Max);

              world_chunk Seed = GatherVoxelsOverlappingArea(Engine, GatherRegion, GetTranArena());
              /* CopyChunkOffset(&Seed, Seed.Dim, Root_LayeredBrushPreview, Root_LayeredBrushPreview->Dim, -1*SmallestMinOffset); */
              CopyChunkOffset(&Seed, Seed.Dim, SeedChunk, SeedChunk->Dim, {});

              /* // NOTE(Jesse): Zero out everything on the exterior edge */
              /* RangeIterator (z, SeedChunk->Dim.z) */
              /* RangeIterator (y, SeedChunk->Dim.y) */
              /* RangeIterator (x, SeedChunk->Dim.x) */
              /* { */
              /*   if (z == 0 || y == 0 || x == 0 || */
              /*       z == (SeedChunk->Dim.z-1) || y == (SeedChunk->Dim.y-1) || x == (SeedChunk->Dim.x-1)) */
              /*   /1* if (z == 0 || y == 0 || x == 0) *1/ */
              /*   { */
              /*     s32 Index = GetIndex(x,y,z, SeedChunk->Dim); */
              /*     SeedChunk->Voxels[Index] = {}; */
              /*   } */
              /* } */

              MarkBoundaryVoxels_MakeExteriorFaces(SeedChunk->Occupancy, SeedChunk->Voxels, SeedChunk->Dim, {}, SeedChunk->Dim);

              FinalizeChunkInitialization(SeedChunk);

              data_type Type = GetMeshDatatypeForDimension(SeedChunk->Dim);
              auto *TempMesh = AllocateTempMesh(Thread->TempMemory, Type);
              RebuildWorldChunkMesh(Thread, SeedChunk, {}, SeedChunk->Dim, MeshBit_Lod0, TempMesh, Thread->TempMemory);
              // TODO(Jesse): Do we still do this here?
              NotImplemented;
              /* SyncGpuBuffersAsync(Engine, &SeedChunk->Meshes); */

              CopyChunkOffset(SeedChunk, SeedChunk->Dim, Root_LayeredBrushPreview, Root_LayeredBrushPreview->Dim, {});
            }
          }
#endif

          RangeIterator(LayerIndex, LayeredBrush->LayerCount)
          {
            brush_layer *Layer = Layers + LayerIndex;
            chunk_thumbnail *Preview = Previews + LayerIndex;
            ApplyBrushLayer(Engine, Layer, Preview, Root_LayeredBrushPreview, SmallestMinOffset+V3i(-1));
          }

          FinalizeChunkInitialization(Root_LayeredBrushPreview);

          {
            auto Chunk = Root_LayeredBrushPreview;

            data_type Type = GetMeshDatatypeForDimension(Chunk->Dim);
            auto *TempMesh = AllocateTempMesh(Thread->TempMemory, Type);

            MarkBoundaryVoxels_MakeExteriorFaces( Root_LayeredBrushPreview->Occupancy,
                                                  Root_LayeredBrushPreview->Voxels,
                                                  Root_LayeredBrushPreview->Dim,
                                                  {},
                                                  Root_LayeredBrushPreview->Dim-V3i(2));

            RebuildWorldChunkMesh(Thread, Chunk, {}, Chunk->Dim, MeshBit_Lod0, TempMesh, Thread->TempMemory);
          }

          Editor->RootChunkNeedsNewMesh = False;
          Editor->NextSelectionRegionMin = Editor->MostRecentSelectionRegionMin;

          // TODO(Jesse): Do we still do this here?
          NotImplemented;
          /* if (SyncGpuBuffersAsync(Engine, &Root_LayeredBrushPreview->Meshes)) */
          /* { */
          /*   Editor->EditorPreviewRegionMin = Editor->NextSelectionRegionMin; */
          /* } */
        }



      }
    }
#endif

    PushWindowEnd(Ui, BrushSettingsWindow);
  }
}

#if 0
link_internal void
DoBrushSettingsWindow(engine_resources *Engine, world_edit_tool WorldEditTool, world_edit_brush_type WorldEditBrushType)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  local_persist window_layout Window = WindowLayout("Brush Settings", WindowLayoutFlag_Align_Right);
  switch (WorldEditTool)
  {
    case  WorldEdit_Tool_Disabled:
    case  WorldEdit_Tool_Select:
    case  WorldEdit_Tool_Eyedropper:
    case  WorldEdit_Tool_BlitEntity:
    {
    } break;

    case WorldEdit_Tool_Brush:
    {
      switch (WorldEditBrushType)
      {
        case WorldEdit_BrushType_Disabled:  {} break;
        case WorldEdit_BrushType_Single:    {} break;
        case WorldEdit_BrushType_Entity:    {} break;
        case WorldEdit_BrushType_Asset:     {} break;

        case WorldEdit_BrushType_Layered:
        {
          DoBrushSettingsWindow(Engine, &Engine->Editor.Brush.Layered, &Window);
        } break;

      }
    } break;
  }
}
#endif

link_internal b32
CurrentToolIs(level_editor *Editor, world_edit_tool Tool, world_edit_brush_type BrushType)
{
  b32 Result = False;
  NotImplemented;
  /* if (Editor->Tool == Tool) */
  /* { */
  /*   Result = True; */
  /*   if (Editor->Tool == WorldEdit_Tool_Brush) */
  /*   { */
  /*     Result = (Editor->Brush.Type == BrushType); */
  /*   } */
  /* } */
  return Result;
}

link_internal aabb_intersect_result
EditWorldSelection(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  aabb_intersect_result AABBTest = {};

  if (Editor->Selection.Clicks)
  {
    r32 Thickness = 0.10f;

    if (SelectionIncomplete(Editor->Selection.Clicks))
    {
      if (Engine->MousedOverVoxel.Tag)
      {
        auto MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value);
        MouseP.Offset = Floor(MouseP.Offset);

        cp MinP = Min(Editor->Selection.Base, MouseP);
        cp MaxP = Max(Editor->Selection.Base, MouseP) + V3(1.f);
        /* Assert(MinP <= MaxP); */
        Editor->Selection.Region = RectMinMax(MinP, MaxP);
      }
    }
    else
    {
      Thickness = 0.20f;
    }

#if 0
    if (CurrentToolIs(Editor, WorldEdit_Tool_Brush, WorldEdit_BrushType_Layered))
    {
      layered_brush *Brush = &Editor->Brush.Layered;
      if (Brush->BrushFollowsCursor)
      {
        if (Engine->MousedOverVoxel.Tag)
        {
          cp MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value, PickedVoxel_LastEmpty);

          v3 SelectionDim = GetDim(GetSimSpaceRect(World, Editor->Selection.Region));
          v3 SelectionRad = SelectionDim / 2.f;

          Editor->Selection.Region.Min = MouseP - SelectionRad;
          Editor->Selection.Region.Max = MouseP + SelectionRad;

          Canonicalize(World, &Editor->Selection.Region.Min);
          Canonicalize(World, &Editor->Selection.Region.Max);

          Truncate(&Editor->Selection.Region.Min.Offset);
          Truncate(&Editor->Selection.Region.Max.Offset);

        }
      }
    }
#endif

    aabb SelectionAABB = GetSimSpaceRect(World, Editor->Selection.Region);
    {
      if (Engine->MaybeMouseRay.Tag == Maybe_Yes)
      {
        ray Ray = Engine->MaybeMouseRay.Ray;

        /* Ray.Origin = GetSimSpaceP(World, Canonical_Position(World->ChunkDim, Ray.Origin, {})); */
        AABBTest = Intersect(&SelectionAABB, &Ray);

        face_index Face = AABBTest.Face;
        /* PushColumn(Ui, CS(Face)); */
        /* PushNewRow(Ui); */

        if (Face)
        {
          /* r32 InsetWidth = 0.25f; */
          r32 InsetWidth  = 0.f;
          v3  HiColor     = RGB_GREEN;
          r32 HiThickness = Thickness*1.2f;

          HighlightFace(Engine, Face, SelectionAABB, InsetWidth, HiColor, HiThickness);

          if ( Input->LMB.Clicked && (Input->Ctrl.Pressed || Input->Shift.Pressed || Input->Alt.Pressed) )
          {
            v3 PlaneBaseP = Ray.Origin + (AABBTest.t*Ray.Dir);
            Editor->Selection.ModState.ClickedFace = Face;
            Editor->Selection.ModState.ClickedP[0] = PlaneBaseP;
          }
        }

        if (Editor->Selection.ModState.ClickedFace)
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

          rect3i ModifiedSelection = DoSelectonModification(Engine, &Ray, SelectionMode, &Editor->Selection.ModState, SelectionAABB);

          if (!Input->LMB.Pressed)
          {
            // If we actually changed the selection region
            rect3cp ProposedSelection = SimSpaceToCanonical(World, &ModifiedSelection);

            // Make ModifiedSelection permanent
            Editor->Selection.Region = ProposedSelection;
            Editor->Selection.ModState.ClickedFace = FaceIndex_None;

          }
        }
      }
    }

    // Draw selection box
    //

    v3 BaseColor = RGB_WHITE;
    DEBUG_DrawSimSpaceAABB(Engine, &SelectionAABB, BaseColor, Thickness);
  }


  // Don't fire selection changed event when dragging selection with selection edit tool
  if (Editor->Selection.Clicks != 1)
  {
    if (AreEqual(Editor->Selection.Region, Editor->Selection.PrevRegion))
    {
      Editor->Selection.Changed = False;
    }
    else
    {
      Editor->Selection.Changed = True;
    }
    Editor->Selection.PrevRegion = Editor->Selection.Region;
  }

  return AABBTest;
}

link_internal cp
GetSelectionCenterP(world *World, level_editor *Editor)
{
  v3i Dim = GetSelectionDim(World, Editor);
  cp Result = Canonicalize(World, Editor->Selection.Region.Min + V3(Dim/2));
  return Result;
}

link_internal b32
InputStateIsValidToApplyEdit(input *Input)
{
  b32 Result = Input->LMB.Clicked && !Input->Shift.Pressed && !Input->Ctrl.Pressed && !Input->Alt.Pressed;
  return Result;
}

#if 0
link_internal world_edit_blend_mode
GetEditModeForSelectedTool(level_editor *Editor)
{
  // Default is attach for tools/brushes that don't have a mode in their settings
  world_edit_blend_mode Result = WorldEdit_Mode_Additive;

  switch(Editor->Tool)
  {
    case WorldEdit_Tool_Disabled:   {} break;
    case WorldEdit_Tool_Select:     {} break;
    case WorldEdit_Tool_Eyedropper: {} break;
    case WorldEdit_Tool_BlitEntity: {} break;

    case WorldEdit_Tool_Brush:
    {
      switch(Editor->Brush.Type)
      {
        case WorldEdit_BrushType_Disabled: {} break;
        case WorldEdit_BrushType_Entity:   {} break;
        case WorldEdit_BrushType_Single:   { Result = Editor->Brush.Single.Mode; } break;
        case WorldEdit_BrushType_Asset:    { Result = Editor->Brush.Asset.Mode;  } break;
        case WorldEdit_BrushType_Layered:  { Result = Editor->Brush.Layered.Mode;} break;
      }
    } break;
  }

  return Result;
}
#endif

link_internal v3 
ColorIndexToV3(u16 ColorIndex)
{
  u32 FiveBits = 0b0000000000011111;
  u32 SixBits  = 0b0000000000111111;

  f32 R = f32((ColorIndex >> 11) & FiveBits) / f32(FiveBits);
  f32 G = f32((ColorIndex >> 6)  & FiveBits) / f32(FiveBits);
  f32 B = f32((ColorIndex >> 0)  & SixBits) / f32(SixBits);

  v3 Result = V3(R,G,B);
  return Result;
}


link_internal void
DoColorPickerSection(renderer_2d *Ui, window_layout *Window, v3 *HSVDest, u32 HSVElementIndex, u32 Slices, v2 WidgetDim)
{
  v2 QuadDim = V2(WidgetDim.x/r32(Slices), WidgetDim.y);
  v4 Padding = V4(0);
  v3 HSV = *HSVDest;

  r32 CurrentValue = HSVDest->E[HSVElementIndex];

  ui_element_reference SaturationTable = PushTableStart(Ui);
  RangeIterator_t(u8, ColorIndex, Slices)
  {
    r32 Value = r32(ColorIndex)/r32(Slices);

    HSV.E[HSVElementIndex] = Value;
    v3 RGB = HSVtoRGB(HSV);

    b32 Selected = Value == CurrentValue;
    ui_style Style = FlatUiStyle(RGB);
    ui_id Id = UiId(Window, Cast(void*, "ColorPicker value button"), Cast(void*, HSVDest), Cast(void*, u64(ColorIndex) | u64(HSVElementIndex<<16)));

    interactable_handle ColorPickerButton = PushButtonStart(Ui, Id);
      PushUntexturedQuad(Ui, {}, QuadDim, zDepth_Text, &Style, Padding );
    PushButtonEnd(Ui);

    b32 ButtonHover = Hover(Ui, &ColorPickerButton);
    if (Selected || ButtonHover)
    {
      f32 BorderDim = 1.f;
       v3 BorderColor = V3(1.0f);
      PushRelativeBorder(Ui, V2(-1.f,1.f)*QuadDim, BorderColor, V4(BorderDim));
    }

    if (ButtonHover)
    {
      PushTooltip(Ui, FSz("%d %.2V3", ColorIndex, &RGB));
    }

    if (Clicked(Ui, &ColorPickerButton))
    {
      HSVDest->E[HSVElementIndex] = Value;
    }
  }
  PushTableEnd(Ui);
  PushNewRow(Ui);
}

link_internal void
DoColorPicker(renderer_2d *Ui, window_layout *Window, v3 *HSVDest, b32 ShowColorSwatch)
{
  /* u8 FourBits   = 0b1111; */
  u8 FiveBits   = 0b11111;
  u8 SixBits    = 0b111111;
  /* u8 EightBits  = 0b11111111; */

  u16 HueSlices        = SixBits;
  u16 SaturationSlices = FiveBits;
  u16 ValueSlices      = FiveBits;

  v2 ColorPickerSectionDim = V2(256, 30);

  DoColorPickerSection(Ui, Window, HSVDest, 0, HueSlices,        ColorPickerSectionDim);
  DoColorPickerSection(Ui, Window, HSVDest, 1, SaturationSlices, ColorPickerSectionDim);
  DoColorPickerSection(Ui, Window, HSVDest, 2, ValueSlices,      ColorPickerSectionDim);

  PushNewRow(Ui);

  v3 RGB = HSVtoRGB(*HSVDest);
  if (ShowColorSwatch)
  {
    v2 QuadDim = V2(ColorPickerSectionDim.x, ColorPickerSectionDim.x);
    DoColorSwatch(Ui, QuadDim, RGB);
  }

  cs HSVColorString = FSz("HSV (%.2V3)", HSVDest);
  cs RGBColorString = FSz("RGB (%.2V3)", &RGB);

  PushColumn(Ui, HSVColorString );
  PushNewRow(Ui);

  PushColumn(Ui, RGBColorString );
  PushNewRow(Ui);
}

link_internal void
DoColorPickerToggle(renderer_2d *Ui, window_layout *Window, v3 *HSVDest, b32 ShowColorSwatch)
{
  ui_id InteractionId = UiId(Window, "ColorPicker toggle button", HSVDest);

  u32 ColumnIndex = StartColumn(Ui);
    if (ToggledOn(Ui, InteractionId))
    {
      // NOTE(Jesse): Gotta use a discrete button id for this
      if (Button(Ui, CSz("Done"), UiId(Window, "ColorPicker toggle close", HSVDest))) { SetToggleButton(Ui, InteractionId, False); }
      PushNewRow(Ui);
      DoColorPicker(Ui, Window, HSVDest, ShowColorSwatch);
    }
    else
    {
      ui_style BStyle = UiStyleFromLightestColor(HSVtoRGB(*HSVDest));
      interactable_handle Handle = ToggleButtonStart(Ui, InteractionId, &BStyle); //, Padding, AlignFlags);
        v2 Dim = V2(25);
        PushUntexturedQuad(Ui, V2(0), Dim, zDepth_Text, &BStyle);
      ToggleButtonEnd(Ui);
    }
  EndColumn(Ui, ColumnIndex);

  PushNewRow(Ui);
}

link_internal void
ColorPickerModal(engine_resources *Engine, ui_id ModalId, v3 *HSVDest, b32 ShowColorSwatch /* = True */)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  if (window_layout *Window = ModalIsActive(Ui, ModalId))
  {
    DoColorPicker(Ui, Window, HSVDest, ShowColorSwatch);

    if (Button(Ui, CSz("Close"), UiId(Window, "modal close button", 0u)))
    {
      CompleteModal(Ui, ModalId);
    }

    PushNewRow(Ui);
  }
}

link_internal b32
CheckSettingsChanged(layered_brush *Brush)
{
  b32 Result = False;
  RangeIterator(LayerIndex, Brush->LayerCount)
  {
    brush_layer *Layer = Brush->Layers + LayerIndex;
    Result |= !AreEqual(&Layer->Settings, &Layer->PrevSettings);
    Layer->PrevSettings = Layer->Settings;
  }
  return Result;
}

link_internal b32
CheckSettingsChanged(world_edit *Edit)
{
  b32 Result = False;
  if (Edit->Brush)
  {
    Result = CheckSettingsChanged(&Edit->Brush->Layered);
  }
  return Result;
}

link_internal void
ApplyEditToOctree(engine_resources *Engine, world_edit *Edit, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  // Gather newly overlapping nodes and add the edit
  {
    octree_node_ptr_block_array Nodes = OctreeNodePtrBlockArray(TempMemory);

    rect3cp QueryRegion = Edit->Region;

    /* QueryRegion.Min.Offset -= 1.f; */
    /* QueryRegion.Max.Offset += 1.f; */

    /* Canonicalize(World, &QueryRegion.Min); */
    /* Canonicalize(World, &QueryRegion.Max); */

    GatherOctreeNodesOverlapping_Recursive(World, &World->Root, &QueryRegion, &Nodes);

    IterateOver(&Nodes, Node, NodeIndex)
    {
      AcquireFutex(&Node->Lock);
      /* auto EditAABB = GetSimSpaceAABB(World, Node); */
      /* random_series S = {u64(Node)}; */
      /* v3 BaseColor = RandomV3Unilateral(&S); */
      /* DEBUG_DrawSimSpaceAABB(Engine, &EditAABB, BaseColor, 1.f); */

      {
        // Shouldn't have this edit already attached ..
        world_edit_ptr_block_array_index Index = Find(&Node->Edits, Edit);
        Assert( IsValid(&Index) == False );
      }

      if (Node->Edits.Memory == 0)
      {
        Node->Edits = WorldEditPtrBlockArray(Editor->Memory);
      }

      Push(&Node->Edits, &Edit);

      /* Assert(Node->Type == OctreeNodeType_Leaf); */

      Node->Dirty = True;
      ReleaseFutex(&Node->Lock);
    }
  }
}

link_internal void
UpdateWorldEdit(engine_resources *Engine, world_edit *Edit, rect3cp Region, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  // First, gather the currently edited nodes and remove the edit
  {
    octree_node_ptr_block_array Nodes = OctreeNodePtrBlockArray(TempMemory);
    GatherOctreeNodesOverlapping_Recursive(World, &World->Root, &Edit->Region, &Nodes);

    IterateOver(&Nodes, Node, NodeIndex)
    {
      AcquireFutex(&Node->Lock);
      auto Index = Find(&Node->Edits, Edit);
      Assert(IsValid(&Index)); // There shouldn't be a node that doesn't contain the edit
      RemoveUnordered(&Node->Edits, Index);

      // Need to reinitialize chunks that no longer have the edit so that it
      // doesn't stay intact in chunks that lose it entirely
      Node->Dirty = True;;
      ReleaseFutex(&Node->Lock);
    }
  }

  //
  // Update the edit
  //

  Editor->CurrentEdit->Region = Region; // TODO(Jesse): I feel like this should be happening more automagically, but ..

  ApplyEditToOctree(Engine, Edit, TempMemory);
}

link_internal void
DoWorldEditor(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  // @selection_changed_flag
  //
  aabb_intersect_result AABBTest = EditWorldSelection(Engine);
  aabb SelectionAABB = GetSimSpaceRect(World, Editor->Selection.Region);

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
      /* ui_element_reference CurrentRef = {}; */

      {
        WorldEditToolButtonGroup = DoEditorUi(Ui, &Window, &Editor->Tool, CSz("Tool"), &Params, ToggleButtonGroupFlags_DrawVertical);
        /* CurrentRef = WorldEditToolButtonGroup.UiRef; */
      }

#if 0
      if (Editor->Tool == WorldEdit_Tool_Brush)
      {
        Params.RelativePosition.Position   = Position_RightOf;
        Params.RelativePosition.RelativeTo = CurrentRef;
        WorldEditBrushTypeButtonGroup = DoEditorUi(Ui, &Window, &Editor->Brush.Type, CSz("Brush Type"), &Params, ToggleButtonGroupFlags_DrawVertical);
        CurrentRef = WorldEditBrushTypeButtonGroup.UiRef;
      }
#endif

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
      /* case WorldEdit_Tool_Brush: */
      case WorldEdit_Tool_Eyedropper:
      case WorldEdit_Tool_BlitEntity:
      /* case WorldEdit_Tool_StandingSpots: */
      { } break;

      case WorldEdit_Tool_Select:
      {
        ResetSelection(Editor);
      } break;
    }
  }


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

#if 0
      case WorldEdit_Tool_Brush:
      {
        switch (Editor->Brush.Type)
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

                  switch (Asset->Type)
                  {
                    InvalidCase(AssetType_Undefined);

                    case AssetType_WorldChunk:
                    case AssetType_Models:
                    {
                      // TODO(Jesse): There should be an unsafe version of this function, and rename this one to TryGetDimForAssetModel ..?
                      maybe_v3i MaybeDim = GetDimForAssetModel(Asset, u32(EngineDebug->ModelIndex));
                      if (MaybeDim.Tag)
                      {
                        v3 AssetHalfDim = MaybeDim.Value / 2.f;
                        world_update_op_shape_params_asset AssetUpdateShape =
                        {
                          EngineDebug->SelectedAsset,
                          EngineDebug->ModelIndex,
                          Canonicalize(World, EntityOrigin - V3(AssetHalfDim.xy, 0.f))
                        };

                        if (Editor->Brush.Type == WorldEdit_BrushType_Asset)
                        {
                          world_edit_shape Shape =
                          {
                            type_world_update_op_shape_params_asset,
                            .world_update_op_shape_params_asset = AssetUpdateShape,
                          };
                          QueueWorldUpdateForRegion(Engine, Editor->Brush.Asset.Mode, Editor->Brush.Asset.Modifier, &Shape, {}, {}, Engine->WorldUpdateMemory);
                        }
                        else if (Editor->Brush.Type == WorldEdit_BrushType_Entity)
                        {
                          entity *E = TryGetFreeEntityPtr(Engine->EntityTable);
                          if (E)
                          {
                            SpawnEntity(E, &EngineDebug->SelectedAsset, EngineDebug->ModelIndex, EntityBehaviorFlags_Default, 0, &AssetUpdateShape.Origin, AssetHalfDim);
                          }
                        }
                        else
                        {
                          InvalidCodePath(); // NOTE(Jesse): This is redundant because we already checked it's one of these,
                                             // but I'm going to keep it here in case we ever move this code.. or the checks.
                        }
                      }
                      else
                      {
                        BUG("Somehow requested a ModelIndex from an Asset that did not have one!");
                      }
                    } break;

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
            NotImplemented;
#if 0
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
                  ApplyEditToRegion(Engine, &AABB, Engine->Editor.SelectedColorIndex, {}, Editor->Params.Mode, Editor->Params.Modifier);
                }
              }
            }
#endif
          } break;

          case WorldEdit_BrushType_Layered:
          {
            layered_brush *Layered = &Editor->Brush.Layered; 
            if (AABBTest.Face && InputStateIsValidToApplyEdit(Input))
            {
              v3i Offset = V3i(s32_MAX);
              world_chunk *Chunk = &Layered->Preview.Chunk;

              // TODO(Jesse): Call GetSmallestMinOffset here
              RangeIterator(LayerIndex, Layered->LayerCount)
              {
                brush_layer *Layer = Layered->Layers + LayerIndex;
                Offset = Min(Layer->Settings.Offset.Min, Offset);
              }

              chunk_data D = {Chunk->Flags, Chunk->Dim, Chunk->Occupancy, Chunk->xOccupancyBorder, Chunk->FaceMasks, Chunk->Voxels, Chunk->VoxelLighting};
              world_update_op_shape_params_chunk_data ChunkDataShape = { D, V3(Offset) + GetSimSpaceP(World, Editor->Selection.Region.Min) - V3i(1) };

              world_edit_shape Shape =
              {
                type_world_update_op_shape_params_chunk_data,
                .world_update_op_shape_params_chunk_data = ChunkDataShape,
              };
              QueueWorldUpdateForRegion(Engine, Layered->Mode, Layered->Modifier, &Shape, DEFAULT_HSV_COLOR, Layered->SeedBrushWithSelection, Engine->WorldUpdateMemory);
            }
          } break;

        }
      } break;
#endif

      case WorldEdit_Tool_Select:
      {
        if (Input->LMB.Clicked)
        { switch (Editor->Selection.Clicks)
          {
            case 0:
            {
              if (Engine->MousedOverVoxel.Tag)
              {
                Editor->Selection.Clicks += 1;
                auto MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value);
                MouseP.Offset = Floor(MouseP.Offset);
                Editor->Selection.Base = MouseP;
              }
            } break;

            case 1:
            {
              Editor->Selection.Clicks += 1;

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
            /* Info("Selecting Color (%S)", CS(V->Color)); */
            /* Engine->Editor.HSVColorSelection = UnpackHSVColor(V->Color); */

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
              NotImplemented;
#if 0
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
              QueueWorldUpdateForRegion(Engine, WorldEdit_Mode_Additive, WorldEdit_Modifier_Default, &Shape, {}, {}, Engine->WorldUpdateMemory);
#endif
            }
          }
        }
      } break;

#if 0
      case WorldEdit_Tool_StandingSpots:
      {
        NotImplemented;
        if (Input->LMB.Clicked && AABBTest.Face && !Input->Shift.Pressed && !Input->Ctrl.Pressed)
        {
          world_edit_shape Shape = {
            .Type = type_world_update_op_shape_params_rect,
            .world_update_op_shape_params_rect.P0 = SelectionAABB.Min,
            .world_update_op_shape_params_rect.P1 = SelectionAABB.Max,
          };
          QueueWorldUpdateForRegion(Engine, WorldUpdateOperationMode_StandingSpots, &Shape, Editor->SelectedColorIndex, Engine->WorldUpdateMemory);
        }
      } break;
#endif

    }
  }


  // NOTE(Jesse): This has to come after the tool interactions because of a trivial
  // but important interaction.  If there's a previous tool to pop back to, we
  // need to do that pop back before the brush settings get updated and the
  // brush preview gets drawn or else when we pop back the settings window thinks
  // the Select tool is active, doesn't update any of the brush stuff, then
  // the preview gets drawn because we pop back to the Layered brush tool and
  // there's a frame of lag.
  /* DoBrushSettingsWindow(Engine, Editor->Tool, Editor->Brush.Type); */



  //
  //
  // Hotkeys
  //
  //


  if (Input->Ctrl.Pressed || Input->Shift.Pressed || Input->Alt.Pressed) { Ui->RequestedForceCapture = True; }

  if (Input->Ctrl.Pressed && Input->E.Clicked) { Editor->PreviousTool = Editor->Tool; Editor->Tool = WorldEdit_Tool_Eyedropper; ResetSelectionIfIncomplete(Editor); }

  if (Input->Ctrl.Pressed && Input->G.Clicked) { if (entity *Ghost = GetCameraGhost(Engine)) { Ghost->P = GetSelectionCenterP(World, Editor); } }

  if (Input->Ctrl.Pressed && Input->S.Clicked)
  {
    Editor->PreviousTool = Editor->Tool;
    Editor->Tool = WorldEdit_Tool_Select;
    ResetSelection(Editor);

    Editor->CurrentEdit = Push(&Editor->CurrentLayer->Edits);
    Editor->CurrentEdit->Ordinal = Editor->NextEditOrdinal++;

    Editor->CurrentEdit->Brush = Editor->CurrentBrush;
  }

#if 0
  if (Editor->Selection.Clicks == 2)
  {
    if (Input->Ctrl.Pressed && Input->D.Clicked) { ApplyEditToRegion(Engine, &SelectionAABB, {}, {}, WorldEdit_Mode_Subtractive, WorldEdit_Modifier_Default); }

    if (Input->Ctrl.Pressed && Input->C.Clicked) { Editor->CopyRegion = Editor->Selection.Region; }

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

      world_update_op_shape_params_chunk_data ChunkDataShape = { D, GetSimSpaceP(World, Editor->Selection.Region.Min) };

      world_edit_shape Shape =
      {
        type_world_update_op_shape_params_chunk_data,
        .world_update_op_shape_params_chunk_data = ChunkDataShape,
      };
      QueueWorldUpdateForRegion(Engine, WorldEdit_Mode_Additive, WorldEdit_Modifier_Default, &Shape, {}, {}, Engine->WorldUpdateMemory);
    }
  }
#endif

  { // All Brushes Window
    local_persist window_layout BrushSettingsWindow = WindowLayout("All Brushes", WindowLayoutFlag_Align_BottomRight);
    PushWindowStart(Ui, &BrushSettingsWindow);

    if (Button(Ui, CSz("New"), UiId(&BrushSettingsWindow, "brush new", 0u)))
    {
      world_edit_brush Brush = {};
      NewBrush(&Brush);
      Editor->CurrentBrush = Insert(Brush, &Editor->LoadedBrushes, Editor->Memory);
    }
    PushNewRow(Ui);

    IterateOver(&Editor->LoadedBrushes, Brush, BrushIndex)
    {
      if (Brush)
      {
        if (Brush == Editor->CurrentBrush)
        {
          PushColumn(Ui, CSz("*"), &DefaultSelectedStyle);
        }
        else
        {
          PushColumn(Ui, CSz(" "), &DefaultSelectedStyle);
        }

        if (Button(Ui, CS(Brush->NameBuf), UiId(&BrushSettingsWindow, "brush select", Brush)))
        {
          Editor->CurrentBrush = Brush;
          CheckSettingsChanged(&Brush->Layered); // Prevent firing a change event @prevent_change_event
        }
        PushNewRow(Ui);
      }
    }
    PushWindowEnd(Ui, &BrushSettingsWindow);

    if (Editor->CurrentBrush)
    {
      local_persist window_layout BrushSettingsWindow = WindowLayout("Brush Settings", WindowLayoutFlag_Align_Right);
      DoBrushSettingsWindow(Engine, Editor->CurrentBrush, &BrushSettingsWindow);

      // NOTE(Jesse): Must come after the settings window draws because the
      // settings window detects and initializes new brushes
      if (SelectionComplete(Editor->Selection.Clicks) && Editor->CurrentBrush)
      {
        if (Editor->Selection.Changed && Editor->CurrentEdit)
        {
          UpdateWorldEdit(Engine, Editor->CurrentEdit, Editor->Selection.Region, GetTranArena());
        }

        b32 SettingsChanged = CheckSettingsChanged(&Editor->CurrentBrush->Layered);
        if (SettingsChanged)
        {
          IterateOver(&Editor->Layers, Layer, LayerIndex)
          {
            IterateOver(&Layer->Edits, Edit, EditIndex)
            {
              if (Edit->Brush == Editor->CurrentBrush)
              {
                UpdateWorldEdit(Engine, Edit, Editor->Selection.Region, GetTranArena());
              }
            }
          }
        }
      }
    }

  }

  {
    local_persist window_layout AllEditsWindow = WindowLayout("All Edits", WindowLayoutFlag_Align_Bottom);
    PushWindowStart(Ui, &AllEditsWindow);


    ui_toggle_button_group Toolbar = PushToolbar(Ui, &AllEditsWindow, {}, &Ui->LayerToolbarAction);

    if (Toolbar.AnyElementClicked)
    {
      switch(Ui->LayerToolbarAction)
      {
        case LayerToolbarActions_New:
        {
          NewLayer(Editor);
        } break;

        case LayerToolbarActions_Delete:
        {
          RemoveOrdered(&Editor->Layers, Editor->CurrentLayer);
        } break;

        case LayerToolbarActions_Rename:
        {
          NotImplemented;
          ui_id Id = {};
          TextBox(Ui, {}, CS(Editor->CurrentLayer->NameBuf, NameBuf_Len), NameBuf_Len, Id);
        } break;

        case LayerToolbarActions_Duplicate:
        {
          NotImplemented;
        } break;
      }
    }

    PushTableStart(Ui);
    IterateOver(&Editor->Layers, Layer, LayerIndex)
    {
      cs Name = CS(Layer->NameBuf);
      if (ToggleButton(Ui, Name, Name, UiId(&AllEditsWindow, Layer, Layer), &DefaultSelectedStyle))
      {
        PushNewRow(Ui);
        IterateOver(&Layer->Edits, Edit, BrushIndex)
        {
          umm I = GetIndex(&BrushIndex);
          const char *NameBuf = Edit->Brush ? Edit->Brush->NameBuf : "no brush";

          if (Edit == Editor->CurrentEdit)
          {
            PushColumn(Ui, CSz("*"), &DefaultSelectedStyle);
          }
          else
          {
            PushColumn(Ui, CSz(" "), &DefaultSelectedStyle);
          }

          auto EditSelectButton = PushSimpleButton(Ui, FSz("(%d) (%s)", I, NameBuf), UiId(&AllEditsWindow, "edit select", Edit));
          if (Clicked(Ui, &EditSelectButton))
          {
            Editor->Selection.Clicks = 2;
            Editor->Selection.Region = Edit->Region;
            Editor->Selection.PrevRegion = Edit->Region;

            Editor->CurrentEdit = Edit;

            if (Edit->Brush)
            {
              Editor->CurrentBrush = Edit->Brush;
              CheckSettingsChanged(&Edit->Brush->Layered); // Prevent firing a change event @prevent_change_event
            }
          }

          if (Hover(Ui, &EditSelectButton))
          {
            Editor->HotEdit = Edit;
          }

          if (Button(Ui, FSz("SetBrush", I, NameBuf), UiId(&AllEditsWindow, "edit brush select", Edit)))
          {
            Edit->Brush = Editor->CurrentBrush;
            UpdateWorldEdit(Engine, Edit, Edit->Region, GetTranArena());
          }

          PushNewRow(Ui);

          /* DoEditorUi(Ui, &AllEditsWindow, Edit, {}); */
          /* PushNewRow(Ui); */
        }
      }

    }
    PushTableEnd(Ui);
    PushWindowEnd(Ui, &AllEditsWindow);

  }



#if VOXEL_DEBUG_COLOR
  {
    cp DebugVoxel = {};
    if (Editor->Selection.Clicks > 0)
    {
      DebugVoxel = Editor->Selection.Region.Min;
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
    /* v3 HotVoxel = GetHotVoxelForEditMode(Engine, GetEditModeForSelectedTool(Editor) ); */
    v3 HotVoxel = GetHotVoxelForEditMode(Engine, WorldEdit_Mode_Additive );
    DEBUG_HighlightVoxel( Engine, HotVoxel, RGB_RED, 0.075f);
  }


  {
    IterateOver(&Editor->Layers, Layer, LayerIndex)
    {
      IterateOver(&Layer->Edits, Edit, BrushIndex)
      {
        auto EditAABB = GetSimSpaceAABB(World, Edit->Region);
        random_series S = {u64(Edit)};
        v3 BaseColor = RandomV3Unilateral(&S);

        f32 Size = DEFAULT_LINE_THICKNESS;
        if (Edit == Editor->CurrentEdit)
        {
          Size = 3.f*DEFAULT_LINE_THICKNESS;
        }

        if (Edit == Editor->HotEdit)
        {
          Size = 5.f*DEFAULT_LINE_THICKNESS;
        }

        DEBUG_DrawSimSpaceAABB(Engine, &EditAABB, BaseColor, Size);
      }
    }
    Editor->HotEdit = 0;
  }
}

link_internal void
DrawLod(engine_resources *Engine, shader *Shader, gpu_mapped_element_buffer *Mesh, r32 DistanceSquared, v3 Basis, Quaternion Rotation, v3 Scale );

#if 0
link_internal void
DrawEditorPreview(engine_resources *Engine, shader *Shader)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  world_chunk *Chunk = {};
  v3 Basis = {};

  switch (Editor->Tool)
  {
    case WorldEdit_Tool_Brush:
    {
      switch (Editor->Brush.Type)
      {
        case WorldEdit_BrushType_Layered:
        {
          layered_brush *LayeredBrush = &Editor->Brush.Layered;
          v3i SmallestMinOffset = GetSmallestMinOffset(LayeredBrush);
          Chunk = &LayeredBrush->Preview.Chunk;
          Basis = V3(SmallestMinOffset) - V3i(1);
        } break;

        default: {} break;
      }
    } break;

    default: {} break;
  }

  if (Chunk)
  {
    DrawLod(Engine, Shader, &Chunk->Mesh, 0.f, Basis, Quaternion(), V3(1.f));
  }
}
#endif

link_internal void
ApplyEditBufferToOctree(engine_resources *Engine, world_edit_paged_list *Edits)
{
  IterateOver(Edits, Edit, EditIndex)
  {
    ApplyEditToOctree(Engine, Edit, GetTranArena());
  }
}

link_internal void
DoLevelWindow(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  local_persist window_layout Window = WindowLayout("Level");

  thread_local_state *Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);

  //
  // Level Export
  //
  PushWindowStart(Ui, &Window);
  PushTableStart(Ui);
    if (Button(Ui, CSz("Export Level"), UiId(&Window, "export_level_button", 0ull)))
    {
      u8_cursor_block_array OutputStream = BeginSerialization();

      u32 EntityCount = 0;
      RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
      {
        entity *E = EntityTable[EntityIndex];
        if (Spawned(E)) { ++EntityCount; }
      }

      level_header Header = {};

      Header.WorldCenter       =  World->Center;
      Header.VisibleRegionSize =  World->VisibleRegionSize;
      Header.Camera            = *Camera;
      Header.RenderSettings    =  Graphics->Settings;
      Header.EntityCount       =  EntityCount;
      Header.LayerCount        =  u32(TotalElements(&Editor->Layers));

      {
        cs Filename = Engine->Graphics.TerrainShapingRC.Program.FragSourceFilename;
        cs Dest = CS(Header.TerrainShapingShader, NameBuf_Len);
        CopyString(&Filename, &Dest);
      }
      {
        cs Filename = Engine->Graphics.TerrainDecorationRC.Program.FragSourceFilename;
        cs Dest = CS(Header.TerrainDecorationShader, NameBuf_Len);
        CopyString(&Filename, &Dest);
      }

      Serialize(&OutputStream, &Header);

      u64 Delimeter = LEVEL_FILE_DEBUG_OBJECT_DELIM;
      Ensure(Serialize(&OutputStream, &Delimeter));

#if 1
      IterateOver(&Editor->Layers, Layer, LayerIndex)
      {
        u32 EditCount = u32(TotalElements(&Layer->Edits));
        Serialize(&OutputStream, &EditCount);
        IterateOver(&Layer->Edits, Edit, EditIndex)
        {
          Serialize(&OutputStream, Edit);
        }
      }
#else
      RangeIterator(HashIndex, s32(World->HashSize))
      {
        if (world_chunk *Chunk = World->ChunkHash[HashIndex])
        {
          SerializeChunk(Chunk, &OutputStream);
        }
      }
#endif

      Ensure(Serialize(&OutputStream, &Delimeter));

      RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
      {
        entity *E = EntityTable[EntityIndex];
        if (Spawned(E))
        {
          Serialize(&OutputStream, E);
        }
      }

      Ensure(Serialize(&OutputStream, &Delimeter));

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

  //
  // Level Import
  //
  if (ClickedNode.Tag)
  {
    cs Filename = Concat(ClickedNode.Value.Dir, CSz("/"), ClickedNode.Value.Name, GetTranArena());

    u8_cursor LevelBytes = BeginDeserialization(Filename, GetTranArena());
    if (LevelBytes.Start)
    {
      level_header LevelHeader = {};

      Leak("Leaking level_header because we need to hold the level_header::TerrainGenShader such that we can reload the shader next frame ..");
      if (Deserialize(&LevelBytes, &LevelHeader, Thread->PermMemory))
      {
        u64 Delimeter = LEVEL_FILE_DEBUG_OBJECT_DELIM;
        Ensure(Read_u64(&LevelBytes) == Delimeter);

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


        Graphics->Settings       = LevelHeader.RenderSettings;
       *Graphics->Camera         = LevelHeader.Camera;
        World->VisibleRegionSize = LevelHeader.VisibleRegionSize;
        World->Center            = LevelHeader.WorldCenter;

        /* Global_ProjectSwitcherGameLibName  = LevelHeader.TerrainGenShader; */

        Engine->Graphics.TerrainShapingRC.Program.FragSourceFilename = CopyString(LevelHeader.TerrainShapingShader, Thread->PermMemory);
        Engine->Graphics.TerrainDecorationRC.Program.FragSourceFilename = CopyString(LevelHeader.TerrainDecorationShader, Thread->PermMemory);

        // Must come after we fill out the VisibleRegion so the root octree node
        // gets initialized to the correct size
        //
        //
        // HACK(Jesse): This is a maaaajor hack and should be dealt with in a better way
        bonsai_type_info_hashtable Tmp = Global_SerializeTypeTable;
        Global_SerializeTypeTable = {};
        HardResetEngine(Engine);
        Global_SerializeTypeTable = Tmp;


        /* s32 ChunkCount = Cast(s32, LevelHeader.ChunkCount); */
        s32 ChunkCount = 0;
        /* Info("ChunksFreed (%u) ChunksLoaded (%u)", ChunksFreed, ChunkCount); */

        NotImplemented;
        /* RangeIterator_t(u32, EditIndex, LevelHeader.EditCount) */
        /* { */
        /*   world_edit DeserEdit = {}; // TODO(Jesse): Do we actually have to clear this? */
        /*   Deserialize(&LevelBytes, &DeserEdit, GetTranArena()); */

        /*   world_edit *FinalEdit = Push(&Editor->WorldEdits, &DeserEdit); */
        /*   if (FinalEdit->Brush) */
        /*   { */
        /*     FinalEdit->Brush = Upsert(*FinalEdit->Brush, &Editor->LoadedBrushes, Editor->Memory); */
        /*   } */

        /*   ApplyEditToOctree(Engine, FinalEdit, GetTranArena()); */
        /* } */

        Ensure(Read_u64(&LevelBytes) == Delimeter);

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

        Ensure(Read_u64(&LevelBytes) == Delimeter);

        Assert(ThreadLocal_ThreadIndex == 0);
        if (Engine->GameApi.OnLibraryLoad) { Engine->GameApi.OnLibraryLoad(Engine, GetThreadLocalState(ThreadLocal_ThreadIndex)); }

        UnsignalFutex(&Plat->WorkerThreadsSuspendFutex);
      }
    }
    FinalizeDeserialization(&LevelBytes);

  }
  PushWindowEnd(Ui, &Window);
}

