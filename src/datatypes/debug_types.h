#if NDEBUG // CMAKE defined
#define RELEASE 1
#else
#define DEBUG 1
#endif

#if DEBUG

struct debug_profile_entry
{
  u64 CycleCount;
  u32 HitCount;

  r32 MaxPerc;
  r32 MinPerc;

  const char* FuncName;
};

#define DEBUG_STATE_ENTRY_COUNT 32
struct debug_text_render_group;
struct debug_state
{
  u64 (*GetCycleCount)(void);
  u64 FrameCount;
  debug_profile_entry Entries[DEBUG_STATE_ENTRY_COUNT];

  debug_text_render_group *TextRenderGroup;
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
  u32 FunctionIndex;

  u64 StartingCycleCount;
  u64 EndingCycleCount;

  const char* FuncName;

  ~debug_timed_function()
  {
    // Record cycle count ASAP when object is cleaned up
    debug_state *DebugState = GetDebugState();
    EndingCycleCount = DebugState->GetCycleCount();
    debug_profile_entry *Entry = &DebugState->Entries[FunctionIndex];

    Entry->CycleCount += (EndingCycleCount - StartingCycleCount);

    Entry->HitCount++;
    Entry->FuncName = FuncName;
  }
};

debug_timed_function
DebugTimedFunction(u32 FunctionIndexIn, const char* FuncNameIn)
{
  Assert(FunctionIndexIn < DEBUG_STATE_ENTRY_COUNT);

  debug_timed_function Result = {};
  Result.FunctionIndex = FunctionIndexIn;
  Result.FuncName = FuncNameIn;

  // Record cycle count at last moment
  Result.StartingCycleCount = GetDebugState()->GetCycleCount();
  return Result;
}

#define INIT_DEUBG_STATE(Plat) InitDebugState(Plat)

#define TIMED_FUNCTION() debug_timed_function FunctionTimer = DebugTimedFunction(__COUNTER__, __FUNCTION_NAME__)
#define TIMED_BLOCK(BlockName) { debug_timed_function BlockTimer = DebugTimedFunction(__COUNTER__, BlockName)
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


