#ifndef BONSAI_PLATFORM_H
#define BONSAI_PLATFORM_H

#include <GL/gl.h>

#define WORK_QUEUE_SIZE (2*VOLUME_VISIBLE_REGION)

#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5

#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF

PROC WINAPI glCreateShader;
PROC WINAPI glShaderSource;
PROC WINAPI glCompileShader;
PROC WINAPI glGetShaderiv;
PROC WINAPI glGetShaderInfoLog;
PROC WINAPI glAttachShader;
PROC WINAPI glDetachShader;
PROC WINAPI glDeleteShader;
PROC WINAPI glCreateProgram;
PROC WINAPI glLinkProgram;
PROC WINAPI glGetProgramiv;
PROC WINAPI glGetProgramInfo;
PROC WINAPI glGetProgramInfoLog;
PROC WINAPI glUseProgram;
PROC WINAPI glDeleteProgram;
PROC WINAPI glGetUniformLocation;
PROC WINAPI glGenFramebuffers;
PROC WINAPI glBindFramebuffer;
PROC WINAPI glFramebufferTexture;
PROC WINAPI glFramebufferTexture2D;
PROC WINAPI glCheckFramebufferStatus;
PROC WINAPI glGenTextures;
PROC WINAPI glBindTexture;
PROC WINAPI glActiveTexture;
PROC WINAPI glTexImage2D;
PROC WINAPI glTexParameteri;
PROC WINAPI glCompressedTexImage2D;
PROC WINAPI glGenBuffers;
PROC WINAPI glBindBuffer;
PROC WINAPI glBufferData;
PROC WINAPI glDrawBuffers;
PROC WINAPI glDeleteBuffers;
PROC WINAPI glVertexAttribPointer;
PROC WINAPI glEnableVertexAttribArray;
PROC WINAPI glDisableVertexAttribArray;
PROC WINAPI glGenVertexArrays;
PROC WINAPI glBindVertexArray;
PROC WINAPI glUniform3fv;
PROC WINAPI glUniformMatrix4fv;
PROC WINAPI glUniform1i;

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

struct platform
{
  work_queue Queue;
  thread_startup_params *Threads;
  void (*PushWorkQueueEntry)(work_queue *Queue, work_queue_entry *Entry);
  real64 (*GetHighPrecisionClock)(void);
  void (*Terminate)(void);
};

struct debug_state
{
  unsigned long long (*GetCycleCount)(void);
};

static debug_state DebugState;

struct game_memory
{
  void* FirstFreeByte;
};

#endif
