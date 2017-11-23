#ifndef BONSAI_PLATFORM_H
#define BONSAI_PLATFORM_H

#include <platform_constants.h>
#include <bonsai_types.h>

#ifdef _WIN32
#include <win32_platform.h>
#else
#include <unix_platform.h>
#endif

#define Assert(condition) \
  if (!(condition)) { Debug(" ! Failed - '%s' on Line: %d in File: %s", #condition, __LINE__, __FILE__); RuntimeBreak(); }

// FIXME(Jesse): Define per application!
#define WORK_QUEUE_SIZE (2048)

struct platform;
struct game_state;
struct memory_arena;
struct hotkeys;
struct work_queue_entry;

typedef void (*GameCallback)(void*);
typedef game_state* (*game_init_proc)(platform*, memory_arena*);
typedef void (*game_main_proc)(platform*, game_state*, hotkeys*);
typedef void (*game_init_globals_proc)(platform*);
typedef void (*game_thread_callback_proc)(work_queue_entry*);

global_variable v2 InvalidMouseP = {-1, -1};

#define PUSH_STRUCT_CHECKED(Type, Arena, Number) \
  (Type*)PushStructChecked_( Arena, sizeof(Type)*Number, #Type, __LINE__, __FILE__ );

enum work_queue_entry_flags
{
  WorkEntry_InitWorldChunk = 1 << 0,
};
struct work_queue_entry
{
  game_state *GameState;
  void *Input;
  work_queue_entry_flags Flags;
};

struct thread
{
  int ThreadIndex;
  thread_id ID;
};

struct work_queue
{
  semaphore Semaphore;
  volatile unsigned int EnqueueIndex;
  volatile unsigned int DequeueIndex;
  work_queue_entry *Entries;
};

struct thread_startup_params
{
  work_queue *Queue;
  thread Self;
};

struct gl_extensions
{
  PFNGLCREATESHADERPROC glCreateShader;
  PFNGLSHADERSOURCEPROC glShaderSource;
  PFNGLCOMPILESHADERPROC glCompileShader;
  PFNGLGETSHADERIVPROC glGetShaderiv;
  PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
  PFNGLATTACHSHADERPROC glAttachShader;
  PFNGLDETACHSHADERPROC glDetachShader;
  PFNGLDELETESHADERPROC glDeleteShader;
  PFNGLCREATEPROGRAMPROC glCreateProgram;
  PFNGLLINKPROGRAMPROC glLinkProgram;
  PFNGLGETPROGRAMIVPROC glGetProgramiv;
  PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
  PFNGLUSEPROGRAMPROC glUseProgram;
  PFNGLDELETEPROGRAMPROC glDeleteProgram;
  PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
  PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
  PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
  /* PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture; */
  PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
  PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
  PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
  PFNGLGENBUFFERSPROC glGenBuffers;
  PFNGLBINDBUFFERPROC glBindBuffer;
  PFNGLBUFFERDATAPROC glBufferData;
  PFNGLDRAWBUFFERSPROC glDrawBuffers;
  PFNGLDELETEBUFFERSPROC glDeleteBuffers;
  PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
  PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
  PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
  PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
  PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
  PFNGLUNIFORM3FVPROC glUniform3fv;
  PFNGLUNIFORM2FVPROC glUniform2fv;
  PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
  PFNGLUNIFORM1IPROC glUniform1i;
  PFNGLACTIVETEXTUREPROC glActiveTexture;
  PFNGLUNIFORM1FPROC glUniform1f;
  PFNGLUNIFORM1UIPROC glUniform1ui;

  // Platform specific (wgl / glX)
  PFNSWAPINTERVALPROC glSwapInterval;
};

struct input
{
  b32 Space;
  b32 Shift;
  b32 Ctrl;
  b32 Alt;

  b32 F12;
  b32 F11;
  b32 F10;
  b32 F9;
  b32 F8;
  b32 F7;
  b32 F6;
  b32 F5;
  b32 F4;
  b32 F3;
  b32 F2;
  b32 F1;

  b32 RMB;
  b32 LMB;
  b32 W;
  b32 A;
  b32 S;
  b32 D;

  b32 Q;
  b32 E;

  s32 DeltaMouseX;
  s32 DeltaMouseY;
};

struct platform
{
  work_queue Queue;
  thread_startup_params *Threads;
  r64 (*GetHighPrecisionClock)(void);
  u64 (*GetCycleCount)(void);
  umm (*Allocate)(u8 Bytes);
  void* (*PushStruct)(memory_arena *Memory, umm sizeofStruct );
  void* (*PushStructChecked_)(memory_arena *Memory, umm sizeofStruct, const char* StructName, s32 Line, const char* File);

  v2 MouseP;
  v2 MouseDP;

  memory_arena *Memory;

  gl_extensions GL;

  r32 dt;
  s32 WindowWidth;
  s32 WindowHeight;

  const char *GlslVersion;

  input Input;
};

struct os
{
  window Window;
  display Display;
  gl_context GlContext;

  b32 ContinueRunning;
};

struct logical_frame_state
{
  u64 LogicalFrame;
  u64 FrameDiff;
  r32 LogicalFrameTime;
};

void
DumpGlErrorEnum(int Error)
{
  if ( Error != 0 )
  {
    Error("%d", Error);
  }

  switch (Error)
  {
    case GL_INVALID_ENUM:
    {
      Error(" GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_INVALID_VALUE:
    {
      Error(" GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_INVALID_OPERATION:
    {
      Error(" GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
    {
      Error(" GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_OUT_OF_MEMORY:
    {
      Error(" GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n");
    } break;

    case GL_STACK_UNDERFLOW:
    {
      Error(" GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n");
    } break;

    case GL_STACK_OVERFLOW:
    {
      Error(" GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n");
    } break;

    case GL_NO_ERROR:
    {
      // Happy days :D
    } break;

    default :
    {
      Error("Some weird OpenGL shit happened\n");
    } break;
  }

  return;
}

u8*
Allocate(umm Bytes)
{
  u8* Result = (u8*)calloc(1, (size_t)Bytes);
  if (!Result)
  {
    Assert(False);
    Error("Unable to allocate memory!");
  }
  return Result;
}

void*
PushSize(memory_arena *Arena, umm Size)
{
  void* Result = 0;

  if (Size <= Arena->Remaining)
  {
    Result = (void*)Arena->FirstFreeByte;
    Arena->FirstFreeByte += Size;
    Arena->Remaining -= Size;
  }

  return Result;
}

void*
PushStruct(memory_arena *Memory, umm sizeofStruct)
{
  void* Result = PushSize(Memory, sizeofStruct);
  return Result;
}

inline void*
PushStructChecked_(memory_arena *Arena, umm Size, const char* StructType, s32 Line, const char* File)
{
  void* Result = PushStruct( Arena, Size );

  if (!(Result)) {
    Error("Pushing %s on Line: %d, in file %s", StructType, Line, File); return False;
  }

  return Result;
}

// TODO(Jesse): Does this function correctly?
inline void
SubArena( memory_arena *Src, memory_arena *Dest, umm Size)
{
  Dest->FirstFreeByte = (u8*)PushSize(Src, Size);
  Dest->Remaining = Size;
  Dest->TotalSize = Size;

  Assert(Dest->FirstFreeByte);

  return;
}

inline void
AllocateAndInitializeArena(memory_arena *Arena, umm Size)
{
  Arena->Remaining = Size;
  Arena->TotalSize = Size;

  Arena->FirstFreeByte = Allocate(Arena->Remaining);

  Assert(Arena->FirstFreeByte);

  return;
}

inline void
Rewind(memory_arena *Memory)
{
  Memory->FirstFreeByte = Memory->FirstFreeByte - (Memory->TotalSize - Memory->Remaining);
  Memory->Remaining = Memory->TotalSize;

  return;
}

inline void
MemCopy(u8 *Src, u8 *Dest, umm Size)
{
  // TODO(Jesse): Vectorize for speed boost!
  for( umm BytesCopied = 0;
       BytesCopied < Size;
       ++BytesCopied )
  {
     Dest[BytesCopied] = Src[BytesCopied];
  }

}

inline void
CopyArena(memory_arena *Src, memory_arena *Dest)
{
  Rewind(Dest);
  Assert(Dest->Remaining >= Src->TotalSize);

  u8 *FirstSrcByte = Src->FirstFreeByte - (Src->TotalSize - Src->Remaining);

  u8 *FirstDestByte = (u8*)PushSize(Dest, Src->TotalSize);

  MemCopy( FirstSrcByte, FirstDestByte, Src->TotalSize);

  return;
}



inline void
UpdateLogicalFrameCount(logical_frame_state *State, r32 dt)
{
  State->LogicalFrameTime += dt;
  State->FrameDiff = 0;

  if (State->LogicalFrameTime >= TargetFrameTime)
  {
    s32 FramesElapsed = State->LogicalFrameTime / TargetFrameTime;
    State->LogicalFrame += FramesElapsed;
    State->LogicalFrameTime -= (TargetFrameTime*FramesElapsed);
    State->FrameDiff = FramesElapsed;
  }

  return;
}

inline b32
CurrentFrameIsLogicalFrame(logical_frame_state *State)
{
  b32 Result = (State->FrameDiff > 0);
  return Result;
}

#endif

