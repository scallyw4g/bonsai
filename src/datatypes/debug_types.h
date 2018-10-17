#if BONSAI_INTERNAL

#define DEBUG_FRAMES_TRACKED (64)

struct cycle_range
{
  u64 StartCycle;
  u64 TotalCycles;
};

struct memory_arena_stats
{
  u64 Allocations;
  u64 Pushes;

  u64 TotalAllocated;
  u64 Remaining;
};

struct min_max_avg_dt
{
  r32 Min;
  r32 Max;
  r32 Avg;
};

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
  u64 StartingCycle;
  const char* Name;
  b32 Expanded;

  debug_profile_scope *Sibling;
  debug_profile_scope *Child;
  debug_profile_scope *Parent;

  scope_stats *Stats;
};

struct debug_scope_tree
{
  debug_profile_scope *Root;

  debug_profile_scope **WriteScope;
  debug_profile_scope *ParentOfNextScope;

  u64 FrameRecorded;
};

struct debug_thread_state
{
  memory_arena *Memory;
  push_metadata *MetaTable;

  debug_scope_tree *ScopeTrees;
  debug_profile_scope *FirstFreeScope;

  mutex_op_array *MutexOps;

  u32 CurrentFrame;

  u8 Pad[20];
};
CAssert(sizeof(debug_thread_state) == 64); // Make sure we stay at a cache friendly size

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

struct frame_stats
{
  u64 TotalCycles;
  u64 StartingCycle;
  r64 FrameMs;
};

debug_global __thread u64 ThreadLocal_ThreadIndex = 0;

inline debug_thread_state * GetThreadDebugState(u32 ThreadIndex);

struct platform;
struct server_state;
struct hotkeys;
typedef void (*debug_frame_end_proc)(platform*, server_state*);
typedef void (*debug_frame_begin_proc)(hotkeys*, r32);
typedef void (*debug_register_arena_proc)(const char*, memory_arena*);
typedef void (*debug_worker_thread_advance_data_system)();
typedef r32 (*debug_main_thread_advance_data_system)();
typedef void (*debug_mutex_waiting_proc)(mutex*);
typedef void (*debug_mutex_aquired_proc)(mutex*);
typedef void (*debug_mutex_released_proc)(mutex*);
typedef debug_profile_scope* (*debug_get_profile_scope_proc)(debug_thread_state*);
typedef void* (*debug_allocate_proc)(memory_arena*, umm, umm, const char*, s32 , const char*, umm, b32);
typedef void (*debug_register_thread_proc)(u32);
typedef void (*debug_clear_meta_records_proc)(memory_arena*);
typedef void (*debug_init_debug_system_proc)(b32);
typedef void (*debug_track_draw_call_proc)(const char*, u32);




#define REGISTERED_MEMORY_ARENA_COUNT 32
#define META_TABLE_SIZE (16 * 1024)
struct debug_state
{
  debug_text_render_group TextRenderGroup;

  untextured_3d_geometry_buffer LineMesh;

  debug_ui_type UIType;

  selected_arenas *SelectedArenas;

  u64 BytesBufferedToCard;
  b32 Initialized;
  b32 Debug_RedrawEveryPush;
  b32 DebugDoScopeProfiling = True;

  debug_profile_scope FreeScopeSentinel;
  mutex FreeScopeMutex;

  frame_stats Frames[DEBUG_FRAMES_TRACKED];
  debug_thread_state *ThreadStates;

  u32 ReadScopeIndex;
  s32 FreeScopeCount;
  u64 NumScopes;

  registered_memory_arena RegisteredMemoryArenas[REGISTERED_MEMORY_ARENA_COUNT];

  volatile b32 MainThreadBlocksWorkerThreads;
  volatile s32 WorkerThreadsWaiting;

  debug_scope_tree* GetReadScopeTree(u32 ThreadIndex)
  {
    debug_scope_tree *RootScope = &this->ThreadStates[ThreadIndex].ScopeTrees[this->ReadScopeIndex];
    return RootScope;
  }

  debug_scope_tree* GetWriteScopeTree()
  {
    debug_thread_state *ThreadState = GetThreadDebugState(ThreadLocal_ThreadIndex);
    debug_scope_tree *RootScope = ThreadState->ScopeTrees + (ThreadState->CurrentFrame % DEBUG_FRAMES_TRACKED);
    return RootScope;
  }

  debug_frame_end_proc                      FrameEnd;
  debug_frame_begin_proc                    FrameBegin;
  debug_register_arena_proc                 RegisterArena;
  debug_worker_thread_advance_data_system   WorkerThreadAdvanceDebugSystem;
  debug_main_thread_advance_data_system     MainThreadAdvanceDebugSystem;
  debug_mutex_waiting_proc                  MutexWait;
  debug_mutex_aquired_proc                  MutexAquired;
  debug_mutex_released_proc                 MutexReleased;
  debug_get_profile_scope_proc              GetProfileScope;
  debug_allocate_proc                       Debug_Allocate;
  debug_register_thread_proc                RegisterThread;
  debug_clear_meta_records_proc             ClearMetaRecordsFor;
  debug_init_debug_system_proc              InitDebugSystem;
  debug_track_draw_call_proc                TrackDrawCall;
};

typedef debug_state* (*get_debug_state_proc)();
get_debug_state_proc GetDebugState;


struct debug_draw_call
{
  const char * Caller;
  u32 N;
  u32 Calls;
};

typedef b32 (*meta_comparator)(push_metadata*, push_metadata*);

inline debug_thread_state *
GetThreadDebugState(u32 ThreadIndex)
{
  debug_state *State = GetDebugState();
  debug_thread_state *Result = State->ThreadStates + ThreadIndex;
  return Result;
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

struct debug_timed_function
{
  debug_profile_scope *Scope;
  debug_scope_tree *Tree;

  debug_timed_function(const char *Name)
  {
    if (!GetDebugState) return;

    debug_state *DebugState = GetDebugState();
    Clear(this);
    if (!DebugState->DebugDoScopeProfiling) return;

    ++DebugState->NumScopes;

    this->Scope = DebugState->GetProfileScope(GetThreadDebugState(ThreadLocal_ThreadIndex));
    this->Tree = DebugState->GetWriteScopeTree();

    if (this->Scope)
    {
      (*this->Tree->WriteScope) = this->Scope;
      this->Tree->WriteScope = &this->Scope->Child;

      this->Scope->Parent = this->Tree->ParentOfNextScope;
      this->Tree->ParentOfNextScope = this->Scope;

      this->Scope->Name = Name;
      this->Scope->StartingCycle = GetCycleCount(); // Intentionally last
    }

    return;
  }

  ~debug_timed_function()
  {
    if (!GetDebugState) return;

    debug_state *DebugState = GetDebugState();
    if (!DebugState->DebugDoScopeProfiling) return;
    if (!this->Scope) return;

    u64 EndingCycleCount = GetCycleCount(); // Intentionally first
    u64 CycleCount = (EndingCycleCount - this->Scope->StartingCycle);
    this->Scope->CycleCount = CycleCount;

    // 'Pop' the scope stack
    this->Tree->WriteScope = &this->Scope->Sibling;
    this->Tree->ParentOfNextScope = this->Scope->Parent;

    return;
  }

};

#define TIMED_FUNCTION() debug_timed_function FunctionTimer(BONSAI_FUNCTION_NAME)
#define TIMED_BLOCK(BlockName) { debug_timed_function BlockTimer(BlockName)
#define END_BLOCK(BlockName) }

#define DEBUG_FRAME_RECORD(...) DoDebugFrameRecord(__VA_ARGS__)
#define DEBUG_FRAME_END(Plat, ServerState) GetDebugState()->FrameEnd(Plat, ServerState)
#define DEBUG_FRAME_BEGIN(Hotkeys, dt) GetDebugState()->FrameBegin(Hotkeys, dt)

#ifndef BONSAI_NO_MUTEX_TRACKING
void DebugTimedMutexWaiting(mutex *Mut);
void DebugTimedMutexAquired(mutex *Mut);
void DebugTimedMutexReleased(mutex *Mut);
#define TIMED_MUTEX_WAITING(Mut)  GetDebugState()->MutexWait(Mut)
#define TIMED_MUTEX_AQUIRED(Mut)  GetDebugState()->MutexAquired(Mut)
#define TIMED_MUTEX_RELEASED(Mut) GetDebugState()->MutexReleased(Mut)
#else
#define TIMED_MUTEX_WAITING(...)
#define TIMED_MUTEX_AQUIRED(...)
#define TIMED_MUTEX_RELEASED(...)

#endif

#define WORKER_THREAD_ADVANCE_DEBUG_SYSTEM() GetDebugState()->WorkerThreadAdvanceDebugSystem()
#define MAIN_THREAD_ADVANCE_DEBUG_SYSTEM() GetDebugState()->MainThreadAdvanceDebugSystem()
#define SUSPEND_WORKER_THREADS()  DebugSuspendWorkerThreads()
#define RESUME_WORKER_THREADS()  DebugResumeWorkerThreads()

#define DEBUG_CLEAR_META_RECORDS_FOR(Arena) GetDebugState()->ClearMetaRecordsFor(Arena)
#define DEBUG_TRACK_DRAW_CALL(CallingFunction, VertCount) GetDebugState()->TrackDrawCall(CallingFunction, VertCount)

#else

#define TIMED_FUNCTION(...)
#define TIMED_BLOCK(...)
#define END_BLOCK(...)

#define TIMED_MUTEX_WAITING(...)
#define TIMED_MUTEX_AQUIRED(...)
#define TIMED_MUTEX_RELEASED(...)

#define DEBUG_FRAME_RECORD(...)
#define DEBUG_FRAME_END(...)
#define DEBUG_FRAME_BEGIN(...)

#define WORKER_THREAD_WAIT_FOR_DEBUG_SYSTEM(...)
#define MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(...) RealDt
#define SUSPEND_WORKER_THREADS(...)

#define DEBUG_CLEAR_META_RECORDS_FOR(...)
#define DEBUG_TRACK_DRAW_CALL(...)

#endif

