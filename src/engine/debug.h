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


enum asset_spawn_mode
{
  AssetSpawnMode_BlitIntoWorld,
  AssetSpawnMode_Entity,
};

poof(
  func radio_button_group_for_enum(enum_t, type_poof_symbol GroupName, type_poof_symbol ModName)
  {
    ui_toggle_button_handle (GroupName)Buttons[] =
    {
      enum_t.map(value)
      {
        UiToggle(CSz("value.name.strip_all_prefix"), ModName),
      }
    };

    ui_toggle_button_group (GroupName)Group = UiToggleButtonGroup(Ui, (GroupName)Buttons, ArrayCount((GroupName)Buttons), ToggleButtonGroupFlags_RadioButtons);

    /* Assert(CountBitsSet_Kernighan((GroupName)Group.ToggleBits) <= 1); */

    enum_t.name GroupName = Cast((enum_t.name), (GroupName)Group.ToggleBits);
  }
)
