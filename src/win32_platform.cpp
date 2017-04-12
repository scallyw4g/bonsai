#ifndef WIN32_PLATFORM
#define WIN32_PLATFORM

#include <Windows.h>
#include <WinBase.h>

#include <win32_platform.h>
#include <platform.h>

inline unsigned int
AtomicCompareExchange( volatile unsigned int *Source, unsigned int Exchange, unsigned int Comparator )
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

int
GetLogicalCoreCount()
{
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  int Result = sysinfo.dwNumberOfProcessors;
  return Result;
}

thread_id
CreateThread( LPTHREAD_START_ROUTINE ThreadMain, thread_startup_params *Params)
{
  DWORD flags = 0;
  int StackSize = 0;

  thread_id ThreadId = CreateThread(
      0,
      0,
      (LPTHREAD_START_ROUTINE)ThreadMain,
      (void *)Params,
      flags,
      0 // &Params->Self.ID
  );

  return ThreadId;
}

#define CompleteAllWrites _WriteBarrier(); _mm_sfence()

unsigned long long
GetCycleCount()
{
  unsigned long long Result = __rdtsc();
  return Result;
}

#endif
