
struct debug_profile_entry
{
  u64 CycleCount;
  u32 HitCount;

  r32 MaxPerc;
  r32 MinPerc = 100.0f;

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
  u32 FunctionIndex = 0;

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

