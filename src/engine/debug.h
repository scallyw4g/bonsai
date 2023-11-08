#define MAX_PICKED_WORLD_CHUNKS (64)

#define EDITOR_UI_FUNCTION_PROTO_DEFAULTS  ui_style *Style = &DefaultStyle, v4 Padding = {{5, 2, 5, 2}}, column_render_params ColumnParams = ColumnRenderParam_LeftAlign
#define EDITOR_UI_FUNCTION_PROTO_ARGUMENTS ui_style *Style,                 v4 Padding,                  column_render_params ColumnParams
#define EDITOR_UI_FUNCTION_INSTANCE_NAMES            Style,                    Padding,                                       ColumnParams


struct world;
struct world_chunk;
struct heap_allocator;
struct entity;


struct picked_world_chunk
{
  world_chunk *Chunk;
  r32 tChunk; // f32_MAX indicates not picked
};

enum picked_voxel_position
{
  PickedVoxel_LastEmpty,
  PickedVoxel_FirstFilled,
  PickedVoxel_Count,
};

struct picked_voxel
{
  picked_world_chunk PickedChunk;
  // Relative to origin of chunk
  v3 Picks[PickedVoxel_Count]; // use picked_voxel_position to index into this
};

struct maybe_picked_voxel
{
  maybe_tag Tag;
  picked_voxel Value;
};

// TODO(Jesse)(metaprogramming, ptr): Once poof can accept pointer types we can generate this struct
/* poof(static_buffer(world_chunk*, 64)) */
/* #include <generated/buffer_world_chunk.h> */
struct picked_world_chunk_static_buffer
{
  picked_world_chunk E[MAX_PICKED_WORLD_CHUNKS];
  u64 At;
};

link_internal void
Push(picked_world_chunk_static_buffer *Buf, world_chunk *Chunk, r32 t)
{
  if (Buf->At < MAX_PICKED_WORLD_CHUNKS)
  {
    Buf->E[Buf->At].Chunk = Chunk;
    Buf->E[Buf->At].tChunk = t;

    ++Buf->At;
  }
}

enum pick_chunk_state
{
  PickedChunkState_None,
  PickedChunkState_Hover,
};

struct engine_debug
{
  picked_world_chunk_static_buffer PickedChunks;
  texture_cursor Textures;

  ui_debug UiDebug;

  b8 DrawEntityCollisionVolumes;
  b8 DrawWorldAxies;

  b8 TriggerRuntimeBreak;
  b8 ResetAssetNodeView;

  u8 PickedChunkState;
  world_chunk *PickedChunk;

  file_traversal_node SelectedAsset;

  entity *SelectedEntity;
};




poof(
  func generic_button_group_for_enum(enum_t, type_poof_symbol NamePrefix, type_poof_symbol extra_poof_flags)
  {
    link_internal b32
    ToggledOn(ui_toggle_button_group *ButtonGroup, enum_t.name Enum)
    {
      b32 Result = ButtonGroup->ToggleBits & (1 << Enum);
      return Result;
    }

    // NOTE(Jesse): This could be implemented by reconstructing the button ID
    // but I'm very unsure that's worth it.  Seems like just
    link_internal b32
    Clicked(ui_toggle_button_group *ButtonGroup, enum_t.name Enum)
    {
      b32 Result = False;
      NotImplemented;
      return Result;
    }

    link_internal ui_toggle_button_group
    (NamePrefix)ButtonGroup_(enum_t.name)(renderer_2d *Ui, umm IdModifier, ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None, UI_FUNCTION_PROTO_DEFAULTS)
    {
      cs ButtonNames[] =
      {
        enum_t.map(value)
        {
          CSz("value.name.strip_all_prefix"),
        }
      };

      u32 ButtonCount = ArrayCount(ButtonNames);

      ui_toggle_button_handle_buffer ButtonBuffer = UiToggleButtonHandleBuffer(ButtonCount, GetTranArena());
      IterateOver(&ButtonBuffer, Button, ButtonIndex)
      {
        *Button = UiToggle(ButtonNames[ButtonIndex], IdModifier+ButtonIndex);
      }

      ui_toggle_button_group Result = UiToggleButtonGroup(Ui, &ButtonBuffer, ui_toggle_button_group_flags(ExtraFlags(extra_poof_flags)), UI_FUNCTION_INSTANCE_NAMES);

      return Result;
    }
  }
)

poof(
  func toggle_button_group_for_enum(enum_t)
  {
    generic_button_group_for_enum(enum_t, {Toggle}, {|ToggleButtonGroupFlags_None})
  }
)

poof(
  func radio_button_group_for_enum(enum_t)
  {
    generic_button_group_for_enum(enum_t, {Radio}, {|ToggleButtonGroupFlags_RadioButtons})
  }
)

poof(
  func radio_button_group_for_bitfield_enum(enum_t)
  {
    link_internal void
    GetRadioEnum(ui_toggle_button_group *RadioGroup, enum_t.name *Result)
    {
      if (RadioGroup->ToggleBits)
      {
        Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1);
        // NOTE(Jesse): This is better; it asserts that we've actually got a bitfield
        Assert((((enum_t.map(value).sep(||) {RadioGroup->ToggleBits == value.name}))));
        /* Assert((((enum_t.map(value).sep(|) {value.name})) & RadioGroup->ToggleBits) != 0); */
      }

      *Result = Cast((enum_t.name), RadioGroup->ToggleBits);
    }

    radio_button_group_for_enum(enum_t)
  }
)

enum asset_spawn_mode
{
  AssetSpawnMode_BlitIntoWorld = (1 << 0),
  AssetSpawnMode_Entity        = (1 << 1),
};

poof(radio_button_group_for_bitfield_enum(asset_spawn_mode))
#include <generated/radio_button_group_for_bitfield_enum_asset_spawn_mode.h>


enum engine_debug_view_mode
{
  EngineDebugViewMode_Level,
  EngineDebugViewMode_WorldEdit,
  EngineDebugViewMode_Entities,
  EngineDebugViewMode_Assets,
  EngineDebugViewMode_WorldChunks,
  EngineDebugViewMode_Textures,
  EngineDebugViewMode_RenderSettings,
  EngineDebugViewMode_EngineDebug,
};

poof(toggle_button_group_for_enum(engine_debug_view_mode))
#include <generated/toggle_button_group_for_enum_engine_debug_view_mode.h>

poof(
  func do_editor_ui_for_compound_type(type)
  {
    link_internal void
    DoEditorUi(renderer_2d *Ui, type.name *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
    {
      /* PushTableStart(Ui); */
      if (ToggleButton(Ui, CS(Name), CS(Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
      {
        PushNewRow(Ui);
        type.map(member)
        {
          DoEditorUi(Ui, &Element->(member.name), "member.type member.name", EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        }
      }
      else
      {
        PushNewRow(Ui);
      }
      /* PushTableEnd(Ui); */
    }
  }
)

poof(
  func do_editor_ui_for_enum(enum_t)
  {
    link_internal void
    DoEditorUi(renderer_2d *Ui, enum_t.name *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
    {
      /* PushTableStart(Ui); */
        PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        /* PushForceAdvance(Ui, V2(150, 0)); */
        /* PushForceUpdateBasis(Ui, V2(150, 0)); */

        cs ElementName = ToString(*Element);
        /* PushTableStart(Ui); */
          if (ToggleButton(Ui, ElementName, ElementName, umm(Element)^umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
          {
            PushNewRow(Ui);
            enum_t.map(value)
            {
              PushColumn(Ui, CSz("")); // Skip the first Name column
              if (Button(Ui, CSz("value.name"), umm(Element)^umm("value.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
              {
                *Element = value.name;
              }
              PushNewRow(Ui);
            }
          }
          else
          {
            PushNewRow(Ui);
          }
        /* PushTableEnd(Ui); */
      /* PushTableEnd(Ui); */
    }
  }
)

link_internal engine_debug * GetEngineDebug();
