#ifndef WIN32_PLATFORM
#define WIN32_PLATFORM

#include <Windows.h>
#include <WinBase.h>
#include <iostream>

struct work_queue_entry
{
  const char *String;
};

struct thread
{
  int ThreadIndex;
  DWORD ID;
};

struct work_queue
{
  unsigned int EntryCount;
  unsigned int NextEntry;
  work_queue_entry Entries[64];
};

struct thread_startup_params
{
  work_queue *Queue;
  thread Self;
};

struct platform
{
  work_queue Queue;
  thread_startup_params Threads[8];
};

DWORD WINAPI DoWorkerWork(void *Input)
{
  thread_startup_params *ThreadParams = (thread_startup_params *)Input;

  work_queue *Queue = ThreadParams->Queue;
  thread *Self = &ThreadParams->Self;

  for (;;)
  {
    if (Queue->NextEntry < Queue->EntryCount)
    {
      unsigned int EntryIndex = InterlockedIncrement((LONG volatile *)&Queue->NextEntry) - 1;
      work_queue_entry Entry = Queue->Entries[EntryIndex];
      printf("Thread %d, %s\n", Self->ThreadIndex, Entry.String);
    }
  }

  return 0;
}

void
PushString(work_queue *Queue, const char *String)
{
  work_queue_entry Entry = {};
  Entry.String = String;

  Queue->Entries[Queue->EntryCount] = Entry;

  _WriteBarrier();
  _mm_sfence();

  ++Queue->EntryCount;

  return;
}

void
PlatformInit(platform *Platform)
{
  int StackSize = 0;
  DWORD flags = 0;
  DWORD ThreadID = 0;

  work_queue *Queue = &Platform->Queue;

  PushString(&Platform->Queue, "0");
  PushString(&Platform->Queue, "1");
  PushString(&Platform->Queue, "2");
  PushString(&Platform->Queue, "3");
  PushString(&Platform->Queue, "4");
  PushString(&Platform->Queue, "5");
  PushString(&Platform->Queue, "6");
  PushString(&Platform->Queue, "7");
  PushString(&Platform->Queue, "8");
  PushString(&Platform->Queue, "9");

  for (int ThreadIndex = 0;
      ThreadIndex < ArrayCount(Platform->Threads);
      ++ ThreadIndex )
  {
    thread_startup_params *Params = &Platform->Threads[ThreadIndex];
    Params->Self.ThreadIndex = ThreadIndex;
    Params->Queue = Queue;

    HANDLE ThreadHandle = CreateThread( 0, StackSize,
      &DoWorkerWork,
      (void *)Params,
      flags,
      &Params->Self.ID
    );
  }

  PushString(&Platform->Queue, "B0");
  PushString(&Platform->Queue, "B1");
  PushString(&Platform->Queue, "B2");
  PushString(&Platform->Queue, "B3");
  PushString(&Platform->Queue, "B4");
  PushString(&Platform->Queue, "B5");
  PushString(&Platform->Queue, "B6");
  PushString(&Platform->Queue, "B7");
  PushString(&Platform->Queue, "B8");
  PushString(&Platform->Queue, "B9");


  return;
}

#endif
