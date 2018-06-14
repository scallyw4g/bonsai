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
  u64 TotalCycles;
  debug_profile_scope *Root;
  r32 FrameMs;
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

#define REGISTERED_MEMORY_ARENA_COUNT 32
#define ROOT_SCOPE_COUNT 64
#define META_TABLE_SIZE (16 * 1024)
struct debug_state
{
  memory_arena            *Memory;
  debug_text_render_group TextRenderGroup;

  untextured_3d_geometry_buffer LineMesh;

  debug_ui_type UIType;

  selected_arenas *SelectedArenas;

  u64 BytesBufferedToCard;
  u64 FrameCount;
  b32 Initialized;
  b32 Debug_RedrawEveryPush;
  b32 DebugDoScopeProfiling = False;

  debug_profile_scope FreeScopeSentinel;
  debug_profile_scope **WriteScope;
  debug_profile_scope *CurrentScope;
  debug_scope_tree ScopeTrees[ROOT_SCOPE_COUNT];
  u32 ReadScopeIndex;
  s32 FreeScopeCount;
  u64 NumScopes;

  push_metadata **MetaTables;

  registered_memory_arena RegisteredMemoryArenas[REGISTERED_MEMORY_ARENA_COUNT];


  debug_scope_tree *GetReadScopeTree()
  {
    debug_scope_tree *RootScope = &this->ScopeTrees[this->ReadScopeIndex];
    return RootScope;
  }

  debug_scope_tree *GetWriteScopeTree()
  {
    if (!this->DebugDoScopeProfiling) return 0;

    s32 Index = (this->ReadScopeIndex + 1) % ROOT_SCOPE_COUNT;
    debug_scope_tree *RootScope = &this->ScopeTrees[Index];
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
#if MEMPROTECT
    State->Memory->MemProtect = False;
#endif
    Result = PUSH_STRUCT_CHECKED(debug_profile_scope, State->Memory, 1);
#if MEMPROTECT
    State->Memory->MemProtect = True;
#endif
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
    Assert (DebugState->WriteScope);

    ++DebugState->NumScopes;

    this->Scope = GetProfileScope(DebugState);

    if (this->Scope)
    {
      this->Scope->Parent = DebugState->CurrentScope;

      (*DebugState->WriteScope) = this->Scope;
      DebugState->CurrentScope = this->Scope;

      DebugState->WriteScope = &this->Scope->Child;

      this->Scope->Name = Name;
      this->StartingCycleCount = GetCycleCount(); // Intentionally last
    }

    /* Debug(" "); */
    /* Debug("Pushing %s", Name); */
    /* PrintScopeTree(DebugState->RootScope); */

  }

  ~debug_timed_function()
  {
    debug_state *DebugState = GetDebugState();
    if (!DebugState->DebugDoScopeProfiling) return;
    if (!this->Scope) return;

    Assert (DebugState->WriteScope);

    u64 EndingCycleCount = GetCycleCount(); // Intentionally first
    u64 CycleCount = (EndingCycleCount - this->StartingCycleCount);
    DebugState->CurrentScope->CycleCount = CycleCount;

    /* Debug(" "); */
    /* Debug("Popping %s", DebugState->CurrentScope->Name); */

    // 'Pop' the scope stack
    DebugState->WriteScope = &DebugState->CurrentScope->Sibling;
    DebugState->CurrentScope = DebugState->CurrentScope->Parent;
    /* PrintScopeTree(DebugState->RootScope); */
  }
};

#define INIT_DEBUG_STATE(PlatPtr, MemArena) InitDebugState(PlatPtr, MemArena)

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


