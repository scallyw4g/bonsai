#ifndef BONSAI_PLATFORM_H
#define BONSAI_PLATFORM_H

#include <platform_constants.h>

#ifdef _WIN32
#include <win32_platform.h>
#else
#include <unix_platform.h>
#endif


#define WORK_QUEUE_SIZE (2*VOLUME_VISIBLE_REGION)

struct platform;
struct game_state;

typedef void (*GameCallback)(void*);
typedef game_state* (*game_init_proc)(platform*);
typedef bool (*game_main_proc)(platform*, game_state*);


// FIXME(Jesse): Surely there's a way to not have work_queue_entires contain the world
struct work_queue_entry
{
  void (*Callback)(void*);
  void *Input;
};

struct thread
{
  int ThreadIndex;
  thread_id ID;
};

struct work_queue
{
  semaphore Semaphore;
  volatile unsigned int EntryCount;
  volatile unsigned int NextEntry;
  work_queue_entry *Entries;
};

struct thread_startup_params
{
  work_queue *Queue;
  thread Self;
};

struct game_memory
{
  void* FirstFreeByte;
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
  PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
  PFNGLUNIFORM1IPROC glUniform1i;
  PFNGLACTIVETEXTUREPROC glActiveTexture;
};

struct input
{
  b32 F11;
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
  void (*PushWorkQueueEntry)(work_queue *Queue, work_queue_entry *Entry);
  r64 (*GetHighPrecisionClock)(void);
  void (*Terminate)(void);

  game_memory GameMemory;
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
};

struct debug_state
{
  unsigned long long (*GetCycleCount)(void);
};

#endif
