#ifndef WIN32_PLATFORM
#define WIN32_PLATFORM

#include <Windows.h>
#include <iostream>

struct work_queue
{
};

struct work_queue_entry
{
};

struct platform
{
};

DWORD WINAPI DoWorkerWork(void *String)
{
  String = (char *)String;
  printf("%s\n", String);
  return 0;
}

void
PushString(char *String)
{
}

void
PlatformInit(platform *Platform)
{
  int StackSize = 0;
  DWORD flags = 0;
  DWORD ThreadID = 0;

  const char *Input = "Hi!";

  DWORD Threads[8];

  for (int ThreadIndex = 0;
      ThreadIndex < ArrayCount(Threads);
      ++ ThreadIndex; )
  {
    HANDLE ThreadHandle = CreateThread( 0, StackSize,
      &DoWorkerWork,
      (void *)Input,
      flags,
      &ThreadID
    );

    Assert(ThreadID);

    Threads[ThreadIndex] = ThreadID;
  }


  return;
}

#endif
