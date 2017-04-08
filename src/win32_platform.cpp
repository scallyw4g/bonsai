#ifndef WIN32_PLATFORM
#define WIN32_PLATFORM

#include <Windows.h>
#include <WinBase.h>
#include <iostream>

struct work_queue_entry
{
  void (*Callback)(void*);
  void *Input;
};

struct thread
{
  int ThreadIndex;
  DWORD ID;
};

struct work_queue
{
  HANDLE SemaphoreHandle;
  unsigned int EntryCount;
  volatile unsigned int NextEntry;
  work_queue_entry Entries[WORK_QUEUE_SIZE];
};

struct thread_startup_params
{
  work_queue *Queue;
  thread Self;
};

struct platform
{
  work_queue Queue;
  thread_startup_params Threads[THREAD_COUNT];
};

DWORD WINAPI ThreadMain(void *Input)
{
  thread_startup_params *ThreadParams = (thread_startup_params *)Input;

  work_queue *Queue = ThreadParams->Queue;
  thread *Self = &ThreadParams->Self;

  for (;;)
  {
    unsigned int OriginalNext = Queue->NextEntry;

    if (OriginalNext < Queue->EntryCount)
    {
      unsigned int EntryIndex = InterlockedCompareExchange( (LONG volatile *)&Queue->NextEntry,
                                                            (OriginalNext + 1) % WORK_QUEUE_SIZE,
                                                            OriginalNext);
      if ( EntryIndex == OriginalNext )
      {
        work_queue_entry Entry = Queue->Entries[EntryIndex];

        Entry.Callback(Entry.Input);

        printf("Thread %d executed callback\n", Self->ThreadIndex);
      }

    }
    else
    {
      WaitForSingleObject( Queue->SemaphoreHandle, INFINITE );
    }
  }

  return 0;
}

void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  Queue->Entries[Queue->EntryCount] = *Entry;

  _WriteBarrier();
  _mm_sfence();

  Queue->EntryCount = ++Queue->EntryCount % WORK_QUEUE_SIZE;
  // TODO(Jesse): Is this check correct?
  Assert(Queue->NextEntry != Queue->EntryCount);

  ReleaseSemaphore( Queue->SemaphoreHandle, 1, 0 );

  return;
}

void
PlatformInit(platform *Platform)
{
  int StackSize = 0;
  DWORD flags = 0;
  DWORD ThreadID = 0;

  int ThreadCount = THREAD_COUNT;

  work_queue *Queue = &Platform->Queue;

  Queue->SemaphoreHandle = CreateSemaphore( 0, 0, ThreadCount, 0);

  for (int ThreadIndex = 0;
      ThreadIndex < ThreadCount;
      ++ ThreadIndex )
  {
    thread_startup_params *Params = &Platform->Threads[ThreadIndex];
    Params->Self.ThreadIndex = ThreadIndex;
    Params->Queue = Queue;

    HANDLE ThreadHandle = CreateThread( 0, StackSize,
      &ThreadMain,
      (void *)Params,
      flags,
      &Params->Self.ID
    );
  }

  return;
}

#endif
