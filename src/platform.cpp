#ifndef BONSAI_PLATFORM_CPP
#define BONSAI_PLATFORM_CPP

#include <iostream>

#ifdef _WIN32
#include <win32_platform.cpp>
#else
#include <unix_platform.cpp>
#endif

THREAD_MAIN_RETURN
ThreadMain(void *Input)
{
  thread_startup_params *ThreadParams = (thread_startup_params *)Input;

  work_queue *Queue = ThreadParams->Queue;
  thread *Self = &ThreadParams->Self;

  for (;;)
  {
    unsigned int OriginalNext = Queue->NextEntry;

    if (OriginalNext != Queue->EntryCount)
    {
      if ( AtomicCompareExchange(&Queue->NextEntry,
                                 (OriginalNext + 1) % WORK_QUEUE_SIZE,
                                 OriginalNext))
      {
        work_queue_entry Entry = Queue->Entries[OriginalNext];
        Entry.Callback(&Entry);
      }

    }
    else
    {
      ThreadSleep( Queue->Semaphore );
    }
  }

  return 0;
}

void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  Queue->Entries[Queue->EntryCount] = *Entry;

  CompleteAllWrites;

  Queue->EntryCount = ++Queue->EntryCount % WORK_QUEUE_SIZE;

  // TODO(Jesse): Is this check correct?
  Assert(Queue->NextEntry != Queue->EntryCount);

  WakeThread( Queue->Semaphore );

  return;
}

void
PlatformInit(platform *Platform)
{
  int StackSize = 0;

  int LogicalCoreCount = GetLogicalCoreCount();
  int ThreadCount = LogicalCoreCount -1; // -1 because we already have a main thread

  Platform->Queue.Entries = (work_queue_entry *)calloc(sizeof(work_queue_entry), WORK_QUEUE_SIZE);
  Platform->Threads = (thread_startup_params *)calloc(sizeof(thread_startup_params), ThreadCount);
  work_queue *Queue = &Platform->Queue;

  Queue->Semaphore = CreateSemaphore(ThreadCount);

  for (int ThreadIndex = 0;
      ThreadIndex < ThreadCount;
      ++ ThreadIndex )
  {
    thread_startup_params *Params = &Platform->Threads[ThreadIndex];
    Params->Self.ThreadIndex = ThreadIndex;
    Params->Queue = Queue;

    thread_id ThreadID = CreateThread( ThreadMain, Params );
  }

  return;
}

debug_state *
GetDebugState(void)
{
  return &DebugState;
}

#endif
