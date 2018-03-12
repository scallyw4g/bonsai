#ifndef BONSAI_PLATFORM_H
#define BONSAI_PLATFORM_H

// FIXME(Jesse): Define per application!
#define WORK_QUEUE_SIZE (2048)

struct platform;
struct game_state;
struct memory_arena;
struct hotkeys;
struct work_queue_entry;
struct os;

typedef void (*GameCallback)(void*);
typedef game_state* (*game_init_proc)(platform*, memory_arena*, os *Os);
typedef void (*game_main_proc)(platform*, game_state*, hotkeys*);
typedef void (*game_init_globals_proc)(platform*);
typedef void (*game_thread_callback_proc)(work_queue_entry*);

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

// XXX Make sure this ONLY contains input_event structs - they are cleared in a
// loop and the struct size is used to determine the ending index
struct input
{
  input_event Space;
  input_event Shift;
  input_event Ctrl;
  input_event Alt;

  input_event F12;
  input_event F11;
  input_event F10;
  input_event F9;
  input_event F8;
  input_event F7;
  input_event F6;
  input_event F5;
  input_event F4;
  input_event F3;
  input_event F2;
  input_event F1;

  input_event RMB;
  input_event LMB;
  input_event W;
  input_event A;
  input_event S;
  input_event D;

  input_event Q;
  input_event E;
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

  graphics *Graphics;

  v2 MouseP;
  v2 MouseDP;

  memory_arena *Memory;

  gl_extensions GL;

  r32 dt;
  s32 WindowWidth;
  s32 WindowHeight;

  const char *GlslVersion;

  input Input;

#if BONSAI_INTERNAL
  debug_state DebugState;
#endif
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

inline b32
StringsMatch(const char *S1, const char *S2)
{
  b32 Result = strcmp(S1, S2) == 0;
  return Result;
}

#endif

