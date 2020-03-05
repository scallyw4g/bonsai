#include <bonsai_types.h>
#include <unix_platform.cpp>
#include <test_utils.cpp>


struct test_struct_1k
{
  u8 Data[1024];
};

struct test_struct_128
{
  u8 Data[16];
};

struct test_struct_32
{
  u32 Data;
};

struct test_struct_64
{
  u64 Data;
};

struct test_struct_8
{
  u8 Data;
};

struct test_struct_16
{
  u8 Data[2];
};

global_variable u32 HitSegfault = False;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void
BreakSegfaultHandler(int sig, siginfo_t *si, void *data)
{
  HitSegfault = True;
  Debug(RED_TERMINAL "   CRASH! " WHITE_TERMINAL);
  PlatformDebugStacktrace();
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
  TestThat(HitSegfault == False);
  HitSegfault = False;
}

void
AssertSegfault()
{
  TestThat(HitSegfault == True);
  HitSegfault = False;
}

template <typename T> T*
TestAllocation(memory_arena *Arena, b32 Protection = True, umm Alignment = 1)
{
  /* T *Test = (T*)Allocate_( Arena, sizeof(T), Alignment, Protection); */
  T *Test = AllocateAlignedProtection( T, Arena, 1, Alignment, Protection);
  TestThat(Test);
  Clear(Test);

  AssertNoSegfault();

  if (Protection)
  {
    ExpectSegfault();
    T *NextThing = Test + 1;
    u8 *NextByte = ((u8*)NextThing);
    NextByte = AlignTo(NextByte, Alignment);

    *NextByte = 0;
    AssertSegfault();
  }

  NoExpectedSegfault();

  return Test;
}

global_variable umm PageSize = PlatformGetPageSize();
global_variable umm TwoPages = PageSize*2;



template <typename T> void
TestAlignmentHelper(memory_arena *Arena, b32 MemProtect)
{
  {
    T* Result = TestAllocation<T>(Arena, MemProtect, 8);
    TestThat((umm)Result % 8 == 0);
  }
  {
    T* Result = TestAllocation<T>(Arena, MemProtect, 16);
    TestThat((umm)Result % 16 == 0);
  }
  {
    T* Result = TestAllocation<T>(Arena, MemProtect, 64);
    TestThat((umm)Result % 64 == 0);
  }

  {
    T* Result = TestAllocation<T>(Arena, MemProtect, 8);
    TestThat((umm)Result % 8 == 0);
  }
  {
    T* Result = TestAllocation<T>(Arena, MemProtect, 16);
    TestThat((umm)Result % 16 == 0);
  }
  {
    T* Result = TestAllocation<T>(Arena, MemProtect, 64);
    TestThat((umm)Result % 64 == 0);
  }

  return;
}

void
TestAlignment()
{
  {
    memory_arena Arena = {};
    Arena.At = (u8*)1;
    Arena.End = (u8*)Gigabytes(1);
    {
      umm Align = 8;
      AlignTo(&Arena, Align);
      TestThat((umm)Arena.At == Align);
    }

    {
      umm Align = 16;
      AlignTo(&Arena, Align);
      TestThat((umm)Arena.At == Align);
    }

    {
      umm Align = 24;
      AlignTo(&Arena, Align);
      TestThat((umm)Arena.At == Align);
    }

    {
      umm Align = 32;
      AlignTo(&Arena, Align);
      TestThat((umm)Arena.At == Align);
    }

    {
      umm Align = 64;
      AlignTo(&Arena, Align);
      TestThat((umm)Arena.At == Align);

      AlignTo(&Arena, Align);
      TestThat((umm)Arena.At == Align);
    }


    {
      /* Arena.At = (u8*)633; */
      /* TestAlignmentHelper<test_struct_8>(&Arena, True); */
    }

  }



  {
    memory_arena Arena = {};
    TestAlignmentHelper<test_struct_8>(&Arena, True);
  }
  {
    memory_arena Arena = {};
    TestAlignmentHelper<test_struct_16>(&Arena, True);
  }
  {
    memory_arena Arena = {};
    TestAlignmentHelper<test_struct_32>(&Arena, True);
  }
  {
    memory_arena Arena = {};
    TestAlignmentHelper<test_struct_64>(&Arena, True);
  }


  {
    memory_arena Arena = {};
    test_struct_128 *Struct = TestAllocation<test_struct_128>(&Arena, True, 4096);
    Assert((umm)Struct % 4096 == 0);
  }

  return;
}

void
AdvanceToBytesBeforeNextPage(umm Bytes, memory_arena *Arena)
{
  umm At = (umm)Arena->At;
  umm AtInPage = (At % PageSize);
  umm AtToPageBoundary = PageSize - AtInPage;

  if (AtToPageBoundary < Bytes)
  {
    SetToPageBoundary(Arena);
    AdvanceToBytesBeforeNextPage(Bytes, Arena);
  }
  else
  {
    umm EndOfBytes = (umm)Arena->At + Bytes;
    umm EndToNextPage = PageSize - (EndOfBytes % PageSize);
    Arena->At += EndToNextPage;

    Assert( (EndOfBytes+EndToNextPage) % PageSize == 0);
    Assert( ((umm)Arena->At + Bytes) % PageSize == 0);
  }

#if 0
  Assert(Bytes < PageSize); // TODO(Jesse): Prove this is unnecessary

  umm EndOfBytes = (umm)Arena->At + Bytes;
  umm StartingPage = (umm)Arena->At - ((umm)Arena->At % PageSize);
  umm StartingPageToEndOfBytes = EndOfBytes - StartingPage;

  umm EndToNextPage = PageSize - StartingPageToEndOfBytes;
  Assert( (EndOfBytes+EndToNextPage) % PageSize == 0);

  Arena->At += EndToNextPage;
  Assert(Arena->At <= Arena->End);
#endif

  return;
}

void
TestSetToPageBoundary()
{
  memory_arena Arena = {};
  SetToPageBoundary(&Arena);
  TestThat(Arena.At == 0);

  Arena.At = (u8*)1;
  Arena.End = (u8*)PageSize;
  SetToPageBoundary(&Arena);
  TestThat((umm)Arena.At == PageSize);

  SetToPageBoundary(&Arena);
  TestThat((umm)Arena.At == PageSize);

  Arena.End = (u8*)TwoPages;
  AdvanceToBytesBeforeNextPage(sizeof(memory_arena), &Arena);
  TestThat((umm)Arena.At == TwoPages-sizeof(memory_arena));

  return;
}


void
TestOnPageBoundary()
{
  memory_arena Arena = {};
  TestThat( OnPageBoundary(&Arena, PageSize) == True);

  Arena.At = (u8*)(PageSize);
  TestThat( OnPageBoundary(&Arena, PageSize) == True);

  Arena.At = (u8*)(PageSize-1);
  TestThat( OnPageBoundary(&Arena, PageSize) == False);

  Arena.At = (u8*)1;
  TestThat( OnPageBoundary(&Arena, PageSize) == False);

  return;
}




void
ArenaAdvancements()
{
  TestSetToPageBoundary();

  TestOnPageBoundary();


  {
    umm SizeofType = sizeof(memory_arena);

    {
      memory_arena Arena = {};
      Arena.End = (u8*)TwoPages;
      Arena.At = (u8*)(PageSize-1);

      AdvanceToBytesBeforeNextPage(SizeofType, &Arena);
      TestThat((umm)Arena.At == TwoPages-SizeofType);
    }

    {
      memory_arena Arena = {};
      Arena.End = (u8*)PageSize;
      Arena.At = (u8*)(PageSize-SizeofType-1);

      AdvanceToBytesBeforeNextPage(SizeofType, &Arena);
      TestThat((umm)Arena.At == PageSize-SizeofType);
    }

    {
      memory_arena Arena = {};
      Arena.End = (u8*)PageSize;
      Arena.At = (u8*)(PageSize-SizeofType);

      AdvanceToBytesBeforeNextPage(SizeofType, &Arena);
      TestThat((umm)Arena.At == TwoPages-SizeofType);
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
    TestThat(Arena.Prev);
  }

  // Single allocation of 8bit type
  {
    memory_arena Arena = {};
    TestAllocation<test_struct_64>(&Arena);
    TestThat(Arena.Prev);
  }

  // Single allocation of 128bit type
  {
    memory_arena Arena = {};
    TestAllocation<test_struct_128>(&Arena);
    TestThat(Arena.Prev);
  }

  return;
}

void
MultipleAllocations()
{
  // Several Arena re-allocations
  {
    memory_arena Arena = {};
    TestThat(Arena.Prev == 0);

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
    TestThat(Arena.Prev == 0);

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
        TestAllocation<test_struct_8>(&Arena, False);
        TestAllocation<test_struct_128>(&Arena, False);
        TestAllocation<test_struct_64>(&Arena, False);
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
    TestThat(Arena.Prev == 0);

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
        TestAllocation<test_struct_8>(&Arena, False);
        TestAllocation<test_struct_128>(&Arena, False);
        TestAllocation<test_struct_64>(&Arena, False);
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

      TestThat( AreEqual(MemStats1,  MemStats2) );
      TestThat( AreEqual(MemStats1,  MemStats3) );
      TestThat( AreEqual(MemStats2,  MemStats3) );
    }

  }

  return;
}

void
ArenaAllocation()
{
  {
    memory_arena *Arena = AllocateArena(Megabytes(1));
    TestThat( Remaining(Arena) >= Megabytes(1) );

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
    VaporizeArena(Arena);
  }

  {
    memory_arena *Arena = AllocateArena(32);
    TestThat(Remaining(Arena) >= 32);
    VaporizeArena(Arena);
  }

  return;
}

void
UnprotectedAllocations()
{
  NoExpectedSegfault();

  { // Tiny allocation works
    memory_arena *Arena = AllocateArena(32);
    TestThat(Remaining(Arena) >= 32);
    VaporizeArena(Arena);
  }

  { // Most basic allocation works
    memory_arena *Arena = AllocateArena(Megabytes(1));
    TestThat( Remaining(Arena) >= Megabytes(1) );

    TestAllocation<test_struct_1k>(Arena, False);
    AssertNoSegfault();
    VaporizeArena(Arena);
  }

  { // Arena Reallocation works
    umm AllocationSize = 32;
    memory_arena *Arena = AllocateArena(AllocationSize);
    TestThat( Remaining(Arena) >= AllocationSize );

    while (!Arena->Prev)
    {
      TestAllocation<test_struct_1k>(Arena, False);
    }

    AssertNoSegfault();
    VaporizeArena(Arena);
  }


  { // Writing to each allocation works
    const u32 StructCount = 1024;
    test_struct_1k *Structs[StructCount];


    memory_arena *Arena = AllocateArena(Kilobytes(1));

    for (u32 StructIndex = 0;
        StructIndex < StructCount;
        ++StructIndex)
    {
      AllocateProtection(test_struct_32, Arena, 1, False);
      AllocateProtection(test_struct_64, Arena, 1, False);

      Structs[StructIndex] = AllocateProtection(test_struct_1k, Arena, 1, False);
      Fill(Structs[StructIndex], (u8)255);
      AssertNoSegfault();
    }

    TestAllocation<test_struct_1k>(Arena, False);
    AssertNoSegfault();
    VaporizeArena(Arena);
  }

  { // Arena De-allocation works
    memory_arena *TestArena = AllocateArena(32);

    AllocateProtection(test_struct_32, TestArena, 1, False);

    AllocateProtection(test_struct_64, TestArena, 1, False);
    AllocateProtection(test_struct_64, TestArena, 1, False);
    AllocateProtection(test_struct_64, TestArena, 1, False);

    AllocateProtection(test_struct_1k, TestArena, 1, False);

    test_struct_1k *TestStruct = AllocateProtection(test_struct_1k, TestArena, 1, False);

    VaporizeArena(TestArena);
    AssertNoSegfault();

    { // Since we're using virtual memory writing to the freed pages directly
      // after should give us a memory fault - on linux at least.
      ExpectSegfault();

      *(u8*)TestArena = 0;
      AssertSegfault();

      *(u8*)TestStruct = 0;
      AssertSegfault();

      NoExpectedSegfault();
    }

  }

  return;
}

void
UnprotectedAlignment()
{
  memory_arena Arena = {};
  Arena.At = (u8*)1;
  Arena.End = (u8*)0xffffffffffff;

  test_struct_64* SecondThing = AllocateAlignedProtection(test_struct_64, &Arena, 1, CACHE_LINE_SIZE, False);


  TestThat((umm)SecondThing % CACHE_LINE_SIZE == 0);
}

s32
main()
{
  TestSuiteBegin("Allocation");

#if MEMPROTECT_OVERFLOW
    ArenaAllocation();

    ArenaAdvancements();

    SingleAllocations();

    MultipleAllocations();

    UnprotectedAllocations();

    TestAlignment();

    UnprotectedAlignment();
#endif

  {
    memory_arena* TestArena = AllocateArena();
    u8* Thing = AllocateAlignedProtection(u8, TestArena, 32, 1, False);
    TestThat(Thing);
    u8* Thing2 = Reallocate(Thing, TestArena, 32, 64);
    TestThat(Thing2);
    u8* Thing3 = Reallocate(Thing, TestArena, 64, 128);
    TestThat(Thing3);
    VaporizeArena(TestArena);
  }

  {
    memory_arena* TestArena = AllocateArena();
    u8* Thing = Allocate(u8, TestArena, 32);
    TestThat(Thing);
    u8* Thing2 = Reallocate(Thing, TestArena, 32, 64);
    TestThat(!Thing2);
    VaporizeArena(TestArena);
  }

  TestSuiteEnd();
  exit(TestsFailed);
}

