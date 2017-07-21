#ifndef BONSAI_DEBUG_H
#define BONSAI_DEBUG_H


#include <iostream>
#include <cstdarg>

using namespace std;

#define DEBUG_STATE_ENTRY_COUNT 32



#define DEBUG_FRAMES_TO_AVERAGE 32
struct debug_profile_entry
{
  u64 CycleCount[DEBUG_FRAMES_TO_AVERAGE];
  u32 HitCount;

  r32 MaxPerc;
  r32 MinPerc = 100.0f;

  const char* FuncName;
};

struct debug_state
{
  u64 (*GetCycleCount)(void);
  u32 FrameIndex;
  debug_profile_entry Entries[DEBUG_STATE_ENTRY_COUNT];
};

enum debug_recording_mode
{
  RecordingMode_Clear,
  RecordingMode_Record,
  RecordingMode_Playback,

  RecordingMode_Count,
};

struct debug_recording_state
{
  s32 FramesRecorded;
  s32 FramesPlayedBack;
  debug_recording_mode Mode;

  memory_arena RecordedMainMemory;

  hotkeys Inputs[DEBUG_RECORD_INPUT_SIZE];
};

DEBUG_GLOBAL debug_state GlobalDebugState;

#define TIMED_FUNCTION() debug_timed_function FunctionTimer(__COUNTER__, __FUNCTION_NAME__)
#define TIMED_BLOCK(BlockName) { debug_timed_function BlockTimer(__COUNTER__, BlockName)
#define END_BLOCK(BlockName) }
#define DEBUG_FRAME_END(DebugRG) DebugFrameEnd(DebugRG)


void
InitDebugState( debug_state *DebugState, platform *Plat)
{
  DebugState->GetCycleCount = Plat->GetCycleCount;
  return;
}

inline debug_state*
GetDebugState() { return &GlobalDebugState; }

struct debug_timed_function
{
  u32 FunctionIndex = 0;

  u64 StartingCycleCount;
  u64 EndingCycleCount;

  const char* FuncName;

  debug_timed_function(u32 FunctionIndexIn, const char* FuncNameIn)
  {
    Assert(FunctionIndexIn < DEBUG_STATE_ENTRY_COUNT);
    FunctionIndex = FunctionIndexIn;
    FuncName = FuncNameIn;

    // Record cycle count at last moment
    StartingCycleCount = GetDebugState()->GetCycleCount();
  }

  ~debug_timed_function()
  {
    // Record cycle count ASAP when object is cleaned up
    debug_state *DebugState = GetDebugState();
    EndingCycleCount = DebugState->GetCycleCount();
    debug_profile_entry *Entry = &DebugState->Entries[FunctionIndex];

    Entry->CycleCount[DebugState->FrameIndex] += (EndingCycleCount - StartingCycleCount);

    Entry->HitCount++;
    Entry->FuncName = FuncName;
  }
};

struct debug_text_render_group
{
  u32 Text2DTextureID;
  u32 Text2DVertexBufferID;
  u32 Text2DUVBufferID;

  u32 Text2DShaderID;
  u32 Text2DUniformID;
};

void Log(const char* fmt...)
{
  va_list args;
  va_start(args, fmt);

  while (*fmt != '\0')
  {

    if ( *fmt == '%' )
    {
      ++fmt;

      if (*fmt == 'd')
      {
        cout << va_arg(args, int);
      }
      else if (*fmt == 'u')
      {
        cout << va_arg(args, unsigned int);
      }
      else if (*fmt == 's')
      {
        cout << va_arg(args, char*);
      }
      else if (*fmt == 'f')
      {
        cout << va_arg(args, double);
      }
      else if (*fmt == 'b')
      {
        b32 BoolVal = va_arg(args, int);
        const char *Output = 0;

        if (BoolVal)
          Output = "True";
        else
          Output = "False";

        cout << Output;
      }
      else
      {
        va_arg(args, void*);
        cout << '?';
      }

    }
    else
    {
      cout << (char)(*fmt);
    }

    ++fmt;
  }

  cout << endl;

  va_end(args);
}

#define Print(Pos) \
  Print_P( Pos, #Pos )

void
Print_Binary( unsigned int input )
{
  Log("High bit --> ");
  for (int i = (sizeof(int)*8)-1; i >= 0; --i)
  {
    if ( ((input >> i) & 1) == 1 )
      Log("1");
    else
      Log("0");

    if ( (i % 8) == 0 ) { Log(" "); }
  }

  Log("<-- Low bit \n");
}


inline void
Print_P( s32 N, const char* name)
{
  Log(" -- %s == %d \n", name, N);
}

inline void
Print_P( r32 N, const char* name)
{
  Log(" -- %s == %f \n", name, N);
}

inline void
Print_P( canonical_position P, const char* name)
{
  Log(" -- %s", name);
  Log(" Offset: %f %f %f ", P.Offset.x, P.Offset.y, P.Offset.z );
  Log(" WorldP: %d %d %d \n", P.WorldP.x, P.WorldP.y, P.WorldP.z );
}

inline void
Print_P( voxel_position P, const char* name)
{
  Log(" %s %d %d %d \n", name, P.x, P.y, P.z );
}

inline void
Print_P( v3 P, const char* name)
{
  Log(" %s %f %f %f \n", name, P.x, P.y, P.z );
}

inline void
Print_P( m4 Mat, const char* name)
{
  Log("\n %s ", name );
  Log(" %f %f %f %f ", Mat.E[0].E[0], Mat.E[0].E[1], Mat.E[0].E[2], Mat.E[0].E[3] );
  Log(" %f %f %f %f ", Mat.E[1].E[0], Mat.E[1].E[1], Mat.E[1].E[2], Mat.E[1].E[3] );
  Log(" %f %f %f %f ", Mat.E[2].E[0], Mat.E[2].E[1], Mat.E[2].E[2], Mat.E[2].E[3] );
  Log(" %f %f %f %f ", Mat.E[3].E[0], Mat.E[3].E[1], Mat.E[3].E[2], Mat.E[3].E[3] );
}


#endif
