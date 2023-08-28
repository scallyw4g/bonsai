
struct debug_state;
struct game_state;

struct world;
struct heap_allocator;
struct entity;



enum level_edit_mode
{
  LevelEditMode_None,

  LevelEditMode_Select,
  LevelEditMode_Add,
  LevelEditMode_Remove,
  LevelEditMode_Paint,
};

struct level_editor
{
  level_edit_mode Mode;

  interactable SelectedColor;
  s32 SelectedColorIndex;
};

struct engine_resources
{
  os         *Os;
  platform   *Plat;
  graphics   *Graphics;
  hotkeys    *Hotkeys;

  world      *World;
  game_state *GameState;

  thread_local_state *ThreadStates;

  heap_allocator Heap;
  memory_arena *Memory;

  entity **EntityTable;

  u64 FrameIndex;

  entity *CameraTarget;

  tiered_mesh_freelist MeshFreelist;

  // TODO(Jesse): Put this in Graphics?
  renderer_2d Ui;

  debug_state *DebugState;

  engine_debug EngineDebug;

  level_editor Editor;
  picked_voxel MousedOverVoxel;
};

// TODO(Jesse): Should this actually be a thing?
global_variable engine_resources *Global_EngineResources;


link_internal engine_resources *
GetEngineResources()
{
  Assert(Global_EngineResources);
  return Global_EngineResources;
}

#define UNPACK_ENGINE_RESOURCES(Res) \
  UNPACK_DATA_RESOURCES(Res)         \
  UNPACK_GRAPHICS_RESOURCES(Res)

#define UNPACK_DATA_RESOURCES(Res)                               \
  platform                  *Plat          =  Res->Plat;         \
  world                     *World         =  Res->World;        \
  game_state                *GameState     =  Res->GameState;    \
  memory_arena              *Memory        =  Res->Memory;       \
  heap_allocator            *Heap          = &Res->Heap;         \
  entity                   **EntityTable   =  Res->EntityTable;  \
  hotkeys                   *Hotkeys       =  Res->Hotkeys;      \
  engine_debug              *EngineDebug   = &Res->EngineDebug;  \
  tiered_mesh_freelist      *MeshFreelist  = &Res->MeshFreelist; \
  input                     *Input         = &Res->Plat->Input;  \
  level_editor              *Editor        = &Res->Editor;       \

#define UNPACK_GRAPHICS_RESOURCES(Res)                                    \
  graphics                  *Graphics      =  Res->Graphics;              \
  lighting_render_group     *Lighting      = &Graphics->Lighting;         \
  renderer_2d               *Ui            = &Res->Ui;                    \
  gpu_mapped_element_buffer *GpuMap        =  GetCurrentGpuMap(Graphics); \
  g_buffer_render_group     *gBuffer       =  Graphics->gBuffer;          \
  camera                    *Camera        =  Graphics->Camera;           \

