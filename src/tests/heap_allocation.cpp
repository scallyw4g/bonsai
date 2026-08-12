
#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>
#include <bonsai_stdlib/test/utils.h>

link_internal void
TestHeapAllocationBasics()
{
  heap_allocator Heap = InitHeap(Megabytes(256));

  u8* AllocationBuffer[64] = {};
  for (umm Index = 0; Index < ArrayCount(AllocationBuffer); ++Index)
  {
    AllocationBuffer[Index] = HeapAllocate(&Heap, 64);
    TestThat(AllocationBuffer[Index]);
  }

  for (umm Index = 0; Index < ArrayCount(AllocationBuffer); ++Index)
  {
    HeapDeallocate(&Heap, AllocationBuffer[Index]);
  }

  DeinitHeap(&Heap);
}

link_internal void
TestHeapAllocationAdvanced()
{
  heap_allocator Heap = InitHeap(Megabytes(256));

  u8* AllocationBuffer[64] = {};

  random_series Entropy = {0x8badf00d};

  for (u32 Iteration = 0; Iteration < 10000; ++Iteration)
  {
    u32 BufferIndex = RandomBetween(0u, &Entropy, 63u);
    u8 *At = AllocationBuffer[BufferIndex];

    if (At)
    {
      HeapDeallocate(&Heap, At);
      AllocationBuffer[BufferIndex] = 0;
    }
    else
    {
      umm RequestedSize = Cast(umm, RandomBetween(0u, &Entropy, Cast(u32, Megabytes(4))));
      AllocationBuffer[BufferIndex] = HeapAllocate(&Heap, RequestedSize);
    }
  }

  for (umm Index = 0; Index < ArrayCount(AllocationBuffer); ++Index)
  {
    if (AllocationBuffer[Index])
    {
      HeapDeallocate(&Heap, AllocationBuffer[Index]);
      AllocationBuffer[Index] = 0;
    }
  }

  DeinitHeap(&Heap);
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("HeapAllocation", ArgCount, Args);

  heap_allocator Heap = InitHeap(Megabytes(1));

  u8* First = HeapAllocate(&Heap, 32);
  u8* Second = HeapAllocate(&Heap, 32);

  TestThat(First);
  TestThat(Second);
  TestThat(First != Second);

  u8* ShouldBeNull = HeapAllocate(&Heap, Megabytes(1));
  TestThat(ShouldBeNull == 0);

  HeapDeallocate(&Heap, First);
  HeapDeallocate(&Heap, Second);


  TestHeapAllocationBasics();
  TestHeapAllocationAdvanced();

  TestSuiteEnd();
}

