
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


enum game_lib_reload_behavior
{
  GameLibReloadBehavior_None,

  GameLibReloadBehavior_FullInitialize = (1 << 0),
};

struct asset_system
{
  // TODO(Jesse): Put on an asset_system struct?
  u64 CurrentUnnamedAssetIndex;
  asset          AssetTable[ASSET_TABLE_COUNT];
  bonsai_futex   AssetFutex;
  heap_allocator AssetMemory;

};

struct engine_resources
{
  engine_settings Settings;

  bonsai_stdlib Stdlib;

  // TODO(Jesse): Should this go in stdlib?
  renderer_2d Ui;

  application_api GameApi;
       engine_api EngineApi;

  // Engine
  hotkeys     Hotkeys;
  maybe_ray   MaybeMouseRay;
  world      *World;
  game_state *GameState;
  graphics    Graphics;

  file_traversal_node      RequestedGameLibReloadNode;
  game_lib_reload_behavior RequestedGameLibReloadBehavior;

  heap_allocator  Heap;
  memory_arena   *GameMemory; // NOTE(Jesse): This gets passed to the game; hopefully they play nice and do all allocations here..
  memory_arena   *WorldUpdateMemory;

  entity **EntityTable;

  asset_system AssetSystem;

  // TODO(Jesse): Rename this to FrameNumber because it's incremented at the
  // beginning of the frame, meaning we never see (FrameIndex == 0)
  u32 FrameIndex; // At 120fps we get 9k hours (385 days) of frames in 32bits

  tiered_mesh_freelist geo_u3d_MeshFreelist;
  tiered_mesh_freelist world_chunk_MeshFreelist;

  //
  // NOTE(Jesse): This is kinda-sorta all debug stuff
  //

  engine_debug EngineDebug;
  level_editor Editor;

  maybe_picked_voxel MousedOverVoxel;
  maybe_standing_spot ClosestStandingSpotToCursor;

  maybe_entity_ptr HoverEntity;

  render_entity_to_texture_group RTTGroup;

  world_chunk_freelist SynChunkFreelist;
};


link_internal engine_resources *
TryGetEngineResources()
{
  engine_resources *Result = 0;
  if (ThreadLocal_ThreadIndex != INVALID_THREAD_LOCAL_THREAD_INDEX)
  {
    thread_local_state *Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);
    Result = Cast(engine_resources*, Thread->UserData);
  }
  return Result;
}

link_internal engine_resources *
GetEngineResources()
{
  thread_local_state *Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);
  return Cast(engine_resources*, Thread->UserData);
}

link_internal engine_debug *
GetEngineDebug()
{
  auto Result = &GetEngineResources()->EngineDebug;
  return Result;
}

link_internal world *
GetWorld()
{
  auto Result = GetEngineResources()->World;
  return Result;
}

link_internal v3i
GetWorldChunkDim()
{
  return GetWorld()->ChunkDim;
}

// NOTE(Jesse): We have to fail on this for avoiding stackoverflows on startup
link_weak bonsai_stdlib *
GetStdlib()
{
  bonsai_stdlib *Result = 0;
  if (auto Engine = TryGetEngineResources())
  {
    Result = &Engine->Stdlib;
  }
  return Result;
}


link_internal level_editor *
GetLevelEditor()
{
  auto Result = &GetEngineResources()->Editor;
  return Result;
}

link_internal entity *
GetCameraGhost(engine_resources *Engine)
{
  entity *Result = GetEntity(Engine->EntityTable, Engine->Graphics.Camera->GhostId);
  return Result;
}

#define UNPACK_ENGINE_RESOURCES(Res) \
  UNPACK_DATA_RESOURCES(Res)         \
  UNPACK_GRAPHICS_RESOURCES(Res)

 // TODO(Jesse): make this unpack DT
 // r32                        dt            = Plat->dt;
#define UNPACK_DATA_RESOURCES(Res)                                       \
  platform                  *Plat          = &(Res)->Stdlib.Plat;        \
  world                     *World         =  Res->World;                \
  game_state                *GameState     =  Res->GameState;            \
  heap_allocator            *Heap          = &Res->Heap;                 \
  entity                   **EntityTable   =  Res->EntityTable;          \
  hotkeys                   *Hotkeys       = &Res->Hotkeys;              \
  engine_debug              *EngineDebug   = &Res->EngineDebug;          \
  tiered_mesh_freelist      *MeshFreelist  = &Res->geo_u3d_MeshFreelist; \
  input                     *Input         = &Res->Stdlib.Plat.Input;    \
  level_editor              *Editor        = &Res->Editor;               \
  work_queue                *WorldUpdateQ  = &Plat->WorldUpdateQ;        \

#define UNPACK_GRAPHICS_RESOURCES(Res)                                    \
  graphics                  *Graphics      = &Res->Graphics;              \
  lighting_render_group     *Lighting      = &Graphics->Lighting;         \
  renderer_2d               *Ui            = &Res->Ui;                    \
  gpu_mapped_element_buffer *GpuMap        =  GetCurrentGpuMap(Graphics); \
  g_buffer_render_group     *gBuffer       =  Graphics->gBuffer;          \
  camera                    *Camera        =  Graphics->Camera;           \
  camera                    *GameCamera    = &Graphics->GameCamera;       \
  work_queue                *RenderQ       = &Plat->RenderQ

