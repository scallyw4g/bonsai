#ifndef BONSAI_PLATFORM_H
#define BONSAI_PLATFORM_H

#include <GL/gl.h>
#include <GL/glext.h>

#define WORK_QUEUE_SIZE (2*VOLUME_VISIBLE_REGION)

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

struct platform
{
  work_queue Queue;
  thread_startup_params *Threads;
  void (*PushWorkQueueEntry)(work_queue *Queue, work_queue_entry *Entry);
  real64 (*GetHighPrecisionClock)(void);
  void (*Terminate)(void);

  game_memory GameMemory;

  real32 dt;
};

struct debug_state
{
  unsigned long long (*GetCycleCount)(void);
};

static debug_state DebugState;

#endif
