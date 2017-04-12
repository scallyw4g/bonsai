#ifndef BONSAI_PLATFORM_H
#define BONSAI_PLATFORM_H

#define WORK_QUEUE_SIZE (2*VOLUME_VISIBLE_REGION)

struct work_queue_entry
{
  World *world;
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
};

struct debug_state
{
  unsigned long long (*GetCycleCount)(void);
};

static debug_state DebugState;

#endif
