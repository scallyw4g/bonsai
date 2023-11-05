#define MAX_PICKED_WORLD_CHUNKS (64)


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
};


link_internal engine_debug* GetEngineDebug();

// , type_poof_symbol EnumVarName, type_poof_symbol ModName, type_poof_symbol ExtraToggleButtonGroupFlags)
poof(
  func radio_button_group_for_bitfield_enum(enum_t)
  {
    link_internal void
    GetRadioEnum(ui_toggle_button_group *RadioGroup, enum_t.name *Result)
    {
      if (RadioGroup->ToggleBits)
      {
        Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1);
        Assert((((enum_t.map(value).sep(|) {value.name})) & RadioGroup->ToggleBits) != 0);
      }

      *Result = Cast((enum_t.name), RadioGroup->ToggleBits);
    }

    link_internal ui_toggle_button_group
    RadioButtonGroup_(enum_t.name)(renderer_2d *Ui, umm IdModifier, ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None, UI_FUNCTION_PROTO_DEFAULTS)
    {
      ui_toggle_button_handle Buttons[] =
      {
        enum_t.map(value)
        {
          UiToggle(CSz("value.name.strip_all_prefix"), IdModifier),
        }
      };

      ui_toggle_button_group Result = UiToggleButtonGroup(Ui, Buttons, ArrayCount(Buttons),
                                                          ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons), UI_FUNCTION_INSTANCE_NAMES);

      return Result;
    }
  }
)

enum asset_spawn_mode
{
  AssetSpawnMode_BlitIntoWorld = (1 << 0),
  AssetSpawnMode_Entity        = (1 << 1),
};

poof(radio_button_group_for_bitfield_enum(asset_spawn_mode))
#include <generated/radio_button_group_for_bitfield_enum_asset_spawn_mode.h>

