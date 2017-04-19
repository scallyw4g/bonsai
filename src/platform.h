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

#define glCreateShader(...)             0
#define glShaderSource(...)             0
#define glCompileShader(...)            0
#define glGetShaderiv(...)              0
#define glGetShaderInfoLog(...)         0
#define glAttachShader(...)             0
#define glDetachShader(...)             0
#define glDeleteShader(...)             0

#define glCreateProgram(...)            0
#define glLinkProgram(...)              0
#define glGetProgramiv(...)             0
#define glGetProgramInfo(...)           0
#define glGetProgramInfoLog(...)        0
#define glUseProgram(...)               0
#define glDeleteProgram(...)            0

#define glGetUniformLocation(...)       0

#define glGenFramebuffers(...)          0
#define glBindFramebuffer(...)          0
#define glFramebufferTexture(...)       0
#define glFramebufferTexture2D(...)     0
#define glCheckFramebufferStatus(...)   0

#define glGenTextures(...)              0
#define glBindTexture(...)              0
#define glActiveTexture(...)            0
#define glTexImage2D(...)               0
#define glTexParameteri(...)            0
#define glCompressedTexImage2D(...)     0

#define glGenBuffers(...)               0
#define glBindBuffer(...)               0
#define glBufferData(...)               0
#define glDrawBuffers(...)              0
#define glDeleteBuffers(...)            0

#define glVertexAttribPointer(...)      0
#define glEnableVertexAttribArray(...)  0
#define glDisableVertexAttribArray(...) 0
#define glGenVertexArrays(...)          0
#define glBindVertexArray(...)          0

#define glUniform3fv(...)               0
#define glUniformMatrix4fv(...)         0
#define glUniform1i(...)                0

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
