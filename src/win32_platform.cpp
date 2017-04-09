#ifndef WIN32_PLATFORM
#define WIN32_PLATFORM

#include <Windows.h>
#include <WinBase.h>
#include <iostream>

#define THREAD_MAIN_RETURN DWORD WINAPI

typedef DWORD thread_id
typedef HANDLE semaphore

inline unsigned int
AtomicCompareExchange( unsigned int *Source, unsigned int Exchange, unsigned int Comparator )
{
  unsigned int Result = InterlockedCompareExchange( (LONG volatile *)Source, Exchange, Comparator);
  return Result;
}

inline void
ThreadSleep( semaphore Semaphore )
{
  WaitForSingleObject( Semaphore, INFINITE );
  return;
}

inline void
WakeThread( semaphore Semaphore )
{
  ReleaseSemaphore( Semaphore, 1, 0 );
  return;
}

semaphore
CreateSemaphore( int ThreadCount )
{
  semaphore Result = CreateSemaphore( 0, 0, ThreadCount, 0);
  return Result;
}

thread_id
CreateThread( void* ThreadMain, thread_startup_params *Params)
{
  DWORD flags = 0;

  thread_id ThreadId = CreateThread( 0, StackSize,
    ThreadMain,
    (void *)Params,
    flags,
    &Params->Self.ID
  );

  return ThreadId;
}

#define CompleteAllWrites _WriteBarrier(); _mm_sfence()

void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  Queue->Entries[Queue->EntryCount] = *Entry;


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
