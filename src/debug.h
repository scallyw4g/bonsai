#ifndef BONSAI_DEBUG_H
#define BONSAI_DEBUG_H

#include <iostream>
#include <cstdarg>

using namespace std;


#define DEBUG_STATE_ENTRY_COUNT 2048


struct debug_profile_entry
{
  u64 CycleCount;
  u32 HitCount;
  const char* FuncName;
};

struct debug_state
{
  u64 (*GetCycleCount)(void);
  debug_profile_entry Entries[DEBUG_STATE_ENTRY_COUNT];
};

DEBUG_GLOBAL debug_state GlobalDebugState;

#define TIMED_FUNCTION() debug_timed_function FN(__COUNTER__, __FUNCTION_NAME__)
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

    // Record cycle count at last moment before returning
    StartingCycleCount = GetDebugState()->GetCycleCount();
  }

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
      else if (*fmt == 's')
      {
        cout << va_arg(args, char*);
      }
      else if (*fmt == 'f')
      {
        cout << va_arg(args, double);
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
    {
      Log("1");
    }
    else
    {
      Log("0");
    }
    if ( (i % 8) == 0 ) { Log(" "); }
  }

  Log("<-- Low bit \n");
}

inline void
Print_P( canonical_position P, const char* name)
{
  Log(" -- %s\n", name);
  Log(" Offset: %f %f %f \n", P.Offset.x, P.Offset.y, P.Offset.z );
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
