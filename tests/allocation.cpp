#define BONSAI_NO_PUSH_METADATA

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <texture.cpp>
#include <shader.cpp>
#include <debug.cpp>

struct test_struct_128
{
  u8 Data[16];
};

struct test_struct_64
{
  u64 Data;
};

struct test_struct_8
{
  u8 Data;
};

global_variable u32 HitSegfault = False;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void
BreakSegfaultHandler(int sig, siginfo_t *si, void *data)
{
  HitSegfault = True;
  Error("Unexpected Segfault");
  RuntimeBreak();
}

void
SegfaultHandler(int sig, siginfo_t *si, void *data)
{
  HitSegfault = True;

  ucontext_t *uc = (ucontext_t *)data;
  u32 instruction_length = 3; // TODO(Jesse): Does this work all the time on x64?
  uc->uc_mcontext.gregs[REG_RIP] += instruction_length;
}

#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
void
NoExpectedSegfault()
{
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = BreakSegfaultHandler;
  if (sigaction(SIGSEGV, &sa, NULL) == -1)
  {
    Error("sigaction not set");
    exit(1);
  }
}

void
ExpectSegfault()
{
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = SegfaultHandler;
  if (sigaction(SIGSEGV, &sa, NULL) == -1)
  {
    Error("sigaction not set");
    exit(1);
  }
}
#pragma clang diagnostic pop

void
AssertNoSegfault()
{
  Assert(HitSegfault == False);
  HitSegfault = False;
}

void
AssertSegfault()
{
  Assert(HitSegfault == True);
  HitSegfault = False;
}

template <typename T> void
TestAllocation(memory_arena *Arena)
{
  T *Test = PUSH_STRUCT_CHECKED( T, Arena, 1);
  Assert(Test);

  Clear(Test);
  AssertNoSegfault();

  ExpectSegfault();
  if (Arena->MemProtect)
  {
    T *NextThing = Test + 1;
    u8 *NextByte = ((u8*)NextThing) + 1;

    *NextByte = 0;
    AssertSegfault();
  }
  NoExpectedSegfault();

  return;
}

global_variable umm PageSize = PlatformGetPageSize();
global_variable umm TwoPages = PageSize*2;


void
ArenaAdvancements()
{
  {
    memory_arena Arena = {};
    SetToPageBoundary(&Arena);
    Assert(Arena.At == 0);

    Arena.At = (u8*)1;
    Arena.End = (u8*)PageSize;
    SetToPageBoundary(&Arena);
    Assert((umm)Arena.At == PageSize);

    SetToPageBoundary(&Arena);
    Assert((umm)Arena.At == PageSize);
  }

  {
    memory_arena Arena = {};
    SetToPageBoundary(&Arena);
    Assert(Arena.At == 0);

    Arena.At = (u8*)1;
    Arena.End = (u8*)PageSize;
    SetToPageBoundary(&Arena);
    Assert((umm)Arena.At == PageSize);

    SetToPageBoundary(&Arena);
    Assert((umm)Arena.At == PageSize);

    Arena.End = (u8*)TwoPages;
    AdvanceToBytesBeforeNextPage(sizeof(memory_arena), &Arena);
    Assert((umm)Arena.At == TwoPages-sizeof(memory_arena));
  }

  {
    memory_arena Arena = {};

    Arena.At = (u8*)(PageSize);
    Assert( OnPageBoundary(&Arena, PageSize) == True);

    Arena.At = (u8*)(PageSize-1);
    Assert( OnPageBoundary(&Arena, PageSize) == False);

    Arena.At = (u8*)1;
    Assert( OnPageBoundary(&Arena, PageSize) == False);
  }


  {
    umm SizeofType = sizeof(memory_arena);
    {
      memory_arena Arena = {};
      Arena.End = (u8*)TwoPages;
      Arena.At = (u8*)(PageSize-1);

      AdvanceToBytesBeforeNextPage(SizeofType, &Arena);
      Assert((umm)Arena.At == TwoPages-SizeofType);
    }

    {
      memory_arena Arena = {};
      Arena.End = (u8*)PageSize;
      Arena.At = (u8*)(PageSize-SizeofType-1);

      AdvanceToBytesBeforeNextPage(SizeofType, &Arena);
      Assert((umm)Arena.At == PageSize-SizeofType);
    }

    {
      memory_arena Arena = {};
      Arena.End = (u8*)PageSize;
      Arena.At = (u8*)(PageSize-SizeofType);

      AdvanceToBytesBeforeNextPage(SizeofType, &Arena);
      Assert((umm)Arena.At == TwoPages-SizeofType);
    }
  }

  return;
}

void
SingleAllocations()
{
  // Single allocation of 64bit type
  {
    memory_arena Arena = {};
    TestAllocation<test_struct_8>(&Arena);
    Assert(Arena.Prev);
  }

  // Single allocation of 8bit type
  {
    memory_arena Arena = {};
    TestAllocation<test_struct_64>(&Arena);
    Assert(Arena.Prev);
  }

  // Single allocation of 128bit type
  {
    memory_arena Arena = {};
    TestAllocation<test_struct_128>(&Arena);
    Assert(Arena.Prev);
  }

  return;
}

void
MultipleAllocations()
{
  // Several Arena re-allocations
  {
    memory_arena Arena = {};
    Assert(Arena.Prev == 0);

    {
      memory_arena *PrevArena = Arena.Prev;
      while ( Arena.Prev == PrevArena )
      {
        TestAllocation<test_struct_64>(&Arena);
      }
    }

    {
      memory_arena *PrevArena = Arena.Prev;
      while ( Arena.Prev == PrevArena )
      {
        TestAllocation<test_struct_64>(&Arena);
      }
    }

  }


  // Several Arena re-allocations with memprotect turned off in the middle
  {
    memory_arena Arena = {};
    Assert(Arena.Prev == 0);

    {
      memory_arena *PrevArena = Arena.Prev;
      while ( Arena.Prev == PrevArena )
      {
        TestAllocation<test_struct_64>(&Arena);
      }
    }

    {
      memory_arena *PrevArena = Arena.Prev;
      Arena.MemProtect = False;
      while ( Arena.Prev == PrevArena )
      {
        TestAllocation<test_struct_8>(&Arena);
        TestAllocation<test_struct_128>(&Arena);
        TestAllocation<test_struct_64>(&Arena);
      }
    }

    {
      memory_arena *PrevArena = Arena.Prev;
      while ( Arena.Prev == PrevArena )
      {
        TestAllocation<test_struct_64>(&Arena);
      }
    }
  }


  // Several Arena re-allocations with memprotect turned off in the middle
  // and accumulated by GetMemoryArenaStats
  {
    memory_arena Arena = {};
    Assert(Arena.Prev == 0);

    {
      memory_arena *PrevArena = Arena.Prev;
      while ( Arena.Prev == PrevArena )
      {
        TestAllocation<test_struct_64>(&Arena);
      }
    }

    {
      memory_arena *PrevArena = Arena.Prev;
      Arena.MemProtect = False;
      while ( Arena.Prev == PrevArena )
      {
        TestAllocation<test_struct_8>(&Arena);
        TestAllocation<test_struct_128>(&Arena);
        TestAllocation<test_struct_64>(&Arena);
      }
    }

    {
      memory_arena *PrevArena = Arena.Prev;
      while ( Arena.Prev == PrevArena )
      {
        TestAllocation<test_struct_8>(&Arena);
        TestAllocation<test_struct_128>(&Arena);
        TestAllocation<test_struct_64>(&Arena);
      }
    }

    {
      memory_arena_stats MemStats1 = GetMemoryArenaStats(&Arena);
      memory_arena_stats MemStats2 = GetMemoryArenaStats(&Arena);
      memory_arena_stats MemStats3 = GetMemoryArenaStats(&Arena);

      Assert( AreEqual(MemStats1,  MemStats2) );
      Assert( AreEqual(MemStats1,  MemStats3) );
      Assert( AreEqual(MemStats2,  MemStats3) );
    }

  }

  return;
}

void
ArenaAllocation()
{
  memory_arena *Arena = PlatformAllocateArena(Megabytes(1));
  Assert( Remaining(Arena) == Megabytes(1) );

  Clear(Arena);

  u8 *ArenaBytes = (u8*)Arena;
  u8 *LastArenaByte = ArenaBytes + sizeof(memory_arena) - 1;
  u8 *FirstMemprotectedByte = ArenaBytes + sizeof(memory_arena);

  ExpectSegfault();
  *FirstMemprotectedByte = 0;
  AssertSegfault();
  NoExpectedSegfault();


  *LastArenaByte = 0;
  AssertNoSegfault();

  return;
}

s32
main()
{

#if MEMPROTECT_OVERFLOW
  ArenaAllocation();

  ArenaAdvancements();

  SingleAllocations();

  MultipleAllocations();
#endif

  return 0;
}

