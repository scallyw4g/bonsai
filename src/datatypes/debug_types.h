#if BONSAI_INTERNAL

struct clip_rect
{
  v2 Min;
  v2 Max;
};

struct font
{
  r32 Size;
  r32 LineHeight;
};

struct layout
{
  v2 At;
  u32 Depth;

  v2 Basis;
  clip_rect Clip;
};

struct ui_render_group
{
  debug_text_render_group *TextGroup;
  font Font;
  v2 MouseP;
  v2 ScreenDim;
  struct input *Input;

  /* v2 ViewportDim; */
};

struct debug_profile_scope;
struct scope_stats
{
  debug_profile_scope *MinScope;
  debug_profile_scope *MaxScope;
  b32 IsFirst;
  u32 Calls;
  u64 CumulativeCycles;
};

struct debug_profile_scope
{
  u64 CycleCount;
  const char* Name;
  b32 Expanded;

  debug_profile_scope *Parent;
  debug_profile_scope *Sibling;
  debug_profile_scope *Child;

  scope_stats *Stats;
};

struct debug_scope_tree
{
  debug_profile_scope *Root;
  debug_profile_scope **WriteScope;
  debug_profile_scope *CurrentScope;

  u64 TotalCycles;
  r32 FrameMs;
};

#define SCOPE_TREE_COUNT 64
struct debug_scope_tree_list
{
  debug_scope_tree List[SCOPE_TREE_COUNT];
};

enum debug_ui_type
{
  DebugUIType_None,

  DebugUIType_Graphics,
  DebugUIType_Network,
  DebugUIType_CallGraph,
  DebugUIType_Memory,
  DebugUIType_DrawCalls,

  DebugUIType_Count
};

// TODO(Jesse): Can this be consolidated with the layout struct?
struct table_column
{
  u32 Max;
};

#define MAX_TABLE_COLUMNS 4
struct table_layout
{
  layout Layout;

  table_column Columns[MAX_TABLE_COLUMNS];
  u32 ColumnIndex;
};

struct registered_memory_arena
{
  memory_arena *Arena;
  const char* Name;
  b32 Expanded;

  table_layout StatsTable;
  table_layout BargraphTable;
  table_layout MetadataTable;
};

struct selected_memory_arena
{
  umm ArenaHash;
  umm HeadArenaHash;
};

#define MAX_SELECTED_ARENAS 128
struct selected_arenas
{
  u32 Count;
  selected_memory_arena Arenas[MAX_SELECTED_ARENAS];
};

debug_global __thread u64 ThreadLocal_ThreadIndex = 0;

#define REGISTERED_MEMORY_ARENA_COUNT 32
#define META_TABLE_SIZE (16 * 1024)
struct debug_state
{
  mt_memory_arena            *Memory;
  debug_text_render_group TextRenderGroup;

  untextured_3d_geometry_buffer LineMesh;

  debug_ui_type UIType = DebugUIType_CallGraph;

  selected_arenas *SelectedArenas;

  u64 BytesBufferedToCard;
  u64 FrameCount;
  b32 Initialized;
  b32 Debug_RedrawEveryPush;
  b32 DebugDoScopeProfiling = True;

  debug_profile_scope FreeScopeSentinel;
  debug_scope_tree_list *ThreadScopeTrees;
  u32 ReadScopeIndex;
  s32 FreeScopeCount;
  u64 NumScopes;

  push_metadata **MetaTables;
  mutex *MetaTableMutexes;

  registered_memory_arena RegisteredMemoryArenas[REGISTERED_MEMORY_ARENA_COUNT];


  debug_scope_tree *GetReadScopeTree()
  {
    debug_scope_tree *RootScope = &this->ThreadScopeTrees[ThreadLocal_ThreadIndex].List[this->ReadScopeIndex];
    return RootScope;
  }

  debug_scope_tree *GetWriteScopeTree()
  {
    if (!this->DebugDoScopeProfiling) return 0;

    s32 Index = (this->ReadScopeIndex + 1) % SCOPE_TREE_COUNT;
    debug_scope_tree *RootScope = &this->ThreadScopeTrees[ThreadLocal_ThreadIndex].List[Index];
    return RootScope;
  }
};

struct debug_draw_call
{
  const char * Caller;
  u32 N;
  u32 Calls;
};

debug_global const u32 Global_DrawCallArrayLength = 128;
debug_global debug_draw_call Global_DrawCalls[Global_DrawCallArrayLength] = {};
debug_global debug_draw_call NullDrawCall = {};
debug_global debug_state *GlobalDebugState = 0;

typedef b32 (*meta_comparator)(push_metadata*, push_metadata*);

inline debug_state*
GetDebugState() {
  Assert(GlobalDebugState && GlobalDebugState->Initialized);
  return GlobalDebugState;
}

enum debug_recording_mode
{
  RecordingMode_Clear,
  RecordingMode_Record,
  RecordingMode_Playback,

  RecordingMode_Count,
};

#define DEBUG_RECORD_INPUT_SIZE 3600
struct debug_recording_state
{
  s32 FramesRecorded;
  s32 FramesPlayedBack;
  debug_recording_mode Mode;

  memory_arena RecordedMainMemory;

  hotkeys Inputs[DEBUG_RECORD_INPUT_SIZE];
};

global_variable debug_profile_scope NullDebugProfileScope = {};

debug_profile_scope * GetProfileScope(debug_state *State);

debug_profile_scope *
GetProfileScope(debug_state *State)
{
  debug_profile_scope *Result = 0;
  debug_profile_scope *Sentinel = &State->FreeScopeSentinel;

  if (Sentinel->Child != Sentinel)
  {
    Result = Sentinel->Child;

    Sentinel->Child = Sentinel->Child->Child;
    Sentinel->Child->Child->Parent = Sentinel;
    --State->FreeScopeCount;
  }
  else
  {
    Result = Allocate(debug_profile_scope, State->Memory, 1, False);
  }

  if (Result)
    *Result = NullDebugProfileScope;

  return Result;
}

struct debug_timed_function
{
  u64 StartingCycleCount;
  debug_profile_scope *Scope;

  debug_timed_function(const char *Name)
  {
    debug_state *DebugState = GetDebugState();
    if (!DebugState->DebugDoScopeProfiling) return;
    /* Assert (DebugState->ThreadScopeTrees[ThreadLocal_ThreadIndex].List[0]->WriteScope); */

    ++DebugState->NumScopes;

    this->Scope = GetProfileScope(DebugState);

    if (this->Scope)
    {
      debug_scope_tree *WriteTree = DebugState->GetWriteScopeTree();
      debug_profile_scope *CurrentScope = WriteTree->CurrentScope;
      this->Scope->Parent = CurrentScope;

      (*WriteTree->WriteScope) = this->Scope;
      WriteTree->WriteScope = &this->Scope->Child;
      WriteTree->CurrentScope = this->Scope;

      this->Scope->Name = Name;
      this->StartingCycleCount = GetCycleCount(); // Intentionally last
    }
  }

  ~debug_timed_function()
  {
    debug_state *DebugState = GetDebugState();
    if (!DebugState->DebugDoScopeProfiling) return;
    if (!this->Scope) return;


    u64 EndingCycleCount = GetCycleCount(); // Intentionally first
    u64 CycleCount = (EndingCycleCount - this->StartingCycleCount);

    debug_scope_tree *Tree = DebugState->GetWriteScopeTree();
    Assert (Tree->WriteScope);

    debug_profile_scope *CurrentScope = Tree->CurrentScope;
    CurrentScope->CycleCount = CycleCount;

    // 'Pop' the scope stack
    Tree->WriteScope = &CurrentScope->Sibling;
    CurrentScope = CurrentScope->Parent;
  }

};

#define INIT_DEBUG_STATE(DebugStatePtr, MemArena) InitDebugState(DebugStatePtr, MemArena)

#define TIMED_FUNCTION() debug_timed_function FunctionTimer(BONSAI_FUNCTION_NAME)
#define TIMED_BLOCK(BlockName) { debug_timed_function BlockTimer(BlockName)
#define END_BLOCK(BlockName) }

#define DEBUG_FRAME_RECORD(...) DoDebugFrameRecord(__VA_ARGS__)
#define DEBUG_FRAME_END(Plat, GameState) DebugFrameEnd(Plat, GameState)
#define DEBUG_FRAME_BEGIN(Hotkeys, dt, Cycles) DebugFrameBegin(Hotkeys, dt, Cycles)

#else

#define INIT_DEBUG_STATE(...)

#define TIMED_FUNCTION(...)
#define TIMED_BLOCK(...)
#define END_BLOCK(...)

#define DEBUG_FRAME_RECORD(...)
#define DEBUG_FRAME_END(...)
#define DEBUG_FRAME_BEGIN(...)

#endif


