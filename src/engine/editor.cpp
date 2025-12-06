

link_internal level_editor *
GetEditor()
{
  engine_resources *Engine = GetEngineResources();
  return &Engine->Editor;
}


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

    if (Deserialize(&Bytes, Editor->CurrentBrush, &Global_PermMemory) == False)
    {
      SoftError("While deserializing brush (%S).", Filename);
      *Editor->CurrentBrush = {};
    }
  }

  FinalizeDeserialization(&Bytes);
}

link_internal void
LoadPrefabFromFile(level_editor *Editor, file_traversal_node *FileNode, memory_arena *TempMemory)
{
  Assert(FileNode->Type == FileTraversalType_File);
  cs Filename = Concat(FileNode->Dir, CSz("/"), FileNode->Name, TempMemory);

  u8_cursor Bytes = BeginDeserialization(Filename, TempMemory);

  if (Bytes.Start)
  {
    prefab P = {};
    /* CopyString( FileNode->Name.Start, P.NameBuf, Min(umm(FileNode->Name.Count), umm(NameBuf_Len))); */
    P.Name = CopyString(FileNode->Name, &Editor->Heap);

    Editor->SelectedPrefab = Upsert(P, &Editor->Prefabs, &Global_PermMemory);

    if (Deserialize(&Bytes, Editor->SelectedPrefab, &Global_PermMemory) == False)
    {
      SoftError("While deserializing prefab (%S).", Filename);
      *Editor->SelectedPrefab = {};
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
    if (Edit->Flags & WorldEditFlag_Tombstone)
    {
      UnsetBitfield(u32, Edit->Flags, WorldEditFlag_Tombstone);
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

link_internal world_edit *
DuplicateEdit(level_editor *Editor, world_edit_layer *Layer, world_edit *Edit, world_edit_block_array_index *IndexOut)
{
  world_edit_block_array_index DupIndex = {};
  auto *Duplicated = NewEdit(Editor, Layer, &DupIndex);
       *Duplicated = *Edit;
        Duplicated->Selected = False;

  *IndexOut = DupIndex;

  return Duplicated;
}


link_internal world_edit_layer *
NewLayer(level_editor *Editor)
{
  auto ResultIndex         = AtElements(&Editor->Layers);
  auto Result              = Push(&Editor->Layers);
       Result->EditIndices = WorldEditBlockArrayIndexBlockArray(Editor->Memory);

  cs DefaultName = FSz("layer_%d", Editor->NextLayerIndex++);
  CopyString(DefaultName.Start, Result->NameBuf, DefaultName.Count);

  Editor->SelectedLayerIndex = ResultIndex;
  return Result;
}

link_internal b32
InitEditor(level_editor *Editor)
{
  b32 Result = True;
  Editor->Memory = AllocateArena();
  Editor->Heap = InitHeap(Megabytes(32));

  Editor->Layers = WorldEditLayerBlockArray(Editor->Memory);
  Assert(Editor->NextLayerIndex == 0);

  Editor->AssetThumbnails = AssetThumbnailBlockArray(Editor->Memory);

  Editor->LoadedBrushes = Allocate_world_edit_brush_hashtable(128, Editor->Memory);
  Editor->Prefabs = Allocate_prefab_hashtable(128, Editor->Memory);

  {
    file_traversal_node_block_array Nodes = GetLexicographicallySortedListOfFilesInDirectory(CSz("brushes"), GetTranArena());
    IterateOver(&Nodes, Node, NodeIndex)
    {
      if (Node->Type == FileTraversalType_File)
      {
        LoadBrushFromFile(Editor, Node, GetTranArena());
      }
    }
  }

  {
    file_traversal_node_block_array Nodes = GetLexicographicallySortedListOfFilesInDirectory(CSz("prefabs"), GetTranArena());
    IterateOver(&Nodes, Node, NodeIndex)
    {
      if (Node->Type == FileTraversalType_File)
      {
        LoadPrefabFromFile(Editor, Node, GetTranArena());
      }
    }
  }



#if 0
  simple_brush Brushes[] = {
    {
      {CSz("Sphere")},
      {
        BrushSettings(  BrushLayerType_Shape, {},
                        ShapeLayer(  ShapeType_Sphere, {}, {}, {}, {}, {}, {}, {}),
                        {}, {}, {}, {}, {}, {}, {}, {}, {}, {} ),
      },
      0,
    }
  };

  Editor->SimpleBrushes.Memory = Editor->Memory;

  RangeIterator_t(umm, BrushIndex, ArrayCount(Brushes))
  {
    Push(&Editor->SimpleBrushes, Brushes+BrushIndex);
  }

#endif

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
  return &GetEngineResources()->EngineDebug.UiDebug;
}

poof(string_and_value_tables(visible_region_size))
#include <generated/string_and_value_tables_visible_region_size.h>
poof(do_editor_ui_for_enum(visible_region_size))
#include <generated/do_editor_ui_for_enum_visible_region_size.h>

poof(do_editor_ui_for_enum(brush_shape_type))
#include <generated/do_editor_ui_for_enum_shape_type.h>

poof(do_editor_ui_for_container(v3_cursor))
#include <generated/do_editor_ui_for_container_v3_cursor.h>

/* poof(do_editor_ui_for_container(ui_toggle_hashtable)) */
/* #include <generated/do_editor_ui_for_container_ui_toggle_hashtable.h> */



link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, ui_id *Element, cs Name, u32 ParentHash, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CSz("Id"));
  PushColumn(Ui, FSz("%x", Element->E[0]));
  PushColumn(Ui, FSz("%x", Element->E[1]));
  PushColumn(Ui, FSz("%x", Element->E[2]));
  PushColumn(Ui, FSz("%x", Element->E[3]));
  PushNewRow(Ui);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shader_uniform *Element, cs Name, u32 ParentHash, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
#if 0
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
        case ShaderUniform_S32:       { DoEditorUi(Ui, Window, Element->S32, CSz("S32"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); PushNewRow(Ui); } break;
        case ShaderUniform_U32:       { DoEditorUi(Ui, Window, Element->U32, CSz("U32"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); PushNewRow(Ui); } break;
        case ShaderUniform_R32:       { DoEditorUi(Ui, Window, Element->R32, CSz("R32"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); PushNewRow(Ui); } break;
        case ShaderUniform_Texture:   { DoEditorUi(Ui, Window, Element->Texture, CSz("Texture"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_Light:     { DoEditorUi(Ui, Window, Element->Light, CSz("Light"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
        case ShaderUniform_Camera:    { DoEditorUi(Ui, Window, Element->Camera, CSz("Camera"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); } break;
      }

      DoEditorUi(Ui, Window, &Element->ID, CSz("s32 ID"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushNewRow(Ui);
      DoEditorUi(Ui, Window, &Element->Count, CSz("u32 Count"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushNewRow(Ui);

      PushTableEnd(Ui);
      /* DoEditorUi(Ui, Window, Element->Next, CSz("shader_uniform Next"), EDITOR_UI_FUNCTION_INSTANCE_NAMES); */
    }

  }
  else
  {
    PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  }

  PushNewRow(Ui);
#else
  NotImplemented;
#endif
}
poof(do_editor_ui_for_container(shader_uniform_buffer))
#include <generated/do_editor_ui_for_container_struct.h>

/* poof(do_editor_ui_for_compound_type(shader)) */
/* #include <generated/do_editor_ui_for_compound_type_shader.h> */

poof(do_editor_ui_for_container(shader_ptr_block_array))
#include <generated/do_editor_ui_for_container_shader_ptr_block_array.h>



poof(do_editor_ui_for_compound_type(work_queue_entry))
#include <generated/do_editor_ui_for_compound_type_work_queue_entry.h>

/* poof(do_editor_ui_for_compound_type(work_queue)) */
/* #include <generated/do_editor_ui_for_compound_type_work_queue.h> */

/* poof(do_editor_ui_for_compound_type(input_event)) */
/* #include <generated/do_editor_ui_for_compound_type_input_event.h> */

/* poof(do_editor_ui_for_compound_type(input)) */
/* #include <generated/do_editor_ui_for_compound_type_input.h> */

/* poof(do_editor_ui_for_compound_type(platform)) */
/* #include <generated/do_editor_ui_for_compound_type_platform.h> */

#if BONSAI_DEBUG_SYSTEM_API
poof(do_editor_ui_for_compound_type(debug_state))
#include <generated/do_editor_ui_for_compound_type_debug_state.h>
#endif

/* poof(do_editor_ui_for_compound_type(bonsai_stdlib)) */
/* #include <generated/do_editor_ui_for_compound_type_bonsai_stdlib.h> */










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




/* poof(do_editor_ui_for_compound_type(lighting_settings)) */
/* #include <generated/do_editor_ui_for_compound_type_lighting_settings.h> */

/* poof(do_editor_ui_for_compound_type(render_settings)) */
/* #include <generated/do_editor_ui_for_compound_type_render_settings.h> */

/* poof(do_editor_ui_for_compound_type(physics)) */
/* #include <generated/do_editor_ui_for_compound_type_physics.h> */

/* poof(do_editor_ui_for_compound_type(random_series)) */
/* #include <generated/do_editor_ui_for_compound_type_random_series.h> */

poof(do_editor_ui_for_enum(particle_spawn_type))
#include <generated/do_editor_ui_for_enum_particle_spawn_type.h>

/* poof(do_editor_ui_for_compound_type(particle)) */
/* #include <generated/do_editor_ui_for_compound_type_particle.h> */

/* poof(do_editor_ui_for_compound_type(particle_system)) */
/* #include <generated/do_editor_ui_for_compound_type_particle_system.h> */

/* poof(do_editor_ui_for_compound_type(keyframe)) */
/* #include <generated/do_editor_ui_for_compound_type_keyframe.h> */

/* poof(do_editor_ui_for_compound_type(animation)) */
/* #include <generated/do_editor_ui_for_compound_type_animation.h> */

/* poof(do_editor_ui_for_compound_type(vertex_material)) */
/* #include <generated/do_editor_ui_for_compound_type_vertex_material.h> */

/* poof(do_editor_ui_for_compound_type(bonsai_futex)) */
/* #include <generated/do_editor_ui_for_compound_type_bonsai_futex.h> */

/* poof(do_editor_ui_for_compound_type(untextured_3d_geometry_buffer)) */
/* #include <generated/do_editor_ui_for_compound_type_untextured_3d_geometry_buffer.h> */

#if 0
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
#endif

/* poof(do_editor_ui_for_compound_type(plane)) */
/* #include <generated/do_editor_ui_for_compound_type_plane.h> */

/* poof(do_editor_ui_for_compound_type(frustum)) */
/* #include <generated/do_editor_ui_for_compound_type_frustum.h> */

/* poof(do_editor_ui_for_compound_type(camera)) */
/* #include <generated/do_editor_ui_for_compound_type_camera.h> */

/* poof(do_editor_ui_for_compound_type(texture)) */
/* #include <generated/do_editor_ui_for_compound_type_texture.h> */

/* poof(do_editor_ui_for_compound_type(asset_thumbnail)) */
/* #include <generated/do_editor_ui_for_compound_type_asset_thumbnail.h> */

/* poof(do_editor_ui_for_compound_type(chunk_thumbnail)) */
/* #include <generated/do_editor_ui_for_compound_type_chunk_thumbnail.h> */




poof(
  for_datatypes(enum)
  func (enum_t)
  {
    enum_t.has_tag(gen_ui_toolbar)?
    {
      string_and_value_tables(enum_t)
      toolbar_for_enum(enum_t)
    }

    enum_t.has_tag(gen_string_and_value_tables)?
    {
      string_and_value_tables(enum_t)
    }

    enum_t.has_tag(do_editor_ui)?
    {
      do_editor_ui_for_enum(enum_t)
    }
  }
)
#include <generated/for_datatypes_kh9dwrCf.h>



/* poof(do_editor_ui_for_compound_type(world_edit_brush)) */
/* #include <generated/do_editor_ui_for_compound_type_struct_world_edit_brush.h> */

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
DoEditorUi_entity_P(renderer_2d *Ui, window_layout *Window, entity *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Column)
{
  DoEditorUi(Ui, Window, &Element->P.WorldP, CSz("WorldP"), ParentHash, Params);
  DoEditorUi(Ui, Window, &Element->P.Offset, CSz("Offset"), ParentHash, Params, 0.f, 32.f);
}

poof(do_editor_ui_for_compound_type(entity_id))
#include <generated/do_editor_ui_for_compound_type_entity_id.h>

poof(do_editor_ui_for_compound_type(entity))
#include <generated/do_editor_ui_for_compound_type_entity.h>


poof(do_editor_ui_for_container(entity_ptr_block_array))
#include <generated/do_editor_ui_for_container_entity_ptr_block_array.h>

/* poof(do_editor_ui_for_compound_type(world_chunk)) */
/* #include <generated/do_editor_ui_for_compound_type_world_chunk.h> */

/* poof(do_editor_ui_for_compound_type(octree_node)) */
/* #include <generated/do_editor_ui_for_compound_type_octree_node.h> */

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




link_internal void
DoWorldEditBrushPicker(renderer_2d *Ui, window_layout *Window, layer_settings *Element, umm ParentHash);

poof(
  for_datatypes(struct)
  func (struct_t)
  {
    struct_t.has_tag(do_editor_ui)?
    {
      struct_t.has_tag(collection)?
      {
        do_editor_ui_for_container(struct_t)
      }
      {
        do_editor_ui_for_compound_type(struct_t)
      }
    }
  }
)
#include <generated/for_datatypes_Bxw4Q7AW.h>



poof(do_editor_ui_for_enum(engine_debug_view_mode))
#include <generated/do_editor_ui_for_enum_engine_debug_view_mode.h>

poof(do_editor_ui_for_enum(resolution_setting))
#include <generated/do_editor_ui_for_enum_resolution_setting.h>

poof(do_editor_ui_for_enum(shader_language_setting))
#include <generated/do_editor_ui_for_enum_shader_language_setting.h>


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
    MakeTexture_RGB_Async(&Engine->Stdlib.Plat.LoRenderQ, Texture, Texture->Dim, 0, CSz("NoisePreviewTexture"));
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
  UpdateGameCamera(GetWorld(), Engine->Stdlib.Plat.ScreenDim, MouseDP, CameraZDelta, {}, ThumbCamera, 0.f);

  /* if (Ui->Input->LMB.Pressed == False && Hover(Ui, &B)) { PushTooltip(Ui, ToString(Texture)); } */

  return B;
}
#endif


link_internal v3
GetHotVoxelForEditMode(engine_resources *Engine, world_edit_blend_mode WorldEditMode)
{
  v3 Result = Floor(GetSimSpaceP(Engine->World, &Engine->MousedOverVoxel.Value, PickedVoxel_FirstFilled));
  return Result;
}

#if 0
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
#endif

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

  brush_layer *Layers =  Brush->Layers;
  RangeIterator(LayerIndex, MAX_BRUSH_LAYERS)
  {
    brush_layer *Layer = Layers + LayerIndex;
    Layer->Settings = {};
  }

  Brush->LayerCount = 1;

  // Initialize PrevSettings so we don't fire a changed event straight away..
  // @prevent_change_event
  CheckSettingsChanged(Brush);
}

link_internal world_edit_brush
NewBrush()
{
  world_edit_brush Brush = {};
  NewBrush(&Brush);
  return Brush;
}

link_internal cs
GetBrushLayerUiText(layer_settings *Settings, memory_arena *TempMem)
{
  cs LayerType = ToStringPrefixless(Settings->Type);
  cs SubType = {};
  switch (Settings->Type)
  {
    case BrushLayerType_Brush: { SubType = Settings->Brush ? CS(Settings->Brush->NameBuf) : CSz("null"); } break;
    case BrushLayerType_Noise: { SubType = ToStringPrefixless(Settings->Noise.Type); } break;
    case BrushLayerType_Shape: { SubType = ToStringPrefixless(Settings->Shape.Type); } break;
  }

  return FSz("%S(%S)", LayerType, SubType);
}

link_internal void
DoColorSwatch(renderer_2d *Ui, v2 QuadDim, v3 RGB)
{
  ui_style Style = FlatUiStyle(RGB);
  PushUntexturedQuad(Ui, {}, QuadDim, zDepth_Text, &Style, {} );
}

struct ui_action_result
{
  ui_editor_action Action;
  ui_id Id;
};

link_internal ui_action_result
DoEditorActionsButtons(renderer_2d *Ui, window_layout *Window, ui_id BaseId, u32 EnabledBits, v3 Tint = {})
{
  // Why would we ever call this if we didn't wanna draw anything?
  Assert(EnabledBits);

  ui_toggle_button_handle ButtonHandles[UiEditorAction_Count];

  u32 ButtonCount = 0;
  RangeIterator(Action, UiEditorAction_Count)
  {
    if ( EnabledBits & (1<<Action) )
    {
      ButtonHandles[ButtonCount++] = ButtonHandleForEnumValue(Ui, ui_editor_action(Action), BaseId);
    }
  }
  Assert(ButtonCount == CountBitsSet_Kernighan(EnabledBits));

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ButtonCount,
    ButtonHandles
  };

  ui_editor_action Action = UiEditorAction_NoAction;

  ui_toggle_button_group Result = {};
  Result.Ui = Ui;
  Result.Flags = ui_toggle_button_group_flags(ToggleButtonGroupFlags_TypeClickButton | ToggleButtonGroupFlags_NoNewRow);
  Result.Buttons = ButtonBuffer;
  Result.EnumStorage = Cast(u32*, &Action);

  auto Params = DefaultUiRenderParams_Button;
  if (LengthSq(Tint) > 0.f)
  {
    auto Style = UiStyleFromLightestColor(Tint);
    Params.FStyle = &Style;
  }

  DrawButtonGroup(&Result, {}, &Params, &DefaultUiRenderParams_Toolbar);

  Assert(IsValid(Action));

  return {Action, Result.ClickedId};
}

link_internal void
DoBrushDetailsWindow(engine_resources *Engine, world_edit_brush *Brush, window_layout *BrushSettingsWindow)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(u32(u64(BrushSettingsWindow)));

  PushWindowStart(Ui, BrushSettingsWindow);
  Assert(Brush);
  {
    b32 IsNewBrush = False;
    {
      b32 BrushUninitialized = Brush->NameBuf[0] == 0 && Brush->LayerCount == 0;
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

      {
        u32 IconBits =
          (1 << UiEditorAction_New)  |
          (1 << UiEditorAction_Save) |
          (1 << UiEditorAction_Duplicate);

        ui_id BaseId = UiId(BrushSettingsWindow, Brush, 0, 0);

        ui_action_result BrushActionResult = DoEditorActionsButtons(Ui, BrushSettingsWindow, BaseId, IconBits);
        switch (BrushActionResult.Action)
        {
          case UiEditorAction_NoAction: {} break;

          case UiEditorAction_New:
          {
            world_edit_brush ThisBrush = NewBrush();
            Editor->CurrentBrush = Insert(ThisBrush, &Editor->LoadedBrushes, Editor->Memory);
          } break;

          case UiEditorAction_Save:
          {
            cs BrushFilepath = GetFilenameForBrush(CS(Brush->NameBuf));
            SaveBrush(Brush, BrushFilepath.Start);
          } break;

          case UiEditorAction_Duplicate:
          {
            world_edit_brush Duplicated = *Brush;

            cs_buffer Pieces = Split( CS(Duplicated.NameBuf), '.', GetTranArena());

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

          InvalidDefaultCase;
        }
      }

      {

        {
          PushNewRow(Ui);
          PushNewRow(Ui);
          PushTableStart(Ui);

          {
            ui_id TextBoxId = UiId(BrushSettingsWindow, "name_buf_textbox", Brush->NameBuf);
            cs NameBuf = CS(Brush->NameBuf);
            TextBox(Ui, CSz("BrushName"), NameBuf, NameBuf_Len, TextBoxId);
            PushNewRow(Ui);

            /* DoEditorUi(Ui, BrushSettingsWindow, &Brush->LayerCount, CSz("Layer Count"), ThisHash, &DefaultUiRenderParams_Generic); */
            /* // Clamp LayerCount to (1,MAX_BRUSH_LAYERS) once it's set */
            /* Brush->LayerCount = Max(Brush->LayerCount, 1); */
            /* Brush->LayerCount = Min(Brush->LayerCount, MAX_BRUSH_LAYERS); */
            /* PushNewRow(Ui); */
            /* PushNewRow(Ui); */
          }

          {
            if (DoEditorUi(Ui, BrushSettingsWindow, &Brush->BrushBlendMode,     CSz("Blend"), ThisHash, &DefaultUiRenderParams_Generic))
            {
              /* ReapplyEditsUsingBrush(Engine, Brush); */
            }

            /* if (Brush->BrushBlendMode == WorldEdit_Mode_SmoothUnion) */
            {
              DoEditorUi(Ui, BrushSettingsWindow, &Brush->Smoothing, CSz("Smoothing"), ThisHash, &DefaultUiRenderParams_Generic);
              {
                /* ReapplyEditsUsingBrush(Engine, Brush); */
              }
            }
          } PushTableEnd(Ui); { /* DoEditorUi(Ui, BrushSettingsWindow, &Brush->AffectExisting, CSz("Affect World"), ThisHash, &DefaultUiRenderParams_Checkbox); */ /* DoEditorUi(Ui, BrushSettingsWindow, &Brush->AffectExisting, CSz("Affect World"), ThisHash, &DefaultUiRenderParams_Checkbox); */ /* PushNewRow(Ui); */ /* PushNewRow(Ui); */

            PushNewRow(Ui);
            PushColumn(Ui, FSz(" ----- LAYERS (%d/%d) ", Brush->LayerCount, MAX_BRUSH_LAYERS), &DefaultUiRenderParams_Generic);

            if (Button(Ui, &Ui->IconTextureArray, UiIconIndex_Add, UiId(BrushSettingsWindow, "brush layer add", Brush, 0)))
            {
              Brush->LayerCount = Min(Brush->LayerCount+1, MAX_BRUSH_LAYERS);
            }

            PushColumn(Ui, CSz(" -----"), &DefaultUiRenderParams_Generic);
            PushNewRow(Ui);
            PushNewRow(Ui);

            /* DoEditorUi(Ui, BrushSettingsWindow, &Brush->BrushFollowsCursor,      CSz("BrushFollowsCursor"),      &DefaultUiRenderParams_Checkbox); */
            /* PushNewRow(Ui); */
            /* PushNewRow(Ui); */
          }

        }

        {
          s32 EditLayerIndex = 0;
          ui_editor_action BrushLayerAction = {};

          PushTableStart(Ui);

          brush_layer *BrushLayers = Brush->Layers;
          RangeIterator(LayerIndex, Brush->LayerCount)
          {
            brush_layer *BrushLayer = BrushLayers + LayerIndex;

            ui_id ToggleId = UiId(BrushSettingsWindow, "brush_layer toggle interaction", ThisHash, u32(LayerIndex));


            ui_id BaseId = UiId(BrushSettingsWindow, BrushLayer, 0, 0);

            u32 EyeIcon = BrushLayer->Settings.Disabled?  (1<<UiEditorAction_Show) : (1<<UiEditorAction_Hide);
            u32 IconBits =
              EyeIcon |
              (1 << UiEditorAction_ReorderUp)   |
              (1 << UiEditorAction_ReorderDown) |
              (1 << UiEditorAction_Duplicate)   ;

            ui_action_result BrushActionResult = DoEditorActionsButtons(Ui, BrushSettingsWindow, BaseId, IconBits);

            switch (BrushActionResult.Action)
            {
              InvalidDefaultCase;

              case UiEditorAction_NoAction: break;

              case UiEditorAction_Show:
              case UiEditorAction_Hide:
              {
                BrushLayer->Settings.Disabled = !BrushLayer->Settings.Disabled;
              } break;


              case UiEditorAction_ReorderDown:
              case UiEditorAction_Duplicate:
              case UiEditorAction_ReorderUp:
              {
                BrushLayerAction = BrushActionResult.Action;
                EditLayerIndex = LayerIndex;
              } break;
            }

            cs LayerName = GetBrushLayerUiText(&BrushLayer->Settings, GetTranArena());
            if (Button(Ui, FSz("%S", LayerName), ToggleId))
            {
              Editor->CurrentBrush_SelectedLayerIndex = LayerIndex;
            }

            DoColorSwatch(Ui, V2(20), HSVtoRGB(BrushLayer->Settings.HSVColor));
            PushForceAdvance(Ui, V2(10,0));

            auto ButtonId = UiId(BrushSettingsWindow, "brush_layer delete", ThisHash, u32(LayerIndex));
            if (Button(Ui, &Ui->IconTextureArray, UiIconIndex_Trash, ButtonId))
            {
              BrushLayerAction = UiEditorAction_Delete;
              EditLayerIndex = LayerIndex;
            }

            if (IsNewBrush && LayerIndex == 0)
            {
              SetToggleButton(Ui, ToggleId, True);
            }

            PushNewRow(Ui);
          }
          PushTableEnd(Ui);

          PushNewRow(Ui);
          PushColumn(Ui, CSz(" ----- DETAILS -----"));
          PushNewRow(Ui);
          PushNewRow(Ui);


          PushTableStart(Ui);
            OPEN_INDENT_FOR_TOGGLEABLE_REGION();
              {
                if (Editor->CurrentBrush_SelectedLayerIndex >= 0 &&
                    Editor->CurrentBrush_SelectedLayerIndex <= Editor->CurrentBrush->LayerCount)
                {
                  auto BrushLayer = Editor->CurrentBrush->Layers + Editor->CurrentBrush_SelectedLayerIndex;
                  DoEditorUi(Ui, BrushSettingsWindow, BrushLayer, {}, ThisHash);
                }
              }
            CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
          PushTableEnd(Ui);

          if (BrushLayerAction == UiEditorAction_ReorderUp)
          {
            if (EditLayerIndex > 0)
            {
              brush_layer *BrushLayer = BrushLayers + EditLayerIndex;
              brush_layer Tmp = BrushLayers[EditLayerIndex-1];
              BrushLayers[EditLayerIndex-1].Settings = BrushLayer->Settings;
              BrushLayer->Settings = Tmp.Settings;
            }
          }

          if (BrushLayerAction == UiEditorAction_ReorderDown)
          {
            if (Brush->LayerCount)
            {
              if (EditLayerIndex < Brush->LayerCount-1)
              {
                brush_layer *BrushLayer = BrushLayers + EditLayerIndex;
                brush_layer Tmp = BrushLayers[EditLayerIndex+1];
                BrushLayers[EditLayerIndex+1].Settings = BrushLayer->Settings;
                BrushLayer->Settings = Tmp.Settings;
              }
            }
          }

          if (BrushLayerAction == UiEditorAction_Duplicate)
          {
            if (Brush->LayerCount < MAX_BRUSH_LAYERS)
            {
              Brush->LayerCount += 1;

              // Shuffle layers forward.  This conveniently duplicates the EditLayerIndex
              RangeIteratorReverseRange(LayerIndex, MAX_BRUSH_LAYERS, EditLayerIndex+1)
              {
                BrushLayers[LayerIndex].Settings = BrushLayers[LayerIndex-1].Settings;
              }
            }
          }

          if (BrushLayerAction == UiEditorAction_Delete)
          {
            // NOTE(Jesse): Not an `if` because we shouldn't be able to ask to
            // delete a layer if there aren't any to delete!
            Assert(Brush->LayerCount > 0);

            // Shuffle layers backwards, overwriting EditLayerIndex
            RangeIteratorRange(LayerIndex, MAX_BRUSH_LAYERS, EditLayerIndex+1)
            {
              Assert(LayerIndex >= 0 && LayerIndex < MAX_BRUSH_LAYERS);
              BrushLayers[LayerIndex-1].Settings = BrushLayers[LayerIndex].Settings;
            }

            Brush->LayerCount -= 1;
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

  auto PrevRegion = Editor->Selection.Region;

  aabb_intersect_result AABBTest = {};

  Editor->Selection.InitialSelect = False;
  Editor->Selection.Diff          = {};

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
  }


  if (Count(&Editor->SelectedEditIndices) == 0)
  {
    Editor->Selection.ModMode = SelectionModificationMode_None;
  }


  switch (Editor->Selection.ModMode)
  {
    case SelectionModificationMode_None:
    {
      Assert(Count(&Editor->SelectedEditIndices) == 0);
    } break;

    case SelectionModificationMode_Initialize:
    {
      Assert(SelectionIncomplete(Editor->Selection.Clicks));
      Assert(Count(&Editor->SelectedEditIndices) == 1);

      // Hot update selection region before we click the second point
      // Must come first or we get a frame of lag.
      //
      if (Engine->MousedOverVoxel.Tag)
      {
        auto MouseP = Canonical_Position(&Engine->MousedOverVoxel.Value);
        MouseP.Offset = Floor(MouseP.Offset);

        cp MinP = Min(Editor->Selection.Base, MouseP);
        cp MaxP = Max(Editor->Selection.Base, MouseP) + V3(1.f);
        /* Assert(MinP <= MaxP); */
        Editor->Selection.Region = RectMinMax(MinP, MaxP);
      }

      // Update Clicks and initial position
      //
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
            Editor->Selection.ModMode = SelectionModificationMode_Modify;
          } break;

          InvalidDefaultCase;
        }
      }
    } break;

    case SelectionModificationMode_Modify:
    {
      Assert(SelectionComplete(Editor->Selection.Clicks));
      Assert(Count(&Editor->SelectedEditIndices) > 0);
      // Edit the selection region
      //
      rect3i ModifiedSelection = {};
      {
        // If we're hovering a face and click, set ClickedFace, which is the
        // source of truth that signals we're editing the selection region
        //
        if (Engine->MaybeMouseRay.Tag == Maybe_Yes)
        {
          ray Ray = Engine->MaybeMouseRay.Ray;

          aabb SelectionAABB = GetSimSpaceRect(World, Editor->Selection.Region);
          AABBTest = Intersect(&SelectionAABB, &Ray);
          face_index Face = AABBTest.Face;
          if (Face)
          {
            if ( Input->LMB.Clicked && (Input->Shift.Pressed || Input->Alt.Pressed) )
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

              // Draw proposed modification region
              //
              rect3 ModifiedAABB = Rect3(&ModifiedSelection);
              v3 CameraSimP = GetSimSpaceP(World, Camera->CurrentP);
              r32 BaseThicc = GetSelectionThicknessForDistance( Distance(CameraSimP, GetCenter(&ModifiedAABB)) );
              DEBUG_DrawSimSpaceAABB(Engine, &ModifiedAABB, RGB_GREEN, BaseThicc*0.75f);
            }
          }
        }

      }
    } break;
  }

  if (Editor->Selection.InitialSelect) { Info("InitialSelect"); }

  return AABBTest;
}

link_internal cp
GetSelectionCenterP(world *World, level_editor *Editor)
{
  v3i Dim = GetSelectionDim(World, Editor);
  cp Result = Canonicalize(World, Editor->Selection.Region.Min + V3(Dim/2));
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
DoColorPickerSection(renderer_2d *Ui, window_layout *Window, u32 ThisHash, v3 *HSVDest, u32 HSVElementIndex, u32 Slices, v2 WidgetDim, f32 Curve)
{
  v2 QuadDim = V2(WidgetDim.x/r32(Slices), WidgetDim.y);
  v4 Padding = V4(0);
  v3 HSV = *HSVDest;

  r32 CurrentValue = HSVDest->E[HSVElementIndex];

  ui_element_reference SaturationTable = PushTableStart(Ui);
  RangeIterator_t(u8, ColorIndex, Slices)
  {
    r32 Value = r32(ColorIndex)/r32(Slices-1);
    Value = Powf(Value, Curve);

    HSV.E[HSVElementIndex] = Value;
    v3 RGB = HSVtoRGB(HSV);

    b32 Selected = Value == CurrentValue;
    ui_style Style = FlatUiStyle(RGB);
    ui_id Id = UiId( Window, "ColorPicker value button", u32(u64(HSVDest)), ThisHash ^ u32(u64(Cast(void*, u64(ColorIndex) | u64(HSVElementIndex<<16)))) );

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
DoColorPicker(renderer_2d *Ui, window_layout *Window, v3 *HSVDest, b32 ShowColorSwatch, u32 ParentHash)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ u32(u64(HSVDest)));

  u8 FourBits    = 0b1111;
  u8 FiveBits    = 0b11111;
  u8 SixBits     = 0b111111;
  u8 SevenBits   = 0b1111111;

  u16 HueSlices        = SixBits;
  u16 SaturationSlices = FiveBits;
  u16 ValueSlices      = FiveBits;

  v2 ColorPickerSectionDim = V2(256, 30);

  DoColorPickerSection(Ui, Window, ThisHash, HSVDest, 0, HueSlices,        ColorPickerSectionDim, 1.f);
  DoColorPickerSection(Ui, Window, ThisHash, HSVDest, 1, SaturationSlices, ColorPickerSectionDim, 0.5f);
  DoColorPickerSection(Ui, Window, ThisHash, HSVDest, 2, ValueSlices,      ColorPickerSectionDim, 1.2f);

  PushNewRow(Ui);

  v3 RGB = HSVtoRGB(*HSVDest);
  if (ShowColorSwatch)
  {
    v2 QuadDim = V2(ColorPickerSectionDim.x, ColorPickerSectionDim.x);
    DoColorSwatch(Ui, QuadDim, RGB);
    PushNewRow(Ui);
  }

  cs HSVColorString = FSz("HSV (%.2V3)", HSVDest);
  cs RGBColorString = FSz("RGB (%.2V3)", &RGB);

  PushColumn(Ui, HSVColorString );
  PushNewRow(Ui);

  PushColumn(Ui, RGBColorString );
  PushNewRow(Ui);
}

link_internal void
DoColorPickerToggle(renderer_2d *Ui, window_layout *Window, v3 *HSVDest, b32 ShowColorSwatch, u32 ParentHash)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ u32(u64(HSVDest)));

  ui_id InteractionId = UiId(Window, "ColorPicker toggle button", HSVDest, ThisHash);

  u32 ColumnIndex = StartColumn(Ui);
    if (ToggledOn(Ui, InteractionId))
    {
      // NOTE(Jesse): Gotta use a discrete button id for this
      if (Button(Ui, CSz("Done"), UiId(Window, "ColorPicker toggle close", HSVDest, ThisHash))) { SetToggleButton(Ui, InteractionId, False); }
      PushNewRow(Ui);
      DoColorPicker(Ui, Window, HSVDest, ShowColorSwatch, ParentHash);
    }
    else
    {
      ui_style BStyle = UiStyleFromLightestColor(HSVtoRGB(*HSVDest));
      ToggleButtonStart(Ui, InteractionId, &BStyle); //, Padding, AlignFlags);
        v2 Dim = V2(25);
        PushUntexturedQuad(Ui, V2(0), Dim, zDepth_Text, &BStyle);
      ToggleButtonEnd(Ui);
    }
  EndColumn(Ui, ColumnIndex);

  PushNewRow(Ui);
}

link_internal b32
CheckSettingsChanged(world_edit_brush *Brush)
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
    Result = CheckSettingsChanged(Edit->Brush);
  }
  return Result;
}

link_internal void
ApplyEditToOctree(engine_resources *Engine, world_edit *Edit, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert( BitfieldNotSet(Edit->Flags, WorldEditFlag_Tombstone) );

  Info("Applying Edit(%p) to Octree", Edit);

  // Gather newly overlapping nodes and add the edit

  {
    octree_node_ptr_block_array Nodes = OctreeNodePtrBlockArray(TempMemory);

    rect3cp QueryRegion = Edit->Region;


    // NOTE(Jesse):
#if 0
    {
      QueryRegion.Min.Offset -= 1.f;
      QueryRegion.Max.Offset += 1.f;
      Canonicalize(World, &QueryRegion.Min);
      Canonicalize(World, &QueryRegion.Max);
    }
#endif

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

      Node->Dirty = True;
      ReleaseFutex(&Node->Lock);
    }
  }
}

link_internal void
MarkOctreeNodesContainingEditDirty(engine_resources *Engine, world_edit *Edit, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert( BitfieldNotSet(Edit->Flags, WorldEditFlag_Tombstone) );

  {
    octree_node_ptr_block_array Nodes = OctreeNodePtrBlockArray(TempMemory);

    rect3cp QueryRegion = Edit->Region;


    // NOTE(Jesse):
#if 0
    {
      QueryRegion.Min.Offset -= 1.f;
      QueryRegion.Max.Offset += 1.f;
      Canonicalize(World, &QueryRegion.Min);
      Canonicalize(World, &QueryRegion.Max);
    }
#endif

    GatherOctreeNodesOverlapping_Recursive(World, &World->Root, &QueryRegion, &Nodes);

    IterateOver(&Nodes, Node, NodeIndex)
    {
      {
        world_edit_ptr_block_array_index Index = Find(&Node->Edits, Edit);
        Assert( IsValid(&Index) == True );
      }

      Node->Dirty = True;
    }
  }
}

#if BONSAI_INTERNAL
link_internal void
SanityCheckOctreeDoesNotContainEdit_Recursive(world *World, octree_node *Current, world_edit *Edit)
{
  Assert(Current);

  IterateOver(&Current->Edits, TestEdit, EditIndex)
  {
    Assert(Edit != TestEdit);
  }

  switch(Current->Type)
  {
    InvalidCase(OctreeNodeType_Undefined);

    case OctreeNodeType_Leaf:
    {
    } break;

    case OctreeNodeType_Branch:
    {
      SanityCheckOctreeDoesNotContainEdit_Recursive(World, Current->Children[0], Edit);
      SanityCheckOctreeDoesNotContainEdit_Recursive(World, Current->Children[1], Edit);
      SanityCheckOctreeDoesNotContainEdit_Recursive(World, Current->Children[2], Edit);
      SanityCheckOctreeDoesNotContainEdit_Recursive(World, Current->Children[3], Edit);
      SanityCheckOctreeDoesNotContainEdit_Recursive(World, Current->Children[4], Edit);
      SanityCheckOctreeDoesNotContainEdit_Recursive(World, Current->Children[5], Edit);
      SanityCheckOctreeDoesNotContainEdit_Recursive(World, Current->Children[6], Edit);
      SanityCheckOctreeDoesNotContainEdit_Recursive(World, Current->Children[7], Edit);
    } break;
  }
}
#endif

link_internal void
DropEditFromOctree(engine_resources *Engine, world_edit *Edit, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert( BitfieldNotSet(Edit->Flags, WorldEditFlag_Tombstone) );

  octree_node_ptr_block_array Nodes = OctreeNodePtrBlockArray(TempMemory);

  rect3cp QueryRegion = Edit->Region;


  // NOTE(Jesse):
#if 0
  {
    QueryRegion.Min.Offset -= 1.f;
    QueryRegion.Max.Offset += 1.f;

    Canonicalize(World, &QueryRegion.Min);
    Canonicalize(World, &QueryRegion.Max);
  }
#endif

  GatherOctreeNodesOverlapping_Recursive(World, &World->Root, &QueryRegion, &Nodes);


  IterateOver(&Nodes, Node, NodeIndex)
  {
    AcquireFutex(&Node->Lock);
    auto Index = Find(&Node->Edits, Edit);
    Assert(IsValid(&Index)); // There shouldn't be a node that doesn't contain the edit
    RemoveUnordered(&Node->Edits, Index);

    // Need to reinitialize chunks that no longer have the edit so that it
    // doesn't stay intact in chunks that lose it entirely
    Node->Dirty = True;
    ReleaseFutex(&Node->Lock);
  }

#if BONSAI_INTERNAL
  SanityCheckOctreeDoesNotContainEdit_Recursive(World, &World->Root, Edit);
#endif
}

link_internal void
UpdateWorldEditBounds(engine_resources *Engine, world_edit *Edit, rect3cp Region, memory_arena *TempMemory)
{
  Assert( BitfieldNotSet(Edit->Flags, WorldEditFlag_Tombstone) );

  BUG("Need to lock outside these functions!");
  DropEditFromOctree(Engine, Edit, TempMemory);
  Edit->Region = Region;
  ApplyEditToOctree(Engine, Edit, TempMemory);
}

link_internal void
ReapplyEditToOctree(engine_resources *Engine, world_edit *Edit, memory_arena *TempMemory)
{
  // TODO(Jesse): Is there a better way to do this, or do we even care?
  /* UpdateWorldEditBounds(Engine, Edit, Edit->Region, TempMemory); */
  MarkOctreeNodesContainingEditDirty(Engine, Edit, TempMemory);
}

link_internal void
ReapplyEditsUsingBrush(engine_resources *Engine, world_edit_brush *Brush)
{
  NotImplemented;
  // TODO(Jesse): Is there a better way to do this, or do we even care?
  /* UpdateWorldEditBounds(Engine, Edit, Edit->Region, TempMemory); */
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
  Assert( BitfieldNotSet(Edit->Flags, WorldEditFlag_Tombstone) );

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
  SetBitfield(u32, Edit->Flags, WorldEditFlag_Tombstone);
}

link_internal world_edit *
GetClosestEditIntersectingRay(world *World, world_edit_block_array *Edits, ray *Ray, world_edit_block_array_index *ResultIndex = 0)
{
  world_edit *Result = {};
  world_edit_block_array_index Index = {INVALID_BLOCK_ARRAY_INDEX};

  r32 tMin = f32_MAX;
  IterateOver(Edits, Edit, EditIndex)
  {
    aabb Box = GetSimSpaceAABB(World, Edit->Region);
    aabb_intersect_result I = Intersect(Box, Ray);
    if (I.t < tMin)
    {
      tMin = I.t;
      Result = Edit;
      Index = EditIndex;
    }
  }

  if (ResultIndex)
    *ResultIndex = Index;

  return Result;
}

link_internal void
DeselectAllEdits(level_editor *Editor)
{
  IterateOver(&Editor->SelectedEditIndices, EI, EII)
  {
    auto SelEdit = GetPtr(&Editor->Edits, *EI);
    SelEdit->Selected = False;
  }
  Editor->SelectedEditIndices.ElementCount = 0;
}

link_internal void
SelectEdit(level_editor *Editor, world_edit *Edit, world_edit_block_array_index EditIndex, b32 MultiSelect = False)
{
  world_edit_block_array_index_block_array *SelectedEditIndices = &Editor->SelectedEditIndices;
  Assert(Edit);
  Assert(EditIndex.Index != INVALID_BLOCK_ARRAY_INDEX);

  if (MultiSelect)
  {
    if (Edit->Selected)
    {
      Assert(Edit->Selected == True);
      Edit->Selected = False;

      auto I = IndexOfValue(SelectedEditIndices, &EditIndex);
      Assert(I.Index != INVALID_BLOCK_ARRAY_INDEX);
      RemoveUnordered(SelectedEditIndices, I);
    }
    else
    {
      Assert(Edit->Selected == False);
      Edit->Selected = True;
      Push(SelectedEditIndices, &EditIndex);
    }
  }
  else
  {
    DeselectAllEdits(Editor);
    Push(SelectedEditIndices, &EditIndex);
    Edit->Selected = True;
  }

  Editor->Selection.Clicks  = 2;
  Editor->Selection.ModMode = SelectionModificationMode_Modify;
}

link_internal void
SpawnPrefabInstance(engine_resources *Engine, prefab *Prefab, cp SpawnPoint)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  world_edit_layer *Layer = NewLayer(Editor);

  cp MinP = { V3(f32_MAX), V3i(s32_MAX) };
  cp MaxP = { V3(f32_MIN), V3i(s32_MIN) };
  {
    IterateOver(&Prefab->Edits, PrefabEdit, StoredEditIndex)
    {
      MinP = Min(MinP, PrefabEdit->Region.Min);
      MaxP = Max(MaxP, PrefabEdit->Region.Max);
    }
  }

  IterateOver(&Prefab->Edits, PrefabEdit, StoredEditIndex)
  {
    world_edit_brush *B = Upsert(*PrefabEdit->Brush, &Editor->LoadedBrushes, Editor->Memory);

    world_edit *FinalEdit = NewEdit(Editor, Layer);

    FinalEdit->Rotation = PrefabEdit->Rotation;
    FinalEdit->Brush = B;

    cp RelativeOffset = PrefabEdit->Region.Min - MinP;
    cp Dim = GetDim_cp(World, PrefabEdit->Region);

    FinalEdit->Region.Min = SpawnPoint + RelativeOffset;
    FinalEdit->Region.Max = SpawnPoint + RelativeOffset + Dim;
    Canonicalize(World, &FinalEdit->Region.Max);

    ApplyEditToOctree(Engine, FinalEdit, GetTranArena());
  }

  DispatchPrefabSpawnCallback(Prefab->SpawnCallback, Prefab, SpawnPoint, RectMinMax(MinP, MaxP));
}

link_internal void
DoWorldEditor(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Editor->HotEdit = 0;
  Editor->HotEditIndex = {INVALID_BLOCK_ARRAY_INDEX};

  if (Editor->CurrentBrush == 0)
  {
    world_edit_brush ThisBrush = NewBrush();
    Editor->CurrentBrush = Insert(ThisBrush, &Editor->LoadedBrushes, Editor->Memory);
  }

  aabb_intersect_result AABBTest = EditWorldSelection(Engine);

  if (Engine->MaybeMouseRay.Tag == Maybe_Yes)
  {
    ray *Ray = &Engine->MaybeMouseRay.Ray;
    Editor->HotEdit = GetClosestEditIntersectingRay(World, &Editor->Edits, Ray, &Editor->HotEditIndex);
  }



  //
  //
  // Hotkeys
  //
  //


  if (Input->Shift.Pressed || Input->Alt.Pressed) { Ui->RequestedForceCapture = True; }

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
    world_edit_layer *Layer = TryGetSelectedLayer(Editor);
    if (Layer == 0)  { Layer = NewLayer(Editor); }

    world_edit_block_array_index Index = {};
    auto E = NewEdit(Editor, Layer, &Index);
    E->Brush = Editor->CurrentBrush;

    SelectEdit(Editor, E, Index);

    // NOTE(Jesse): Must come after SelectEdit because it sets the ModMode to Modify
    ResetSelection(Editor);
    Editor->Selection.ModMode = SelectionModificationMode_Initialize;
  }


  {
    window_layout *Window = GetOrCreateWindow(Ui, "Tools");
    PushWindowStart(Ui, Window);
      PushToolbar(Ui, Window, {}, &Editor->SelectedTool);
    PushWindowEnd(Ui, Window);
  }


  // All Brushes Window
  //
  switch (Editor->SelectedTool)
  {
    case UiEditorTool_Prefab:
    {
      {
        window_layout *Window = GetOrCreateWindow(Ui, "All Prefabs", WindowLayoutFlag_Align_BottomRight | WindowLayoutFlag_Default);
        PushWindowStart(Ui, Window);

        IterateOver(&Editor->Prefabs, Prefab, PrefabIndex)
        {
          if (Prefab)
          {
            b32 Selected = Editor->SelectedPrefab == Prefab;
            if (Button(Ui, Prefab->Name, UiId(Window, "prefab select", Prefab), Selected))
            {
              Editor->SelectedPrefab = Prefab;
            }
            PushNewRow(Ui);
          }
        }

        PushWindowEnd(Ui, Window);
      }

      {
        window_layout *Window = GetOrCreateWindow(Ui, "Prefab", WindowLayoutFlag_Align_Right | WindowLayoutFlag_Default);
        PushWindowStart(Ui, Window);
        if (Editor->SelectedPrefab)
        {
          u32 IconBits =
            (1 << UiEditorAction_Save);

          ui_id BaseId = UiId(Window, Editor->SelectedPrefab, 0, 0);
          ui_action_result BrushActionResult = DoEditorActionsButtons(Ui, Window, BaseId, IconBits);
          switch (BrushActionResult.Action)
          {
            case UiEditorAction_NoAction: {} break;

            case UiEditorAction_Save:
            {
              auto P = Editor->SelectedPrefab;
              auto Blocks = BeginSerialization();
              Serialize(&Blocks, P);
              FinalizeSerialization(&Blocks, Concat(CSz("prefabs/"), P->Name, CSz(".prefab"), GetTranArena(), 1).Start);
            } break;

            InvalidDefaultCase;
          }
          PushNewRow(Ui);

          DoEditorUi(Ui, Window, Editor->SelectedPrefab, Editor->SelectedPrefab->Name, 0);
        }
        PushWindowEnd(Ui, Window);
      }

      if ( Editor->SelectedPrefab &&
           Engine->MousedOverVoxel.Tag && 
           Input->LMB.Clicked )
      {
        SpawnPrefabInstance(Engine, Editor->SelectedPrefab, Engine->MousedOverVoxel.Value.Picks[PickedVoxel_LastEmpty]);
      }
    } break;

    case UiEditorTool_Brush:
    {
      window_layout *AllBrushesWindow = GetOrCreateWindow(Ui, "All Brushes", WindowLayoutFlag_Align_BottomRight | WindowLayoutFlag_Default);
      PushWindowStart(Ui, AllBrushesWindow);

      IterateOver(&Editor->LoadedBrushes, Brush, BrushIndex)
      {
        if (Brush)
        {
          ui_style *Style = &DefaultStyle;
          if (Brush == Editor->CurrentBrush)
          {
            Style = &DefaultSelectedStyle;
          }

          if (Button(Ui, CS(Brush->NameBuf), UiId(AllBrushesWindow, "brush select", Brush), Style))
          {
            Editor->CurrentBrush = Brush;
            CheckSettingsChanged(Brush); // Prevent firing a change event @prevent_change_event
          }
          PushNewRow(Ui);
        }
      }
      PushWindowEnd(Ui, AllBrushesWindow);

      window_layout *BrushSettingsWindow = GetOrCreateWindow(Ui, "Brush Details", WindowLayoutFlag_Align_Right | WindowLayoutFlag_Default);
      /* DoEditorUi(Ui, BrushSettingsWindow, Editor->CurrentBrush, {}, 0); */
      DoBrushDetailsWindow(Engine, Editor->CurrentBrush, BrushSettingsWindow);

      // NOTE(Jesse): Must come after the settings window draws because the
      // settings window detects and initializes new brushes
      if (SelectionComplete(Editor->Selection.Clicks) && Editor->CurrentBrush)
      {
        if (Editor->Selection.InitialSelect)
        {
          Info("Setting Initial edit state");
          Assert(AtElements(&Editor->SelectedEditIndices).Index == 1);
          Assert(LengthSq(Editor->Selection.Diff) == 0.f);

          auto EditIndex = GetPtr(&Editor->SelectedEditIndices, {});
          world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
          UpdateWorldEditBounds(Engine, Edit, Editor->Selection.Region, GetTranArena());
        }

        if (LengthSq(Editor->Selection.Diff) > 0.f)
        {
          Info("Applying diff to edit buffer");
          world_edit_selection_mode SelectionMode = ComputeSelectionMode(Input);
          Assert(SelectionMode);
          ApplyDiffToEditBuffer(Engine, Editor->Selection.Diff, &Editor->SelectedEditIndices, SelectionMode);
          Editor->Selection.ModState.ClickedFace = FaceIndex_None;
        }

      }
    } break;
  }

  {
    b32 CurrentBrushSettingsChanged  = Editor->CurrentBrush && CheckSettingsChanged(Editor->CurrentBrush);
    IterateOver(&Editor->Edits, Edit, EditIndex)
    {
      if (BitfieldIsSet(Edit->Flags, WorldEditFlag_Tombstone)) { continue; }

      if ( Edit->Dirty ||
           (Edit->Brush == Editor->CurrentBrush && 
            CurrentBrushSettingsChanged) )
      {
        Edit->Dirty = False;
        ReapplyEditToOctree(Engine, Edit, GetTranArena());
      }
    }
  }

  {
    window_layout *LayersWindow = GetOrCreateWindow(Ui, "Layers", WindowLayoutFlag_Default|WindowLayoutFlag_Align_Bottom);
    PushWindowStart(Ui, LayersWindow);

    if (Button(Ui, CSz("New Layer"), UiId(LayersWindow, "new layer", 0ull)))
    {
      NewLayer(Editor);
    }

    PushNewRow(Ui);
    PushNewRow(Ui);

    PushTableStart(Ui);

    world_edit_layer *PrevLayer = {};


    s32 LayerEditIndex = -1;
    ui_editor_action ReorderAction = {};
    ui_editor_action DeleteAction = {};
    IterateOver(&Editor->Layers, Layer, LayerIndex)
    {
      cs Name = CS(Layer->NameBuf);
      b32 LayerSelected = Editor->SelectedLayerIndex == LayerIndex;

      PushTableStart(Ui);
      {

        u32 VisibilityAction = (Layer->Flags & WorldEditLayerFlag_Hidden) ? (1<<UiEditorAction_Show): (1<<UiEditorAction_Hide) ;
        u32 ActionBits =
          (VisibilityAction)                |
          (1 << UiEditorAction_ReorderUp)   |
          (1 << UiEditorAction_ReorderDown) |
          (1 << UiEditorAction_Duplicate)   |
          (1 << UiEditorAction_ExportAsPrefab);


        ui_id BaseId = UiId(LayersWindow, Layer, 0, 0);
        ui_action_result LayerAction = DoEditorActionsButtons(Ui, LayersWindow, BaseId, ActionBits);

        s32 IconIndex = (Layer->Flags & WorldEditLayerFlag_Collapsed) ? UiIconIndex_AngleRight : UiIconIndex_AngleDown;
        if (Button(Ui, &Ui->IconTextureArray, IconIndex, UiId(LayersWindow, "layer expand/contract", Layer, 0)))
        {
          ToggleBitfieldValue(Layer->Flags, WorldEditLayerFlag_Collapsed);
        }

        ui_id NameButtonId = UiId(LayersWindow, Layer, Layer);

        TextBox(Ui, {}, CS(Layer->NameBuf), NameBuf_Len, NameButtonId);

        ui_style *Style = LayerSelected ? &DefaultSelectedStyle : &DefaultStyle;
        if (LClicked(Ui, &NameButtonId))
        {
          Editor->SelectedLayerIndex = LayerIndex;

          // NOTE(Jesse): Clear the list if we didn't have ctrl pressed
          b32 MultiSelect = Input->Ctrl.Pressed;
          IterateOver(&Layer->EditIndices, EditIndex, EII)
          {
            auto Edit = GetPtr(&Editor->Edits, *EditIndex);
            SelectEdit(Editor, Edit, *EditIndex, MultiSelect);
            MultiSelect = True;
          }
        }

        {
          switch (LayerAction.Action)
          {
            case UiEditorAction_NoAction:
            {} break;

            case UiEditorAction_ReorderUp:
            case UiEditorAction_ReorderDown:
            {
              ReorderAction = LayerAction.Action;
              LayerEditIndex = s32(LayerIndex.Index);
            } break;

            case UiEditorAction_Show:
            case UiEditorAction_Hide:
            {
              ToggleBitfieldValue(Layer->Flags, WorldEditLayerFlag_Hidden);
              if (Layer->Flags & WorldEditLayerFlag_Hidden)
              {
                IterateOver(&Layer->EditIndices, EditIndex, EII)
                {
                  auto Edit = GetPtr(&Editor->Edits, *EditIndex);
                  DropEditFromOctree(Engine, Edit, GetTranArena());
                }
              }
              else
              {
                IterateOver(&Layer->EditIndices, EditIndex, EII)
                {
                  auto Edit = GetPtr(&Editor->Edits, *EditIndex);
                  ApplyEditToOctree(Engine, Edit, GetTranArena());
                }
              }
            } break;

            case UiEditorAction_Expand:
            {
            } break;

            case UiEditorAction_ExportAsPrefab:
            {
              prefab P = {};

              // NOTE(Jesse): If the Upsert overwrites an existing prefab the previous
              // Edits get leaked.  Not that this particularly matters .. but .. it would
              // be kinda nice if that didn't happen.
              Leak("Leaking prefab memory");

              P.Name = CS(Layer->NameBuf);
              P.Edits.Memory = Editor->Memory;

              IterateOver(&Layer->EditIndices, EditIndex, EII)
              {
                world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
                Push(&P.Edits, Edit);
              }

              Upsert(P, &Editor->Prefabs, Editor->Memory);

              auto Blocks = BeginSerialization();
              Serialize(&Blocks, &P);
              FinalizeSerialization(&Blocks, Concat(CSz("prefabs/"), P.Name, CSz(".prefab"), GetTranArena(), 1).Start);

            } break;

            case UiEditorAction_Duplicate:
            {
              auto SrcLayer = Layer;
              auto DstLayer = NewLayer(Editor);

              b32 MultiSelect = Input->Ctrl.Pressed;
              IterateOver(&SrcLayer->EditIndices, EditIndex, EditIndexIndex)
              {
                world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
                world_edit_block_array_index DupIndex = {};
                auto *Duplicated = DuplicateEdit(Editor, DstLayer, Edit, &DupIndex);

                SelectEdit(Editor, Duplicated, DupIndex, MultiSelect);

                ApplyEditToOctree(Engine, Duplicated, GetTranArena());
                MultiSelect = True;
              }
            } break;

            InvalidDefaultCase;
          }
        }


        if (Button(Ui, &Ui->IconTextureArray, UiIconIndex_Trash, UiId(LayersWindow, "layer delete", Layer, 0)))
        {
          DeleteAction = UiEditorAction_Delete;
          LayerEditIndex = s32(LayerIndex.Index);
        }
        PushNewRow(Ui);
      }
      PushTableEnd(Ui);

      if ( Layer &&
          (Layer->Flags & WorldEditLayerFlag_Collapsed) == 0)
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
            b32 LayerIsHidden = (Layer->Flags & WorldEditLayerFlag_Hidden) != 0 ;

            world_edit_block_array_index *PrevEditIndex = {};

            v3 EditInstanceTint = LayerIsHidden ? V3(0.5f) : V3(0);
            IterateOver(&Layer->EditIndices, EditIndex, EditIndexIndex)
            {
              world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
              Assert( BitfieldNotSet(Edit->Flags, WorldEditFlag_Tombstone) );

              const char *NameBuf = Edit->Brush ? Edit->Brush->NameBuf : "no brush";

              u32 VisibilityAction = u32_MAX;
              if (LayerIsHidden)
              {
                VisibilityAction = (Edit->Flags & WorldEditFlag_Hidden) ? (1<<UiEditorAction_Show): (1<<UiEditorAction_HideObstructed) ;
              }
              else
              {
                VisibilityAction = (Edit->Flags & WorldEditFlag_Hidden) ? (1<<UiEditorAction_Show): (1<<UiEditorAction_Hide) ;
              }

              Assert(VisibilityAction != u32_MAX);

              u32 IconBits =
                VisibilityAction                  |
                (1 << UiEditorAction_ReorderUp)   |
                (1 << UiEditorAction_ReorderDown) |
                (1 << UiEditorAction_Duplicate)   |
                (1 << UiEditorAction_SetBrush);

              ui_id BaseId = UiId(LayersWindow, Edit, 0, 0);
              ui_action_result EditReorderAction = DoEditorActionsButtons(Ui, LayersWindow, BaseId, IconBits, EditInstanceTint);

              switch(EditReorderAction.Action)
              {
                InvalidDefaultCase;

                case UiEditorAction_NoAction: {} break;

                case UiEditorAction_Show:
                case UiEditorAction_HideObstructed:
                case UiEditorAction_Hide:
                {
                  ToggleBitfieldValue(Edit->Flags, WorldEditFlag_Hidden);
                } break;

                case UiEditorAction_SetBrush:
                {
                  Assert(Editor->CurrentBrush);
                  Edit->Brush = Editor->CurrentBrush;
                  ReapplyEditToOctree(Engine, Edit, GetTranArena());
                } break;

                case UiEditorAction_Duplicate:
                {
                  world_edit_block_array_index DupIndex = {};
                  auto *Duplicated = DuplicateEdit(Editor, Layer, Edit, &DupIndex);

                  b32 MultiSelect = Input->Ctrl.Pressed;
                  SelectEdit(Editor, Duplicated, DupIndex, MultiSelect);

                  ApplyEditToOctree(Engine, Duplicated, GetTranArena());
                } break;

                case UiEditorAction_ReorderUp:
                {
                  if (PrevEditIndex)
                  {
                    world_edit *PrevEdit = GetPtr(&Editor->Edits, *PrevEditIndex);
                    Assert(Edit != PrevEdit);
                    ReapplyEditToOctree(Engine, Edit, GetTranArena());
                    ReapplyEditToOctree(Engine, PrevEdit, GetTranArena());

                    auto Tmp = *PrevEditIndex;
                    *PrevEditIndex = *EditIndex;
                    *EditIndex = Tmp;
                  }
                  else if (PrevLayer)
                  {
                    Push(&PrevLayer->EditIndices, EditIndex);
                    RemoveOrdered(&Layer->EditIndices, EditIndex);

                    // NOTE(Jesse): No need to reapply because we didn't change
                    // the ordering, just shifted up a layer
                    /* ReapplyEditToOctree(Engine, Edit, GetTranArena()); */
                  }

                } break;

                case UiEditorAction_ReorderDown:
                {
                  auto NextEditIndexIndex = EditIndexIndex;
                     ++NextEditIndexIndex;

                  if (auto NextEditIndex = TryGetPtr(&Layer->EditIndices, NextEditIndexIndex))
                  {
                    world_edit *NextEdit = GetPtr(&Editor->Edits, *NextEditIndex);
                    Assert(Edit != NextEdit);
                    ReapplyEditToOctree(Engine, Edit, GetTranArena());
                    ReapplyEditToOctree(Engine, NextEdit, GetTranArena());

                    auto Tmp = *NextEditIndex;
                    *NextEditIndex = *EditIndex;
                    *EditIndex = Tmp;

                    EditIndexIndex = AtElements(&Layer->EditIndices);
                  }
                  else
                  {
                    auto NextLayerIndex = LayerIndex;
                       ++NextLayerIndex;

                    if (auto NextLayer = TryGetPtr(&Editor->Layers, GetIndex(&NextLayerIndex)))
                    {
                      RemoveOrdered(&Layer->EditIndices, EditIndex);
                      Shift(&NextLayer->EditIndices, EditIndex);

                    // NOTE(Jesse): No need to reapply because we didn't change
                    // the ordering, just shifted up a layer
                    /* ReapplyEditToOctree(Engine, Edit, GetTranArena()); */
                    }

                    EditIndexIndex = AtElements(&Layer->EditIndices);
                  }
                }

                break;
              }


              ui_render_params ButtonParams = DefaultUiRenderParams_Button;

              b32 EditIsSelected = Edit->Selected;
              if (EditIsSelected)
              {
                ButtonParams.FStyle = &DefaultSelectedStyle;
              }

              if (LayerIsHidden)
              {
                auto S = UiStyleFromLightestColor(EditInstanceTint);
                ButtonParams.FStyle = &S;
              }

              // One make up for the expand button
              PushBlankIconButton(Ui);

              auto EditSelectButton = PushSimpleButton(Ui, FSz("(%s)", NameBuf), UiId(LayersWindow, "edit select", Edit), &ButtonParams);
              if (Clicked(Ui, &EditSelectButton))
              {
                // NOTE(Jesse): We do SelectEdit on the HotEdit later
                /* SelectEdit(Editor, Edit, *EditIndex, Input->Ctrl.Pressed); */

                Editor->SelectedLayerIndex = LayerIndex;
                if (Edit->Brush)
                {
                  Editor->CurrentBrush = Edit->Brush;
                  CheckSettingsChanged(Edit->Brush); // Prevent firing a change event @prevent_change_event
                }
              }

              if (Hover(Ui, &EditSelectButton))
              {
                Editor->HotEdit = Edit;
                Editor->HotEditIndex = *EditIndex;
              }


              auto ButtonId = UiId(LayersWindow, "edit delete", Edit, 0);
              if (Button(Ui, &Ui->IconTextureArray, UiIconIndex_Trash, ButtonId))
              {
                DeleteEdit(Engine, Edit, EditIndex, Layer);
              }
              PushNewRow(Ui);

              PrevEditIndex = EditIndex;
            }
          }
        }
        PushTableEnd(Ui);

        PrevLayer = Layer;
      }

      PushNewRow(Ui);
    }

    if (ReorderAction)
    {
      Assert( ReorderAction == UiEditorAction_ReorderUp  ||
              ReorderAction == UiEditorAction_ReorderDown );
      Assert(LayerEditIndex > -1);

      s32 NextLayerIndex = LayerEditIndex;

      if (ReorderAction == UiEditorAction_ReorderUp)
      {
        NextLayerIndex = Max(0, NextLayerIndex -1);
      }

      if (ReorderAction == UiEditorAction_ReorderDown)
      {
        NextLayerIndex = Min(s32(AtElements(&Editor->Layers).Index-1), s32(NextLayerIndex +1));
      }

      if (NextLayerIndex != LayerEditIndex)
      {
        /* Swap(&Editor->Layers, NextLayerIndex, LayerEditIndex); */

        auto P0 = GetPtr(&Editor->Layers, umm(NextLayerIndex));
        auto P1 = GetPtr(&Editor->Layers, umm(LayerEditIndex));

        auto Tmp = *P0;
        *P0 = *P1;
        *P1 = Tmp;
      }
    }

    if (DeleteAction)
    {
      auto Layer = GetPtr(&Editor->Layers, umm(LayerEditIndex));
      IterateOver(&Layer->EditIndices, EditIndex, EditIndexIndex)
      {
        world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
        // NOTE(Jesse): We're deleting the layer here so we don't need
        // to remove the edits one-by-one .. we'll clear the whole thing.
        DeleteEdit(Engine, Edit, EditIndex);
      }

      Layer->EditIndices.ElementCount = 0;

      RemoveOrdered(&Editor->Layers, Layer);
      Editor->SelectedLayerIndex = {INVALID_BLOCK_ARRAY_INDEX};
      LayerIndex = AtElements(&Editor->Layers);
    }

    PushNewRow(Ui);
    PushTableEnd(Ui);
    PushWindowEnd(Ui, LayersWindow);
  }



  //
  //
  // Draw
  //
  //

  {
    v3 CameraSimP = GetSimSpaceP(World, Camera->CurrentP);

    auto Face = Editor->Selection.ModState.ClickedFace;
    if (Face == FaceIndex_None) { Face = AABBTest.Face; }

    f32 HiThicknessMod           = 1.5f;
    f32 SelectionThicknessMod    = 3.0f;


    b32 ShiftOrAltPressed = Input->Shift.Pressed | Input->Alt.Pressed;

    // Selection region
    //
    if (Editor->Selection.ModMode && Editor->Selection.Clicks > 0)
    {
      aabb FinalSelectionAABB = GetSimSpaceRect(World, Editor->Selection.Region);
      r32 BaseThicc = GetSelectionThicknessForDistance( Distance(CameraSimP, GetCenter(&FinalSelectionAABB)) );

      if (Face && ShiftOrAltPressed)
      {
        /* r32 InsetWidth = 0.25f; */
        r32 InsetWidth  = 0.f;
        v3  HiColor     = RGB_RED;

        HighlightFace(Engine, Face, FinalSelectionAABB, InsetWidth, HiColor, BaseThicc*SelectionThicknessMod*HiThicknessMod);
      }

      DEBUG_DrawSimSpaceAABB(Engine, &FinalSelectionAABB, RGB_GREEN, BaseThicc*SelectionThicknessMod);
    }

    // Highlight moused over voxel
    //
    if (Engine->MousedOverVoxel.Tag)
    {
      v3 HotVoxel = GetHotVoxelForEditMode(Engine, WorldEdit_Mode_Subtractive );
      DEBUG_HighlightVoxel( Engine, HotVoxel, RGB_RED, 0.075f);
    }

    // Highlight edits in the current layer
    //
    {
      if ( world_edit_layer *Layer = TryGetSelectedLayer(Editor) )
      {
        IterateOver(&Layer->EditIndices, EditIndex, EditIndexIndex)
        {
          world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
          Assert( BitfieldNotSet(Edit->Flags, WorldEditFlag_Tombstone) );

          if (Edit == Editor->HotEdit) continue;
          if (Edit->Selected)          continue;

          auto EditAABB = GetSimSpaceAABB(World, Edit->Region);
          random_series S = {u64(Edit)};
          v3 BaseColor = RandomV3Unilateral(&S);

          r32 BaseThicc = GetSelectionThicknessForDistance( Distance(CameraSimP, GetCenter(&EditAABB)) );
          DEBUG_DrawSimSpaceAABB(Engine, &EditAABB, BaseColor, BaseThicc);
        }
      }
    }

    // Highlight currently selected edits
    //
    {
      IterateOver(&Editor->SelectedEditIndices, EditIndex, EditIndexIndex)
      {
        world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
        Assert( BitfieldNotSet(Edit->Flags, WorldEditFlag_Tombstone) );


        auto EditAABB = GetSimSpaceAABB(World, Edit->Region);
        random_series S = {u64(Edit)};
        v3 BaseColor = RandomV3Unilateral(&S);

        r32 BaseThicc = GetSelectionThicknessForDistance( Distance(CameraSimP, GetCenter(&EditAABB)) );
        if (Face && ShiftOrAltPressed)
        {
          /* r32 InsetWidth = 0.25f; */
          r32 InsetWidth  = 0.f;
          v3  HiColor     = RGB_RED;

          HighlightFace(Engine, Face, EditAABB, InsetWidth, HiColor, BaseThicc*HiThicknessMod);
        }

        // NOTE(Jesse): This is intentionally after the HighlightFace call; we
        // always wanna highlight that no matter if we're hovering or not
        if (Edit == Editor->HotEdit) continue;

        DEBUG_DrawSimSpaceAABB(Engine, &EditAABB, RGB_YELLOW, BaseThicc);
      }
    }

    // Highlight the hot edit
    //
    if (Editor->HotEdit)
    {
      if ( Input->Shift.Pressed  == False &&
           Input->Alt.Pressed    == False &&
           Editor->Selection.ModMode != SelectionModificationMode_Initialize )
      {
        if (Input->LMB.Clicked)
        {
          b32 MultiSelect = Input->Ctrl.Pressed;
          SelectEdit(Editor, Editor->HotEdit, Editor->HotEditIndex, MultiSelect);
        }
      }

      auto EditAABB = GetSimSpaceAABB(World, Editor->HotEdit->Region);
      r32 BaseThicc = GetSelectionThicknessForDistance( Distance(CameraSimP, GetCenter(&EditAABB)) );
      DEBUG_DrawSimSpaceAABB(Engine, &EditAABB, RGB_PINK, BaseThicc);
    }

  }


#if BONSAI_INTERNAL
  // NOTE(Jesse): Sanity check we keep the selected flag in sync with the array
  IterateOver(&Editor->Edits, Edit, EI)
  {
    auto SelectedEditIndex = IndexOfValue(&Editor->SelectedEditIndices, &EI);
    if (SelectedEditIndex.Index == INVALID_BLOCK_ARRAY_INDEX)
    {
      Assert(Edit->Selected == False);
    }
    else
    {
      Assert(Edit->Selected == True);
    }
  }
#endif


  u32 Ordinal = 0;
  IterateOver(&Editor->Layers, Layer, LayerIndex)
  {
    IterateOver(&Layer->EditIndices, EditIndex, EII)
    {
      world_edit *Edit = GetPtr(&Editor->Edits, *EditIndex);
      Edit->Ordinal = Ordinal++;
    }
  }

}

link_internal void
DrawLod(engine_resources *Engine, shader *Shader, gpu_mapped_element_buffer *Mesh, r32 DistanceSquared, v3 Basis, Quaternion Rotation, v3 Scale );

link_internal void
ApplyEditBufferToOctree(engine_resources *Engine, world_edit_paged_list *Edits)
{
  IterateOver(Edits, Edit, EditIndex)
  {
    if (BitfieldIsSet(Edit->Flags, WorldEditFlag_Tombstone)) { continue; }
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

      Header.WorldCenter        =  World->Center;
      Header.VisibleRegionSize  =  World->VisibleRegionSize;
      Header.Camera             = *Camera;
      Header.RenderSettings     =  Graphics->Settings;
      Header.EntityCount        =  EntityCount;
      Header.SelectedLayerIndex =  u32(Editor->SelectedLayerIndex.Index);
      Header.LayerCount         =  u32(AtElements(&Editor->Layers).Index);
      Header.EditCount          =  u32(AtElements(&Editor->Edits).Index);

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

        FreeOctreeChildren(Engine, &World->Root);
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

        Engine->Editor.SelectedLayerIndex = {LevelHeader.SelectedLayerIndex};


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
          if (BitfieldNotSet(FinalEdit->Flags, WorldEditFlag_Tombstone))
          {
            if (FinalEdit->Brush)
            {
              FinalEdit->Brush = Upsert(*FinalEdit->Brush, &Editor->LoadedBrushes, Editor->Memory);
            }

            if (FinalEdit->Selected)
            {
              FinalEdit->Selected = False; // Minor hack so SelectEdit doesn't assert
              SelectEdit(Editor, FinalEdit, {EditIndex}, True);
              Editor->Selection.ModMode == SelectionModificationMode_Modify;
            }

            ApplyEditToOctree(Engine, FinalEdit, GetTranArena());
          }
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
        if (Engine->Stdlib.AppApi.OnLibraryLoad) { Engine->Stdlib.AppApi.OnLibraryLoad(Engine, GetThreadLocalState(ThreadLocal_ThreadIndex)); }

        UnsignalFutex(&Plat->WorkerThreadsSuspendFutex);
      }
    }
    FinalizeDeserialization(&LevelBytes);

  }
  PushWindowEnd(Ui, &Window);
}

link_internal void
DoBrushTypePicker(renderer_2d *Ui, window_layout *Window, layer_settings *Element, umm ParentHash)
{
  PushColumn(Ui, CSz("Type"));

  ui_id ActivateId = UiId(Window, "brush type select toggle", Element, 0);
  cs LayerName = GetBrushLayerUiText(Element, GetTranArena());
  Button(Ui, LayerName, ActivateId);
  PushNewRow(Ui);

  b32 Active = (Ui->Active.Id == ActivateId);
  if (Active)
  {
    poof(
      func (brush_noise_type enum_t) @code_fragment
      {
        enum_t.map(enum_v)
        {
          {
            PushColumn(Ui, CSz("|"));
            ui_id ButtonId = UiId(Window, "brush type select enum_v.name", Element, 0);
            if (Button(Ui, CSz("Noise((enum_v.name.strip_all_prefix))"), ButtonId))
            {
              Element->Type = BrushLayerType_Noise;
              Element->Noise.Type = enum_v.name;
            }
            PushNewRow(Ui);
          }
        }
      }
    )
#include <generated/anonymous_7E9dQwND.h>

    poof(
      func (brush_shape_type enum_t) @code_fragment
      {
        enum_t.map(enum_v)
        {
          {
            PushColumn(Ui, CSz("|"));
            ui_id ButtonId = UiId(Window, "brush type select enum_v.name", Element, 0);
            if (Button(Ui, CSz("Shape((enum_v.name.strip_all_prefix))"), ButtonId))
            {
              Element->Type = BrushLayerType_Shape;
              Element->Shape.Type = enum_v.name;
            }
            PushNewRow(Ui);
          }
        }
      }
    )
#include <generated/anonymous_PH0rbBE5.h>

    {
      ui_id ButtonId = UiId(Window, "brush type select brush", Element, 0);
      PushColumn(Ui, CSz("|"));
      if (Button(Ui, CSz("Brush"), ButtonId))
      {
        Element->Type = BrushLayerType_Brush;
      }
      PushNewRow(Ui);
    }

    auto Input = Ui->Input;
    if (Input->LMB.Clicked || Input->RMB.Clicked || Input->MMB.Clicked || Input->Enter.Clicked)
    {
      Ui->Active = {};
    }
  }

  if (Clicked(Ui, ActivateId))
  {
    Ui->Active.Id = ActivateId;
  }
}

link_internal void
DrawBrushButtons(renderer_2d *Ui, window_layout *Window, layer_settings *Element)
{
  auto Editor = GetEditor();
  IterateOver(&Editor->LoadedBrushes, ThisBrush, BrushIndex)
  {
    if (ThisBrush)
    {
      ui_style *Style = &DefaultStyle;
      if (ThisBrush == Editor->CurrentBrush)
      {
        Style = &DefaultSelectedStyle;
      }

      PushColumn(Ui, CSz("|"));
      if (Button(Ui, CS(ThisBrush->NameBuf), UiId(Window, "brush select 2", ThisBrush), Style))
      {
        Element->Brush = ThisBrush;
      }
      PushNewRow(Ui);

    }
  }
}

link_internal void
DoBrushBrushPicker(renderer_2d *Ui, window_layout *Window, layer_settings *Element, umm ParentHash)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(u32(ParentHash));

  if (Element->Brush)
  {
    PushColumn(Ui, CSz("Brush"));

    ui_id ActivateId = UiId(Element, Element->Brush, Cast(void*,"brush layer brush pick"), Cast(void*, u64(ThisHash)));
    Button(Ui, CS(Element->Brush->NameBuf), ActivateId);
    PushNewRow(Ui);

    b32 Active = (Ui->Active.Id == ActivateId);
    if (Active)
    {
      DrawBrushButtons(Ui, Window, Element);

      auto Input = Ui->Input;
      if (Input->LMB.Clicked || Input->RMB.Clicked || Input->MMB.Clicked || Input->Enter.Clicked)
      {
        Ui->Active = {};
      }
    }

    if (Clicked(Ui, ActivateId))
    {
      Ui->Active.Id = ActivateId;
    }
  }
  else
  {
    PushColumn(Ui, CSz("(null)"));
    DrawBrushButtons(Ui, Window, Element);
    PushNewRow(Ui);
  }

}

link_internal rtt_framebuffer
GetOrAllocateTextureFramebufferForWorldEdit(rtt_framebuffer_paged_list *Freelist, v2i Dim)
{
  rtt_framebuffer Result = {};
  if (rtt_framebuffer *E = Pop(Freelist))
  {
    Result = *E;
  }
  else
  {
    InitializeRenderToTextureFramebuffer(&Result, Dim, {});
  }
  return Result;
}

link_internal void
BindUniformsForBrush(
    shader *Program,
    brush_layer *Layer,
    rtt_framebuffer *Write,

    b32 SeedNoiseValueFromInput,
    b32 SeedColorValueFromInput,
    rtt_framebuffer *Read,

    b32 SampleBlendTex,
    rtt_framebuffer *Blend,
    world_edit_blend_mode  BlendMode,

    v3 *ChunkRelEditMin,
    v3 *ChunkRelEditMax,

    r32 ColorBlendBias
    )
{
  BindFramebuffer(Write);

  BindUniformByName(Program, "SeedNoiseValueFromInput", SeedNoiseValueFromInput);
  BindUniformByName(Program, "SeedColorValueFromInput", SeedColorValueFromInput);
  if (SeedNoiseValueFromInput || SeedColorValueFromInput)
  {
    texture *InputTex = &Read->DestTexture;
    // @derivs_texture_binding_to_shader_unit_0
    BindUniformByName(Program, "InputTex", InputTex, 1);
  }
  SeedNoiseValueFromInput = True;
  SeedColorValueFromInput = True;

  BindUniformByName(Program, "SampleBlendTex", SampleBlendTex);
  if (SampleBlendTex)
  {
    texture *BlendTex = &Blend->DestTexture;
    BindUniformByName(Program, "BlendTex", BlendTex, 2);

    // NOTE(Jesse): We pass this blend mode in because we want to take the
    // layers blend mode, not the blend mode for the brush.
    BindUniformByName(Program, "BlendTexBlendMode", BlendMode);
  }

  {
    v3 RGBColor = HSVtoRGB(Layer->Settings.HSVColor);
    BindUniformByName(Program, "RGBColor", &RGBColor);
  }

  BindUniformByName(Program, "Normalized",     Layer->Settings.Normalized);
  BindUniformByName(Program, "ValueBias",      Layer->Settings.ValueBias);
  BindUniformByName(Program, "BrushType",      Layer->Settings.Type);
  BindUniformByName(Program, "BlendMode",      Layer->Settings.BlendMode);
  BindUniformByName(Program, "ValueModifiers", Layer->Settings.ValueFunc);
  /* BindUniformByName(Program, "ColorMode",      Layer->Settings.ColorMode); */
  BindUniformByName(Program, "Invert",         Layer->Settings.Invert);
  /* BindUniformByName(Program, "Threshold",      Layer->Settings.Threshold); */
  BindUniformByName(Program, "Power",          Layer->Settings.Power);

  // NOTE(Jesse): Must call bind explicitly because the
  // driver doesn't cache these values otherwise .. it
  // just reads them whenever it wants through the pointer..
  BindUniformByName(Program, "ChunkRelEditMin", ChunkRelEditMin);
  BindUniformByName(Program, "ChunkRelEditMax", ChunkRelEditMax);

  BindUniformByName(Program, "ColorBlendBias", ColorBlendBias);

  AssertNoGlErrors;
}

link_internal rtt_framebuffer
ApplyBrush( world_edit_render_context *WorldEditRC,
                              rect3cp  EditBounds,
                                   v3  ParentRotation,
                     world_edit_brush *Brush,
                world_edit_blend_mode  BlendMode,
                          world_chunk *Chunk,
                      rtt_framebuffer *Read,
                      rtt_framebuffer *Write,
                      rtt_framebuffer *Accum,
                                  b32  SeedNoise,
                                  b32  SeedColor,
                                  r32  ColorBlendBias)
{
  world *World = GetWorld();

  RangeIterator(LayerIndex, Brush->LayerCount)
  {
    b32 IsLastValidLayer = (AnyValidLayersRemaining(Brush, LayerIndex+1) == False);
    b32 SampleBlendTex = IsLastValidLayer;

    brush_layer *Layer = Brush->Layers + LayerIndex;

    if (Layer->Settings.Disabled) continue;

    rect3 SimEditRect = GetSimSpaceRect(World, EditBounds);
       v3 SimChunkMin = GetSimSpaceP(World, Chunk->WorldP);
       v3 EditRectRad = GetRadius(&SimEditRect);

    v3 ChunkRelEditMin = SimEditRect.Min - SimChunkMin;
    v3 ChunkRelEditMax = SimEditRect.Max - SimChunkMin;
    v3 EditDim = ChunkRelEditMax -  ChunkRelEditMin;

    // NOTE(Jesse): Key 0 is the Min, Key 2 is Max
    sort_key_f32 EditDimKeys[3];

    EditDimKeys[0].Index = 0;
    EditDimKeys[0].Value = EditDim.E[0];

    EditDimKeys[1].Index = 1;
    EditDimKeys[1].Value = EditDim.E[1];

    EditDimKeys[2].Index = 2;
    EditDimKeys[2].Value = EditDim.E[2];


    BubbleSort_descending(EditDimKeys, 3);


    BindUniformsForBrush( &WorldEditRC->Program,
                           Layer,
                           Write,

                           SeedNoise,
                           SeedColor,
                           Read,

                           SampleBlendTex,
                           Accum,

                           BlendMode,
                           &ChunkRelEditMin,
                           &ChunkRelEditMax,
                           ColorBlendBias
                         );
    SeedNoise = True;
    SeedColor = True;

    switch (Layer->Settings.Type)
    {
      case BrushLayerType_Brush:
      {
        world_edit_brush *NestedBrush = Layer->Settings.Brush;
        if (NestedBrush)
        {
          /* rtt_framebuffer B0 = {}, B1 = {}; */

          rtt_framebuffer B0 = GetOrAllocateTextureFramebufferForWorldEdit(&WorldEditRC->BrushTextureFramebufferFreelist, Read->DestTexture.Dim);
          rtt_framebuffer B1 = GetOrAllocateTextureFramebufferForWorldEdit(&WorldEditRC->BrushTextureFramebufferFreelist, Read->DestTexture.Dim);

          /* InitializeRenderToTextureFramebuffer(&B0, Read->DestTexture.Dim, CSz("Nested Brush Framebuffer 0")); */
          /* InitializeRenderToTextureFramebuffer(&B1, Read->DestTexture.Dim, CSz("Nested Brush Framebuffer 1")); */

          rtt_framebuffer Applied = ApplyBrush( WorldEditRC,
                                                EditBounds,
                                                ParentRotation,
                                                NestedBrush,
                                                Layer->Settings.BlendMode,
                                                Chunk,
                                                &B0, &B1, Read, False, False, NestedBrush->Smoothing.ColorBlend);

          // NOTE(Jesse): This is kinda gross, but if we're the last layer we have 
          // to apply the result of the brush back into the accumulation texture
          //*
          // There's probably a better way of doing this, but I'm pretty sure
          // this'll work, so I'm just going to put the pig-hat on and go for it.
          //
          if (IsLastValidLayer)
          {
            world_edit_brush BlendBrush = {};
            BlendBrush.BrushBlendMode = WorldEdit_Mode_Disabled;
            BlendBrush.LayerCount = 1;
            BlendBrush.Layers[0].Settings.BlendMode = WorldEdit_Mode_Disabled;
            BlendBrush.Layers[0].Settings.ColorMode = WorldEdit_ColorBlendMode_FinalBlend;


            Applied = ApplyBrush( WorldEditRC,
                                  EditBounds,
                                  ParentRotation,
                                  &BlendBrush,
                                  Brush->BrushBlendMode,
                                  Chunk,
                                  &Applied,
                                  Read, // Intentionally writing into read here because that's what we return
                                  Accum,
                                  True, True, ColorBlendBias);

            Push(&WorldEditRC->BrushTextureFramebufferFreelist, &B0);
            Push(&WorldEditRC->BrushTextureFramebufferFreelist, &B1);
            /* DeallocateRenderToTextureFramebuffer(&B0); */
            /* DeallocateRenderToTextureFramebuffer(&B1); */
          }
          else
          {
            Push(&WorldEditRC->BrushTextureFramebufferFreelist, Read);
            /* DeallocateRenderToTextureFramebuffer(Read); */
            *Read = Applied;

            if (Applied.FBO.ID == B0.FBO.ID)
            {
              Push(&WorldEditRC->BrushTextureFramebufferFreelist, &B1);
              /* DeallocateRenderToTextureFramebuffer(&B1); */
            }
            else
            {
              Assert(Applied.FBO.ID == B1.FBO.ID);
              Push(&WorldEditRC->BrushTextureFramebufferFreelist, &B0);
              /* DeallocateRenderToTextureFramebuffer(&B0); */
            }
          }
        }

        // NOTE(Jesse): It is important we continue here so we do not do the
        // draw call and swap the read/write Framebuffer pointers 
        //
        continue;

      } break;

      case BrushLayerType_Noise:
      {
        noise_layer *Noise = &Layer->Settings.Noise;
        BindUniformByName(&WorldEditRC->Program, "Power", Noise->Power);
        BindUniformByName(&WorldEditRC->Program, "NoiseType", Noise->Type);

        Quaternion Rotation = FromEuler(RadiansFromDegress(ParentRotation)) * FromEuler(RadiansFromDegress(V3(0)));
        m4 RotationMatrix = RotateTransform(Rotation);
        BindUniformByName(&WorldEditRC->Program, "RotTransform", &RotationMatrix);

        switch (Noise->Type)
        {
          case NoiseType_Perlin:
          {
            auto *Perlin = &Noise->Perlin;
            BindUniformByName(&WorldEditRC->Program, "Period",   &Perlin->Period);
          } break;

          case NoiseType_Voronoi:
          {
            auto *Voronoi = &Noise->Voronoi;
            BindUniformByName(&WorldEditRC->Program, "Squareness", Voronoi->Squareness);
            BindUniformByName(&WorldEditRC->Program, "Period",   &Voronoi->Period);
          } break;

          case NoiseType_RectLattice:
          {
            auto *RectLattice = &Noise->RectLattice;
            BindUniformByName(&WorldEditRC->Program, "Squareness", RectLattice->Jitter);
            BindUniformByName(&WorldEditRC->Program, "Period",    &RectLattice->Period);
            BindUniformByName(&WorldEditRC->Program, "Radius",     RectLattice->Radius);
          } break;

          case NoiseType_White:
          {} break;
        }

      } break;

      case BrushLayerType_Shape:
      {
        shape_layer *Shape = &Layer->Settings.Shape;
        BindUniformByName(&WorldEditRC->Program, "ShapeType",  Shape->Type);
        BindUniformByName(&WorldEditRC->Program, "Hollow",     Shape->Advanced.Hollow);
        BindUniformByName(&WorldEditRC->Program, "Rounding",   Shape->Advanced.Rounding);
        BindUniformByName(&WorldEditRC->Program, "Stretch",   &Shape->Advanced.Stretch);
        BindUniformByName(&WorldEditRC->Program, "Repeat",    &Shape->Advanced.Repeat);

        Quaternion Rotation = FromEuler(RadiansFromDegress(ParentRotation)) * FromEuler(RadiansFromDegress(Shape->Advanced.Rotation));
        m4 RotationMatrix = RotateTransform(Rotation);
        BindUniformByName(&WorldEditRC->Program, "RotTransform", &RotationMatrix);

        switch(Shape->Type)
        {
          case ShapeType_Rect:
          {
            auto Rect = &Shape->Rect;

            v3 Dim = Rect->Dim;
            RangeIterator(Index, 3)
            {
              if (Dim.E[Index] == 0)
              {
                Dim.E[Index] = EditDim.E[Index];
              }

              if (Dim.E[Index] < 0)
              {
                Dim.E[Index] = EditDim.E[Index] + Dim.E[Index];
              }
            }
            BindUniformByName(&WorldEditRC->Program, "RectDim", &Dim);
          } break;

          case ShapeType_Sphere:
          {
            auto Sphere = &Shape->Sphere;

            v3 SimSphereOrigin = GetSimSpaceP(World, EditBounds.Min + EditRectRad);
            v3 EditRelativeSphereCenter = SimSphereOrigin - SimEditRect.Min;

            f32 Rad = Sphere->Radius;
            if (Sphere->Radius == 0.f)
            {
              Rad = (MinChannel(EditDim)/2.f);
            }

            if (Sphere->Radius < 0.f)
            {
              Rad = (MinChannel(EditDim)/2.f) + Sphere->Radius;
            }

            BindUniformByName(&WorldEditRC->Program, "EditRelativeSphereCenter", &EditRelativeSphereCenter);
            BindUniformByName(&WorldEditRC->Program, "Radius", Rad);
          } break;

          case ShapeType_Line:
          {
            auto Line = &Shape->Line;
            BindUniformByName(&WorldEditRC->Program, "Radius", Line->Radius);
          } break;

          case ShapeType_Cylinder:
          {
            auto Cylinder = &Shape->Cylinder;

            f32 Radius = Cylinder->Radius;
            f32 Height = Cylinder->Height;

            if (Radius <= 0.f)
            {
              Radius = Radius + (EditDimKeys[0].Value/2.f);
            }

            if (Height <= 0.f)
            {
              // NOTE(Jesse): We divide height by two because the height
              // extends up and down from the middle of the selection.
              Height = (Height/2.f) + (EditDimKeys[2].Value/2.f);
            }

            switch(EditDimKeys[2].Index)
            {
              case 0:
              {
                Quaternion Q2 = Rotation * FromEuler(RadiansFromDegress(V3(0,0,90)));
                m4 Rot2 = RotateTransform(Q2);
                BindUniformByName(&WorldEditRC->Program, "RotTransform", &Rot2);
              } break;

              case 1:
              {
                // Already facing the Y axis
              } break;

              case 2:
              {
                Quaternion Q2 = Rotation * FromEuler(RadiansFromDegress(V3(90,0,0)));
                m4 Rot2 = RotateTransform(Q2);
                BindUniformByName(&WorldEditRC->Program, "RotTransform", &Rot2);
              } break;

            }



            BindUniformByName(&WorldEditRC->Program, "Radius", Radius);
            BindUniformByName(&WorldEditRC->Program, "Height", Height);
          } break;

          case ShapeType_Plane:
          {
            auto Plane = &Shape->Plane;
            auto Orientation = Plane->Orientation;

            v3 xAxis = V3(1,0,0);
            v3 yAxis = V3(0,1,0);
            v3 zAxis = V3(0,0,1);

            switch (Orientation)
            {
              InvalidCase(ShapeAxis_Count);

              case ShapeAxis_InferFromMajorAxis:
              {} break;

              // Traverses the X axis and ascends
              case ShapeAxis_PosX:
              {
                xAxis = Normalize(V3(EditRectRad.x, 0.f, EditRectRad.z));
                yAxis = Normalize(V3(0.f, EditRectRad.y, 0.f));
                zAxis = Normalize(Cross(xAxis, yAxis));
              } break;
              // Traverses the X axis and descends
              case ShapeAxis_NegX:
              {
                xAxis = Normalize(V3(EditRectRad.x, 0.f, -EditRectRad.z));
                yAxis = Normalize(V3(0.f, EditRectRad.y, 0.f));
                zAxis = Normalize(Cross(xAxis, yAxis));
              } break;


              case ShapeAxis_PosY:
              {
                xAxis = Normalize(V3(0.f, EditRectRad.y, EditRectRad.z));
                yAxis = Normalize(V3(EditRectRad.x, 0.f, 0.f));
                zAxis = Normalize(Cross(xAxis, yAxis));
              } break;
              case ShapeAxis_NegY:
              {
                xAxis = Normalize(V3(0.f, EditRectRad.y, -EditRectRad.z));
                yAxis = Normalize(V3(EditRectRad.x, 0.f, 0.f));
                zAxis = Normalize(Cross(xAxis, yAxis));
              } break;


              case ShapeAxis_PosZ:
              {
                xAxis = Normalize(V3(0.f, 0.f, EditRectRad.z));
                yAxis = Normalize(V3(EditRectRad.x, EditRectRad.y, 0.f));
                zAxis = Normalize(Cross(xAxis, yAxis));
              } break;

              case ShapeAxis_NegZ:
              {
                xAxis = Normalize(V3(0.f, 0.f, EditRectRad.z));
                yAxis = Normalize(V3(-EditRectRad.x, EditRectRad.y, 0.f));
                zAxis = Normalize(Cross(xAxis, yAxis));
              } break;
            }

            v3 Plane_SimShapeOrigin = EditRectRad;
            v3 PlaneNormal = zAxis;

            auto PlaneRadius = Plane->Thickness/2.f;
            auto Planed = -1.0f * ( PlaneNormal.x*Plane_SimShapeOrigin.x +
                                    PlaneNormal.y*Plane_SimShapeOrigin.y +
                                    PlaneNormal.z*Plane_SimShapeOrigin.z );

            auto PlanePos = Plane_SimShapeOrigin;

            BindUniformByName(&WorldEditRC->Program, "PlaneNormal", &PlaneNormal);
            BindUniformByName(&WorldEditRC->Program, "Planed",       Planed);
            BindUniformByName(&WorldEditRC->Program, "PlaneRadius",  PlaneRadius);

          } break;

          case ShapeType_Torus:
          {
            auto Torus = &Shape->Torus;

            r32 MinRad = Torus->MinorRadius;
            r32 MajRad = Torus->MajorRadius;

            if (MinRad <= 0.f)
            {
              MinRad += EditDimKeys[0].Value/2.f; 
            }

            if (MajRad <= 0.f)
            {
              // Take the middle dimension for the radius
              MajRad += (EditDimKeys[1].Value/2.f) - MinRad;

              // Orient along the minimum dimension
              switch(EditDimKeys[0].Index)
              {
                case 0:
                {
                  Quaternion Q2 = Rotation * FromEuler(RadiansFromDegress(V3(0,0,90)));
                  m4 Rot2 = RotateTransform(Q2);
                  BindUniformByName(&WorldEditRC->Program, "RotTransform", &Rot2);
                } break;

                case 1:
                {
                  // Already facing the Y axis
                } break;

                case 2:
                {
                  Quaternion Q2 = Rotation * FromEuler(RadiansFromDegress(V3(90,0,0)));
                  m4 Rot2 = RotateTransform(Q2);
                  BindUniformByName(&WorldEditRC->Program, "RotTransform", &Rot2);
                } break;

              }
            }


            BindUniformByName(&WorldEditRC->Program, "Radius",      MajRad);
            BindUniformByName(&WorldEditRC->Program, "MinorRadius", MinRad);

          } break;

          case ShapeType_Pyramid:
          {
            auto Pyramid = &Shape->Pyramid;

            BindUniformByName(&WorldEditRC->Program, "Radius",      Pyramid->Height);

          } break;


          // @sdf_shape_step(5): Calculate values and bind uniform variables for the new shape
          //
        }
      } break;
    }


    /* gpu_timer Timer = StartGpuTimer(); */
    RenderQuad();
    /* EndGpuTimer(&Timer); */
    /* Push(&Graphics->GpuTimers, &Timer); */

#define Swap(a, b) do { auto tmp = b; b = a; a = tmp; } while (false)
    Swap(Read, Write);
#undef Swap
  }

  // This is actually the last-written to texture
  return *Read;
}

