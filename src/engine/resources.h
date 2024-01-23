
struct debug_state;
struct game_state;

struct world;
struct heap_allocator;
struct entity;

typedef entity* entity_ptr;
poof(maybe(entity_ptr))
#include <generated/maybe_entity_ptr.h>



#define ASSET_TABLE_COUNT (256)
CAssert(ASSET_TABLE_COUNT < u16_MAX); // NOTE(Jesse): u16_MAX is max_value(asset_handle::index)

struct engine_resources
{
  bonsai_stdlib Stdlib;

  // TODO(Jesse): Should this go in stdlib?
  renderer_2d   Ui;

  application_api GameApi;

  // Engine
  hotkeys     Hotkeys;
  maybe_ray   MaybeMouseRay;
  world      *World;
  game_state *GameState;
  graphics   *Graphics;

  heap_allocator  Heap;
  memory_arena   *Memory;

  entity **EntityTable;

  // TODO(Jesse): Put on an asset_system struct?
  u64 CurrentUnnamedAssetIndex;
  asset          AssetTable[ASSET_TABLE_COUNT];
  bonsai_futex   AssetFutex;
  heap_allocator AssetMemory;

  u32 FrameIndex; // At 120fps we get 9k hours (385 days) of frames in 32bits

  tiered_mesh_freelist MeshFreelist;

  //
  // NOTE(Jesse): This is kinda-sorta all debug stuff
  //

  debug_state *DebugState;

  engine_debug EngineDebug;

  level_editor Editor;
  maybe_picked_voxel MousedOverVoxel;
  maybe_standing_spot ClosestStandingSpotToCursor;

  maybe_entity_ptr HoverEntity;

  render_entity_to_texture_group RTTGroup;
};

// TODO(Jesse): Should this actually be a thing?
global_variable engine_resources *Global_EngineResources;


link_internal engine_resources *
GetEngineResources()
{
  Assert(Global_EngineResources);
  return Global_EngineResources;
}

link_internal engine_debug *
GetEngineDebug()
{
  Assert(Global_EngineResources);
  return &Global_EngineResources->EngineDebug;
}

link_internal world *
GetWorld()
{
  Assert(Global_EngineResources);
  return Global_EngineResources->World;
}


link_internal level_editor *
GetLevelEditor()
{
  Assert(Global_EngineResources);
  return &Global_EngineResources->Editor;
}

#define UNPACK_ENGINE_RESOURCES(Res) \
  UNPACK_DATA_RESOURCES(Res)         \
  UNPACK_GRAPHICS_RESOURCES(Res)

#define UNPACK_DATA_RESOURCES(Res)                                    \
  platform                  *Plat          = &(Res)->Stdlib.Plat;     \
  world                     *World         =  Res->World;             \
  game_state                *GameState     =  Res->GameState;         \
  heap_allocator            *Heap          = &Res->Heap;              \
  entity                   **EntityTable   =  Res->EntityTable;       \
  hotkeys                   *Hotkeys       = &Res->Hotkeys;           \
  engine_debug              *EngineDebug   = &Res->EngineDebug;       \
  tiered_mesh_freelist      *MeshFreelist  = &Res->MeshFreelist;      \
  input                     *Input         = &Res->Stdlib.Plat.Input; \
  level_editor              *Editor        = &Res->Editor;            \

#define UNPACK_GRAPHICS_RESOURCES(Res)                                    \
  graphics                  *Graphics      =  Res->Graphics;              \
  lighting_render_group     *Lighting      = &Graphics->Lighting;         \
  renderer_2d               *Ui            = &Res->Ui;                    \
  gpu_mapped_element_buffer *GpuMap        =  GetCurrentGpuMap(Graphics); \
  g_buffer_render_group     *gBuffer       =  Graphics->gBuffer;          \
  camera                    *Camera        =  Graphics->Camera;           \

