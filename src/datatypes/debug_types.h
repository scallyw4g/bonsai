#if DEBUG

struct debug_profile_scope
{
  u64 CycleCount;
  const char* FuncName;

  debug_profile_scope *Parent;
  debug_profile_scope *Sibling;
  debug_profile_scope *Child;
};

struct debug_text_render_group;
struct debug_state
{
  u64 (*GetCycleCount)(void);
  u64 FrameCount;

  debug_text_render_group *TextRenderGroup;
  memory_arena *Memory;

  debug_profile_scope **WriteScope;
  debug_profile_scope *CurrentScope;
  debug_profile_scope RootScope;

  u64 NumScopes;
};


debug_global debug_state GlobalDebugState;
inline debug_state*
GetDebugState() { return &GlobalDebugState; }


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

struct debug_timed_function
{
  const char* FuncName;
  u64 StartingCycleCount;

  debug_timed_function(const char *FuncName)
  {
    debug_state *DebugState = GetDebugState();
    DebugState->NumScopes ++;

    // FIXME(Jesse): Recycle these
    debug_profile_scope *NewScope =
      PUSH_STRUCT_CHECKED(debug_profile_scope, DebugState->Memory, 1);

    NewScope->Parent = DebugState->CurrentScope;

    (*DebugState->WriteScope) = NewScope;
    DebugState->CurrentScope = NewScope;

    DebugState->WriteScope = &NewScope->Child;

    this->FuncName = FuncName;
    this->StartingCycleCount = DebugState->GetCycleCount(); // Intentionally last
  }

  ~debug_timed_function()
  {
    debug_state *DebugState = GetDebugState();
    u64 EndingCycleCount = DebugState->GetCycleCount(); // Intentionally first
    u64 CycleCount = (EndingCycleCount - StartingCycleCount);
    DebugState->CurrentScope->CycleCount = CycleCount;

    // 'Pop' the scope stack
    DebugState->CurrentScope = DebugState->CurrentScope->Parent;
    DebugState->WriteScope = &DebugState->CurrentScope->Sibling;
  }
};

#define INIT_DEUBG_STATE(Plat) InitDebugState(Plat)

#define TIMED_FUNCTION() debug_timed_function FunctionTimer(__FUNCTION_NAME__)
#define TIMED_BLOCK(BlockName) { debug_timed_function BlockTimer(BlockName)
#define END_BLOCK(BlockName) }

#define DEBUG_FRAME_RECORD(...) DoDebugFrameRecord(__VA_ARGS__)
#define DEBUG_FRAME_END(Plat) DebugFrameEnd(Plat)

#elif RELEASE

#define INIT_DEUBG_STATE(...)

#define TIMED_FUNCTION(...)
#define TIMED_BLOCK(...)
#define END_BLOCK(...)

#define DEBUG_FRAME_RECORD(...)
#define DEBUG_FRAME_END(...)

#else
#error "Unknown Build State Encountered"
#endif


