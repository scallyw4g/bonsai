
link_internal void
LoadBrushFromFile(level_editor *Editor, file_traversal_node *FileNode, memory_arena *TempMemory)
{
  Assert(FileNode->Type == FileTraversalType_File);
  cs Filename = Concat(FileNode->Dir, CSz("/"), FileNode->Name, TempMemory);

  u8_cursor Bytes = BeginDeserialization(Filename, TempMemory);

  if (Bytes.Start)
  {
    world_edit_brush B = {};
    CopyString( FileNode->Name.Start, B.NameBuf, Min(umm(FileNode->Name.Count), umm(NameBuf_Len)));
    Editor->CurrentBrush = Upsert(B, &Editor->LoadedBrushes, &Global_PermMemory);

    if (Deserialize(&Bytes, Editor->CurrentBrush, TempMemory) == False)
    {
      SoftError("While deserializing brush (%S).", Filename);
      *Editor->CurrentBrush = {};
    }
  }

  FinalizeDeserialization(&Bytes);
}

link_internal world_edit *
NewEdit(level_editor *Editor, world_edit_layer *Layer, world_edit_block_array_index *IndexOut = 0)
{
  world_edit_block_array_index Index = {};
  world_edit *Result = {};
  IterateOver(&Editor->Edits, Edit, EditIndex)
  {
    if (Edit->Tombstone)
    {
      Edit->Tombstone = False;
      Result = Edit;
      Index = EditIndex;
    }
  }

  if (Result == 0)
  {
    Result = Push(&Editor->Edits);
    Index = LastIndex(&Editor->Edits);
  }

  Push(&Layer->EditIndices, &Index);

  if (IndexOut)
    *IndexOut = Index;

  return Result;
}

link_internal world_edit_layer*
NewLayer(level_editor *Editor)
{
  auto Result              = Push(&Editor->Layers);
       Result->EditIndices = WorldEditBlockArrayIndexBlockArray(Editor->Memory);

  cs DefaultName = FSz("layer_%d", Editor->NextLayerIndex++);
  CopyString(DefaultName.Start, Result->NameBuf, DefaultName.Count);

  Editor->CurrentLayer = Result;
  return Result;
}

link_internal b32
InitEditor(level_editor *Editor)
{
  b32 Result = True;
  Editor->Memory = AllocateArena();

  Editor->Layers = WorldEditLayerBlockArray(Editor->Memory);
  Assert(Editor->NextLayerIndex == 0);

  Editor->AssetThumbnails = AssetThumbnailBlockArray(Editor->Memory);

  Editor->LoadedBrushes = Allocate_world_edit_brush_hashtable(128, Editor->Memory);

  file_traversal_node_block_array Nodes = GetLexicographicallySortedListOfFilesInDirectory(CSz("brushes"), GetTranArena());

  IterateOver(&Nodes, Node, NodeIndex)
  {
    if (Node->Type == FileTraversalType_File)
    {
      LoadBrushFromFile(Editor, Node, GetTranArena());
    }
  }

  Editor->Edits.Memory = Editor->Memory;
  Editor->SelectedEditIndices.Memory = Editor->Memory;

  return Result;
}

link_internal b32
HardResetEditor(level_editor *Editor)
{
  IterateOver(&Editor->AssetThumbnails, Thumb, Index)
  {
    DeleteTexture(&Thumb->Texture);
  }

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


poof(do_editor_ui_for_compound_type(world_update_op_shape_params_rect))
#include <generated/do_editor_ui_for_compound_type_world_update_op_shape_params_rect.h>
poof(do_editor_ui_for_compound_type(world_update_op_shape_params_sphere))
#include <generated/do_editor_ui_for_compound_type_world_update_op_shape_params_sphere.h>
poof(do_editor_ui_for_compound_type(world_update_op_shape_params_cylinder))
#include <generated/do_editor_ui_for_compound_type_world_update_op_shape_params_cylinder.h>

// @sdf_shape_step(4): Add the editor shit here ..


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




poof(
  for_datatypes(all)
  func (struct_t) {}
  func (enum_t) {
    enum_t.has_tag(gen_ui_toolbar)?
    {
      string_and_value_tables(enum_t)
      toolbar_for_enum(enum_t)
    }
  }
)
#include <generated/for_datatypes_kh9dwrCf.h>




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

poof(do_editor_ui_for_compound_type(world_edit_block_array_index))
#include <generated/do_editor_ui_for_compound_type_world_edit_block_array_index.h>

poof(do_editor_ui_for_container(world_edit_block_array_index_block_array))
#include <generated/do_editor_ui_for_container_world_edit_block_array_index_block_array.h>

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





poof(
  for_datatypes(all)
  func (struct_t) {
    struct_t.has_tag(do_editor_ui)?
    {
      do_editor_ui_for_compound_type(struct_t)
    }
  }
  func (enum_t) {}
)
#include <generated/for_datatypes_Bxw4Q7AW.h>




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

link_internal void
UpdateSelectionStateForFrame( ray *MouseRay,
    camera *Camera,
    input *Input,
        world_edit_selection_mode  SelectionMode,
     selection_modification_state *SelectionState)
{
  v3 Normal = NormalForFace(SelectionState->ClickedFace);
  v3 PlaneN = {};

  switch (SelectionMode)
  {
    case SelectionMode_Noop: { return; }

    case SelectionMode_TranslateLinear:
    case SelectionMode_ResizeSingleLinearAxis:
    case SelectionMode_ResizeBothLinearAxies:
    case SelectionMode_ResizeAllAxies:
    {
      v3 PerpN = Cross(Normal, Camera->Front);
      PlaneN   = Cross(Normal, PerpN);
    } break;

    case SelectionMode_TranslatePlanar:
    {
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
}

link_internal rect3i
DoSelectonModification( engine_resources *Engine,
                        ray *MouseRay,
                        world_edit_selection_mode SelectionMode,
                        selection_modification_state *SelectionState,
                        aabb SelectionAABB,
                        v3 *Diff = 0)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  v3 RoughUpdateVector = (SelectionState->ClickedP[1] - SelectionState->ClickedP[0]);

  v3 UpdateVector = ConstrainUpdateVector(RoughUpdateVector, SelectionState->ClickedFace, SelectionMode);
  rect3i Result  = ModifySelectionAABB(&SelectionAABB, V3i(UpdateVector), SelectionState->ClickedFace, SelectionMode);

  if (Diff)
  {
    *Diff = UpdateVector;
  }

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
  ZeroMemory(Brush->NameBuf, NameBuf_Len);
  cs BrushNameBuf = CS(Brush->NameBuf, NameBuf_Len);

  cs BrushBasename = Basename(CS(FilenameZ));
  CopyString(&BrushBasename, &BrushNameBuf);

  u8_cursor_block_array OutputStream = BeginSerialization();
  Serialize(&OutputStream, Brush);

  if (FinalizeSerialization(&OutputStream, FilenameZ) == False)
  {
    SoftError("Unable to serialize brush (%s) to file (%s).", Brush->NameBuf, FilenameZ);
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

link_internal world_edit_brush
NewBrush()
{
  world_edit_brush Brush = {};
  NewBrush(&Brush);
  return Brush;
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

  PushWindowStart(Ui, BrushSettingsWindow);
  Assert(Brush);
  {
    b32 IsNewBrush = False;
    layered_brush *LayeredBrush = &Brush->Layered;
    {
      b32 BrushUninitialized = Brush->NameBuf[0] == 0 && Brush->Layered.LayerCount == 0;
      if (BrushUninitialized)
      {
        NewBrush(Brush);
        IsNewBrush = True;
      }
    }

    //
    // Brush toolbar buttons
    //
    {
      memory_arena *Tran = GetTranArena();

      if (LayeredBrush->LayerCount)
      {
        ui_brush_actions BrushAction = {};
        ui_toggle_button_group Toolbar = PushToolbar(Ui, BrushSettingsWindow, CSz(""), &BrushAction);
        switch (BrushAction)
        {
          case UiBrushAction_NoAction: {} break;

          case UiBrushAction_New:
          {
            world_edit_brush ThisBrush = NewBrush();
            Editor->CurrentBrush = Insert(ThisBrush, &Editor->LoadedBrushes, Editor->Memory);
          } break;

          case UiBrushAction_Save:
          {
            cs BrushFilepath = GetFilenameForBrush(CS(Brush->NameBuf));
            SaveBrush(Brush, BrushFilepath.Start);
          } break;

          case UiBrushAction_Duplicate:
          {
            world_edit_brush Duplicated = *Brush;

            cs_buffer Pieces = Split( CS(Duplicated.NameBuf), '.', Tran);

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

                SaveBrush(&Duplicated, BrushFilepath.Start);
              }
            }
            else
            {
              cs BrushFilepath = GetFilenameForBrush(CS(Duplicated.NameBuf), 1);
              SaveBrush(&Duplicated, BrushFilepath.Start);
            }

            Editor->CurrentBrush = Insert(Duplicated, &Editor->LoadedBrushes, Editor->Memory);
          } break;

#if 0
          case UiBrushAction_Import:
          {
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
          } break;
#endif

        }
      }

      {

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
          ui_brush_layer_actions BrushLayerAction = {};

          PushTableStart(Ui);

          brush_layer *BrushLayers = Brush->Layered.Layers;
          RangeIterator(LayerIndex, LayeredBrush->LayerCount)
          {
            brush_layer *BrushLayer = BrushLayers + LayerIndex;

            ui_id ToggleId = UiId(BrushSettingsWindow, "brush_layer toggle interaction", u32(LayerIndex));
            cs LayerDetails = GetLayerUiText(BrushLayer, GetTranArena());

            if (ToggleButton(Ui, FSz("v %d %S", LayerIndex, LayerDetails), FSz("> %d %S", LayerIndex, LayerDetails), ToggleId))
            {
              ui_toggle_button_group Toolbar = PushToolbar(Ui, BrushSettingsWindow, CSz(""), &BrushLayerAction, u64(LayerIndex));
              if (Toolbar.AnyElementClicked)
              {
                EditLayerIndex = LayerIndex;

                if (BrushLayerAction == UiBrushLayerAction_Delete) { SetToggleButton(Ui, ToggleId, False); }

                b32 ThisState = GetToggleState(Ui, ToggleId);

                if (BrushLayerAction == UiBrushLayerAction_MoveUp)
                {
                  ui_id NextId = ToggleId;
                  NextId.ElementBits -= 1;
                  b32 NextState = GetToggleState(Ui, NextId);

                  SetToggleButton(Ui, ToggleId, NextState);
                  SetToggleButton(Ui, NextId, ThisState);
                }

                if (BrushLayerAction == UiBrushLayerAction_MoveDown)
                {
                  ui_id NextId = ToggleId;
                  NextId.ElementBits += 1;
                  b32 NextState = GetToggleState(Ui, NextId);

                  SetToggleButton(Ui, ToggleId, NextState);
                  SetToggleButton(Ui, NextId, ThisState);
                }

              }

              OPEN_INDENT_FOR_TOGGLEABLE_REGION();
                DoEditorUi(Ui, BrushSettingsWindow, BrushLayer, {});
              CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
            }
            else
            {
              DoColorSwatch(Ui, V2(20), HSVtoRGB(BrushLayer->Settings.HSVColor));
            }

            if (IsNewBrush && LayerIndex == 0)
            {
              SetToggleButton(Ui, ToggleId, True);
            }

            PushNewRow(Ui);
          }
          PushTableEnd(Ui);

          if (BrushLayerAction == UiBrushLayerAction_MoveUp)
          {
            if (EditLayerIndex > 0)
            {
              brush_layer *BrushLayer = BrushLayers + EditLayerIndex;
              brush_layer Tmp = BrushLayers[EditLayerIndex-1];
              BrushLayers[EditLayerIndex-1].Settings = BrushLayer->Settings;
              BrushLayer->Settings = Tmp.Settings;
            }
          }

          if (BrushLayerAction == UiBrushLayerAction_MoveDown)
          {
            if (LayeredBrush->LayerCount)
            {
              if (EditLayerIndex < LayeredBrush->LayerCount-1)
              {
                brush_layer *BrushLayer = BrushLayers + EditLayerIndex;
                brush_layer Tmp = BrushLayers[EditLayerIndex+1];
                BrushLayers[EditLayerIndex+1].Settings = BrushLayer->Settings;
                BrushLayer->Settings = Tmp.Settings;
              }
            }
          }

          if (BrushLayerAction == UiBrushLayerAction_Duplicate)
          {
            if (LayeredBrush->LayerCount < MAX_BRUSH_LAYERS)
            {
              LayeredBrush->LayerCount += 1;

              // Shuffle layers forward.  This conveniently duplicates the EditLayerIndex
              RangeIteratorReverseRange(LayerIndex, MAX_BRUSH_LAYERS, EditLayerIndex+1)
              {
                BrushLayers[LayerIndex].Settings = BrushLayers[LayerIndex-1].Settings;
              }
            }
          }

          if (BrushLayerAction == UiBrushLayerAction_Delete)
          {
            // NOTE(Jesse): Not an `if` because we shouldn't be able to ask to
            // delete a layer if there aren't any to delete!
            Assert(LayeredBrush->LayerCount > 0);

            // Shuffle layers backwards, overwriting EditLayerIndex
            RangeIteratorRange(LayerIndex, MAX_BRUSH_LAYERS, EditLayerIndex+1)
            {
              Assert(LayerIndex >= 0 && LayerIndex < MAX_BRUSH_LAYERS);
              BrushLayers[LayerIndex-1].Settings = BrushLayers[LayerIndex].Settings;
            }

            LayeredBrush->LayerCount -= 1;
          }
        }
      }

    }
  }
  PushWindowEnd(Ui, BrushSettingsWindow);
}

link_internal aabb_intersect_result
EditWorldSelection(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  aabb_intersect_result AABBTest = {};

  Editor->Selection.InitialSelect = False;
  Editor->Selection.Changed       = False;
  Editor->Selection.Diff          = {};

#if 1
  aabb TotalEditAreas = InvertedInfinityRectangle_rect3();
  // NOTE(Jesse): We can't predicate this because when we deselect everything
  // we want this to automagically resize
  /* if (AtElements(&Editor->SelectedEditIndices).Index) */
  {
    IterateOver(&Editor->SelectedEditIndices, EditIndex, EditIndexIndex)
    {
      world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
      aabb EditAABB = GetSimSpaceRect(World, Edit->Region);
      TotalEditAreas = Union(&TotalEditAreas, &EditAABB);
    }
    Editor->Selection.Region = SimSpaceToCanonical(World, &TotalEditAreas);
    /* Info("%V3 %V3i", Editor->Selection.Region.Min.Offset); */
  }
#endif

  if (Editor->Selection.Clicks)
  {
    // Hot update selection region before we click the second point
    //
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

    // Edit the selection region
    //
    aabb SelectionAABB = GetSimSpaceRect(World, Editor->Selection.Region);
    rect3i ModifiedSelection = {};
    {
      // If we're hovering a face and click, set ClickedFace, which is the
      // source of truth that signals we're editing the selection region
      //
      if (Engine->MaybeMouseRay.Tag == Maybe_Yes)
      {
        ray Ray = Engine->MaybeMouseRay.Ray;

        AABBTest = Intersect(&SelectionAABB, &Ray);
        face_index Face = AABBTest.Face;
        if (Face)
        {
          if ( Input->LMB.Clicked && (Input->Ctrl.Pressed || Input->Shift.Pressed || Input->Alt.Pressed) )
          {
            v3 PlaneBaseP = Ray.Origin + (AABBTest.t*Ray.Dir);
            Editor->Selection.ModState.ClickedFace = Face;
            Editor->Selection.ModState.ClickedP[0] = PlaneBaseP;
          }
        }

        // If we're editing the selection region, Compute the proposed modification
        // and make it permanent when we release the LMB
        //
        if (Editor->Selection.ModState.ClickedFace)
        {
          world_edit_selection_mode SelectionMode = ComputeSelectionMode(Input);
          if (SelectionMode) // We could have started a selection edit and released the accelerator key
          {
            UpdateSelectionStateForFrame( &Ray, Camera, Input, SelectionMode, &Editor->Selection.ModState );
            v3 UpdateVector = {};
            ModifiedSelection = DoSelectonModification(Engine, &Ray, SelectionMode, &Editor->Selection.ModState, SelectionAABB, &UpdateVector);
            if (Input->LMB.Pressed == False)
            {
              // If we actually changed the selection region
              rect3cp ProposedSelection = SimSpaceToCanonical(World, &ModifiedSelection);

              // Make ModifiedSelection permanent
              Editor->Selection.Region = ProposedSelection;
              Editor->Selection.Diff   = UpdateVector;
            }
          }
        }
      }

      // Draw
      {
        {
          auto Face = Editor->Selection.ModState.ClickedFace;
          if (Face == FaceIndex_None) { Face = AABBTest.Face; }

          if (Face)
          {
            /* r32 InsetWidth = 0.25f; */
            r32 InsetWidth  = 0.f;
            v3  HiColor     = RGB_GREEN;
            r32 HiThickness = EDITOR_DEFAULT_SELECTION_THICKNESS*2.5f;

            HighlightFace(Engine, Face, SelectionAABB, InsetWidth, HiColor, HiThickness);
          }
        }

        // Draw proposed modification region
        //
        rect3 ModifiedAABB = Rect3(&ModifiedSelection);
        DEBUG_DrawSimSpaceAABB(Engine, &ModifiedAABB, RGB_GREEN, EDITOR_DEFAULT_SELECTION_THICKNESS*0.75f);

        // And finally the selection region
        //
        DEBUG_DrawSimSpaceAABB(Engine, &SelectionAABB, RGB_GREEN, EDITOR_DEFAULT_SELECTION_THICKNESS);

      }
    }
  }

  // Detect changes
  //

  // TODO(Jesse): Can we actually just cache the PrevRegion at the top of this
  // function and check if we changed it like that?  I guess that's not robust
  // if we change it in other ways, but I don't think that's likely..
  {
    if (Input->LMB.Clicked)
    {
      switch (Editor->Selection.Clicks)
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
          Editor->Selection.InitialSelect = True;
        } break;

        case 2: {} break;

        InvalidDefaultCase;
      }
    }

    // Don't fire selection changed event when dragging selection with selection edit tool
    if (Editor->Selection.Clicks != 1)
    /* if (SelectionComplete(Editor->Selection.Clicks)) */ // NOTE(Jesse): Should be able to be this .. Didn't test
    {
      if (AreEqual(Editor->Selection.Region, Editor->Selection.PrevRegion))
      {
        // Same as prev frame, Changed already reset at top of function
      }
      else
      {
        Editor->Selection.Changed = True;
      }
      Editor->Selection.PrevRegion = Editor->Selection.Region;
    }
  }

  if (Editor->Selection.InitialSelect) { Info("InitialSelect"); }
  if (Editor->Selection.Changed)       { Info("Changed"); }

  if (Editor->Selection.InitialSelect)
  {
    Assert(Editor->Selection.Changed);
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
  Assert(Edit->Tombstone == False);

  Info("Applying Edit(%p) to Octree", Edit);

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

      Push(&Node->Edits, Edit);

      /* Assert(Node->Type == OctreeNodeType_Leaf); */

      Node->Dirty = True;
      ReleaseFutex(&Node->Lock);
    }
  }
}

link_internal void
DropEditFromOctree(engine_resources *Engine, world_edit *Edit, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(Edit->Tombstone == False);

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

link_internal void
UpdateWorldEditBounds(engine_resources *Engine, world_edit *Edit, rect3cp Region, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(Edit->Tombstone == False);

  DropEditFromOctree(Engine, Edit, TempMemory);

  Edit->Region = Region;

  ApplyEditToOctree(Engine, Edit, TempMemory);
}

#if 0

link_internal void
IncrementAllEditOrdinalsAbove(world_edit_block_array *Edits, u32 Ordinal)
{
  IterateOver(Edits, Edit, EditIndex)
  {
    if (Edit->Ordinal >= Ordinal)
    {
      if (Edit->Ordinal == EDIT_ORDINAL_TOMBSTONE) { continue; }
      ++Edit->Ordinal;
    }
  }
}


// NOTE(Jesse): @duplicated_edit_ordinal_sort_code
link_internal sort_key_buffer
GetEditsSortedByOrdianl(world_edit_block_array *Edits, memory_arena *TempMem)
{
  umm EditCount = TotalElements(Edits);
  sort_key *Keys = Allocate(sort_key, TempMem, EditCount);

  IterateOver(Edits, Edit, EditIndex)
  {
    u32 KeyIndex = u32(GetIndex(&EditIndex));
    Keys[KeyIndex] = {u64(Edit), u64(Edit->Ordinal)};
  }

  BubbleSort_descending(Keys, u32(EditCount));

  return {EditCount, Keys};
}
#endif


link_internal void
ApplyDiffToEditBuffer(engine_resources *Engine, v3 Diff, world_edit_block_array_index_block_array *Edits, world_edit_selection_mode SelectionMode)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  IterateOver(Edits, EditI, EditII)
  {
    world_edit *Edit = GetPtr(&Editor->Edits, *EditI);

    Assert(Engine->MaybeMouseRay.Tag == Maybe_Yes);
    ray *Ray = &Engine->MaybeMouseRay.Ray;
    aabb SelectionAABB = GetSimSpaceAABB(World, &Edit->Region);
    rect3i ModifiedSelection = DoSelectonModification(Engine, Ray, SelectionMode, &Editor->Selection.ModState, SelectionAABB);

    rect3cp NewRegion = SimSpaceToCanonical(World, &ModifiedSelection);
    UpdateWorldEditBounds(Engine, Edit, NewRegion, GetTranArena());
  }
}

link_internal void
DeleteEdit(engine_resources *Engine, world_edit *Edit, world_edit_block_array_index *EditIndex, world_edit_layer *Layer = 0)
{
  Assert(Edit->Tombstone == False);

  level_editor *Editor = &Engine->Editor;

  DropEditFromOctree(Engine, Edit, GetTranArena());

  umm SelIndex = IndexOfValue(&Editor->SelectedEditIndices, EditIndex).Index;
  if (SelIndex != INVALID_BLOCK_ARRAY_INDEX)
  {
    RemoveUnordered(&Editor->SelectedEditIndices, {SelIndex});
  }

  if (Layer)
  {
    RemoveOrdered(&Layer->EditIndices, EditIndex);
  }

  *Edit = {};
   Edit->Tombstone = True;
}

link_internal void
DoWorldEditor(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  if (Editor->CurrentBrush == 0)
  {
    world_edit_brush ThisBrush = NewBrush();
    Editor->CurrentBrush = Insert(ThisBrush, &Editor->LoadedBrushes, Editor->Memory);
  }


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
      case WorldEdit_Tool_Disabled: {} break;

      case WorldEdit_Tool_Select:
      {
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

  if (Input->Ctrl.Pressed && Input->G.Clicked)
  {
    if (AtElements(&Editor->SelectedEditIndices).Index)
    {
      if (entity *Ghost = GetCameraGhost(Engine))
      {
        Ghost->P = GetSelectionCenterP(World, Editor);
      }
    }
  }

  if (Input->Ctrl.Pressed && Input->S.Clicked)
  {
    if (Editor->CurrentLayer == 0)
    {
      NewLayer(Editor);
    }

    Editor->PreviousTool = Editor->Tool;
    Editor->Tool = WorldEdit_Tool_Select;
    ResetSelection(Editor);

    world_edit_block_array_index Index;
    auto E = NewEdit(Editor, Editor->CurrentLayer, &Index);
    E->Brush = Editor->CurrentBrush;

    Editor->SelectedEditIndices.ElementCount = 0;
    Push(&Editor->SelectedEditIndices, &Index);
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
    local_persist window_layout AllBrushesWindow = WindowLayout("All Brushes", WindowLayoutFlag_Align_BottomRight);
    PushWindowStart(Ui, &AllBrushesWindow);

    IterateOver(&Editor->LoadedBrushes, Brush, BrushIndex)
    {
      if (Brush)
      {
        ui_style *Style = &DefaultStyle;
        if (Brush == Editor->CurrentBrush)
        {
          Style = &DefaultSelectedStyle;
        }

        if (Button(Ui, CS(Brush->NameBuf), UiId(&AllBrushesWindow, "brush select", Brush), Style))
        {
          Editor->CurrentBrush = Brush;
          CheckSettingsChanged(&Brush->Layered); // Prevent firing a change event @prevent_change_event
        }
        PushNewRow(Ui);
      }
    }
    PushWindowEnd(Ui, &AllBrushesWindow);

    local_persist window_layout BrushSettingsWindow = WindowLayout("Brush Settings", WindowLayoutFlag_Align_Right);
    DoBrushSettingsWindow(Engine, Editor->CurrentBrush, &BrushSettingsWindow);

    // NOTE(Jesse): Must come after the settings window draws because the
    // settings window detects and initializes new brushes
    if (SelectionComplete(Editor->Selection.Clicks) && Editor->CurrentBrush)
    {
      if (Editor->Selection.InitialSelect)
      {
        Info("Setting Initial edit state");
        Assert(AtElements(&Editor->SelectedEditIndices).Index == 1);
        auto EditIndex = GetPtr(&Editor->SelectedEditIndices, {});
        world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
        UpdateWorldEditBounds(Engine, Edit, Editor->Selection.Region, GetTranArena());
      }
      else if (Editor->Selection.Changed)
      {
        if (LengthSq(Editor->Selection.Diff) > 0.f)
        {
          Info("Applying diff to edit buffer");
          world_edit_selection_mode SelectionMode = ComputeSelectionMode(Input);
          Assert(SelectionMode);
          ApplyDiffToEditBuffer(Engine, Editor->Selection.Diff, &Editor->SelectedEditIndices, SelectionMode);
          Editor->Selection.ModState.ClickedFace = FaceIndex_None;
        }
        else
        {
          // The selection area was probably just resizing to enclose multiple selected edits
        }
      }

      b32 SettingsChanged = CheckSettingsChanged(&Editor->CurrentBrush->Layered);
      if (SettingsChanged)
      {
        IterateOver(&Editor->Edits, Edit, EditIndex)
        {
          if (Edit->Tombstone) { continue; }

          if (Edit->Brush == Editor->CurrentBrush)
          {
            // TODO(Jesse): We should be able to just mark the overlapping
            // nodes dirty because we're not actually updating the edit bounds here..
            UpdateWorldEditBounds(Engine, Edit, Edit->Region, GetTranArena());
          }
        }
      }
    }

  }

  {
    local_persist window_layout LayersWindow = WindowLayout("Layers", WindowLayoutFlag_Align_Bottom);
    PushWindowStart(Ui, &LayersWindow);

    if (Button(Ui, CSz("New Layer"), UiId(&LayersWindow, "new layer", 0ull)))
    {
      NewLayer(Editor);
    }

    PushNewRow(Ui);
    PushNewRow(Ui);

    PushTableStart(Ui);

    world_edit_layer *PrevLayer = {};

    IterateOver(&Editor->Layers, Layer, LayerIndex)
    {
      cs Name = CS(Layer->NameBuf);
      b32 LayerSelected = Editor->CurrentLayer == Layer;

      PushTableStart(Ui);
      {
        ui_style *Style = LayerSelected ? &DefaultSelectedStyle : &DefaultStyle;
        if (Button(Ui, Name, UiId(&LayersWindow, Layer, Layer), Style))
        {
          Editor->CurrentLayer = Layer;

          Editor->SelectedEditIndices.ElementCount = 0;
          IterateOver(&Layer->EditIndices, EditIndex, EII)
          {
            Push(&Editor->SelectedEditIndices, EditIndex);
          }
        }

        ui_layer_toolbar_actions LayerToolbarAction = {};
        ui_toggle_button_group Toolbar = PushToolbar(Ui, &LayersWindow, {}, &LayerToolbarAction, u64(Layer));
        if (Toolbar.AnyElementClicked)
        {
          switch (LayerToolbarAction)
          {
            case LayerToolbarActions_NoAction: {} break;

            case LayerToolbarActions_Rename:
            {
            } break;

            case LayerToolbarActions_Duplicate:
            {
              auto SrcLayer = Layer;
              auto DstLayer = NewLayer(Editor);

              IterateOver(&SrcLayer->EditIndices, EditIndex, EditIndexIndex)
              {
                world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);

                world_edit_block_array_index DupIndex = {};
                auto *Duplicated = NewEdit(Editor, DstLayer, &DupIndex);
                     *Duplicated = *Edit;

                ApplyEditToOctree(Engine, Duplicated, GetTranArena());
              }
            } break;

            case LayerToolbarActions_Delete:
            {
              IterateOver(&Layer->EditIndices, EditIndex, EditIndexIndex)
              {
                world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
                // NOTE(Jesse): We're deleting the layer here so we don't need
                // to remove the edits one-by-one .. we'll clear the whole thing.
                DeleteEdit(Engine, Edit, EditIndex);
              }

              Layer->EditIndices.ElementCount = 0;

              RemoveOrdered(&Editor->Layers, Layer);
              Layer = 0;

              Editor->CurrentLayer = 0;
              LayerIndex = AtElements(&Editor->Layers);
            } break;
          }
        }

      }
      PushTableEnd(Ui);

      if (Layer)
      {
        PushTableStart(Ui);
        if (AtElements(&Layer->EditIndices).Index == 0)
        {
          PushColumn(Ui, CSz("--- no edits ---"));
          PushNewRow(Ui);
        }
        else
        {
          {
            world_edit_block_array_index *PrevEditIndex = {};
            IterateOver(&Layer->EditIndices, EditIndex, EditIndexIndex)
            {
              world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
              Assert(Edit->Tombstone == False);

              const char *NameBuf = Edit->Brush ? Edit->Brush->NameBuf : "no brush";

              ui_reorder_action EditReorderAction = {};
              PushToolbar(Ui, &LayersWindow, {}, &EditReorderAction, u64(Edit)^u64(Layer), &DefaultUiRenderParams_Toolbar, ToggleButtonGroupFlags_NoNewRow);

              switch(EditReorderAction)
              {
                case UiReorderAction_NoAction: break;

                case UiReorderAction_ReorderUp:
                {
                  if (PrevEditIndex)
                  {
                    auto Tmp = *PrevEditIndex;
                    *PrevEditIndex = *EditIndex;
                    *EditIndex = Tmp;
                  }
                  else if (PrevLayer)
                  {
                    Push(&PrevLayer->EditIndices, EditIndex);
                    RemoveOrdered(&Layer->EditIndices, EditIndex);
                  }

                } break;

                case UiReorderAction_ReorderDown:
                {
                  auto NextEditIndexIndex = EditIndexIndex;
                     ++NextEditIndexIndex;

                  if (NextEditIndexIndex < AtElements(&Layer->EditIndices))
                  {
                    auto NextEditIndex = GetPtr(&Layer->EditIndices, NextEditIndexIndex);

                    auto Tmp = *NextEditIndex;
                    *NextEditIndex = *EditIndex;
                    *EditIndex = Tmp;

                    EditIndexIndex = AtElements(&Layer->EditIndices);
                  }
                  else
                  {
                    auto NextLayerIndex = LayerIndex;
                       ++NextLayerIndex;

                    auto NextLayer = TryGetPtr(&Editor->Layers, GetIndex(&NextLayerIndex));
                    if (NextLayer)
                    {
                      RemoveOrdered(&Layer->EditIndices, EditIndex);
                      Shift(&NextLayer->EditIndices, EditIndex);
                    }

                    EditIndexIndex = AtElements(&Layer->EditIndices);
                  }
                }

                break;
              }


              ui_render_params ButtonParams = DefaultUiRenderParams_Button;

              b32 EditIsSelected = False;
              world_edit_block_array_index_block_array_index I;
              IterateOver(&Editor->SelectedEditIndices, SelEditIndex, SEII)
              {
                if (*SelEditIndex == *EditIndex)
                {
                  ButtonParams.FStyle = &DefaultSelectedStyle;
                  EditIsSelected = True;
                  I = SEII;
                }
              }

              auto EditSelectButton = PushSimpleButton(Ui, FSz("(%s)", NameBuf), UiId(&LayersWindow, "edit select", Edit), &ButtonParams);
              if (Clicked(Ui, &EditSelectButton))
              {
                Editor->Selection.Clicks = 2;
                Editor->Selection.Region = Edit->Region;
                Editor->Selection.PrevRegion = Edit->Region;

                if (Input->Ctrl.Pressed)
                {
                  if (EditIsSelected)
                  {
                    RemoveUnordered(&Editor->SelectedEditIndices, I);
                  }
                  else
                  {
                    Push(&Editor->SelectedEditIndices, EditIndex);
                  }
                }
                else
                {
                  Editor->SelectedEditIndices.ElementCount = 0;
                  Push(&Editor->SelectedEditIndices, EditIndex);
                }


                /* Editor->CurrentEdit = Edit; */
                Editor->CurrentLayer = Layer;

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

              ui_layer_edit_actions LayerEditAction = {};
              PushToolbar( Ui, &LayersWindow, {}, &LayerEditAction, u64(Edit));
              switch (LayerEditAction)
              {
                case UiLayerEditAction_NoAction: {} break;

                case UiLayerEditAction_SetBrush:
                {
                  Assert(Editor->CurrentBrush);
                  Edit->Brush = Editor->CurrentBrush;
                  UpdateWorldEditBounds(Engine, Edit, Edit->Region, GetTranArena());
                } break;

                case UiLayerEditAction_Duplicate:
                {
                  auto *Duplicated = NewEdit(Editor, Layer);
                       *Duplicated = *Edit;
                  ApplyEditToOctree(Engine, Duplicated, GetTranArena());
                } break;

                case UiLayerEditAction_Delete:
                {
                  DeleteEdit(Engine, Edit, EditIndex, Layer);
                } break;
              }

              PrevEditIndex = EditIndex;
            }
          }
        }
        PushTableEnd(Ui);
        PushNewRow(Ui);

        PrevLayer = Layer;
      }
    }

    PushNewRow(Ui);
    PushTableEnd(Ui);
    PushWindowEnd(Ui, &LayersWindow);

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
      IterateOver(&Layer->EditIndices, EditIndex, EditIndexIndex)
      {
        world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
        Assert(Edit->Tombstone == False);

        auto EditAABB = GetSimSpaceAABB(World, Edit->Region);
        random_series S = {u64(Edit)};
        v3 BaseColor = RandomV3Unilateral(&S);

        f32 Size = EDITOR_DEFAULT_SELECTION_THICKNESS;

        // Always highlight the hot edit
        if (Edit == Editor->HotEdit)
        {
          Size *= 3.f;
          DEBUG_DrawSimSpaceAABB(Engine, &EditAABB, BaseColor, Size);
        }
        else
        { // But otherwise only highlight the edits on the current layer.
          if (Layer == Editor->CurrentLayer)
          {
            /* if (Edit == Editor->CurrentEdit) { Size *= 2.f; } */
            DEBUG_DrawSimSpaceAABB(Engine, &EditAABB, BaseColor, Size);
          }
        }

      }
    }
    Editor->HotEdit = 0;
  }
}

link_internal void
DrawLod(engine_resources *Engine, shader *Shader, gpu_mapped_element_buffer *Mesh, r32 DistanceSquared, v3 Basis, Quaternion Rotation, v3 Scale );

link_internal void
ApplyEditBufferToOctree(engine_resources *Engine, world_edit_paged_list *Edits)
{
  IterateOver(Edits, Edit, EditIndex)
  {
    if (Edit->Tombstone) { continue; }
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
      Header.LayerCount        =  u32(AtElements(&Editor->Layers).Index);
      Header.EditCount         =  u32(AtElements(&Editor->Edits).Index);

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

      IterateOver(&Editor->Layers, Layer, LI)
      {
        Serialize(&OutputStream, Layer);
      }
      Ensure(Serialize(&OutputStream, &Delimeter));

      IterateOver(&Editor->Edits, Edit, EI)
      {
        Serialize(&OutputStream, Edit);
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

        RangeIterator_t(u32, LayerIndex, LevelHeader.LayerCount)
        {
          world_edit_layer DeserLayer =  {};
          Deserialize(&LevelBytes, &DeserLayer, Editor->Memory);
          world_edit_layer *FinalLayer = Push(&Editor->Layers, &DeserLayer);
          FinalLayer->EditIndices.Memory = Editor->Memory;
        }
        Ensure(Read_u64(&LevelBytes) == Delimeter);

        RangeIterator_t(u32, EditIndex, LevelHeader.EditCount)
        {
          world_edit DeserEdit = {}; // TODO(Jesse): Do we actually have to clear this?
          Deserialize(&LevelBytes, &DeserEdit, Editor->Memory);

          world_edit *FinalEdit = Push(&Editor->Edits, &DeserEdit);
          if (FinalEdit->Brush)
          {
            FinalEdit->Brush = Upsert(*FinalEdit->Brush, &Editor->LoadedBrushes, Editor->Memory);
          }

          ApplyEditToOctree(Engine, FinalEdit, GetTranArena());
        }
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

