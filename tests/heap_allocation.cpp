#include <bonsai_types.h>
#include <unix_platform.cpp>
#include <heap_memory_types.cpp>

#include <test_utils.cpp>

s32
main()
{
  TestSuiteBegin("HeapAllocation");

  heap_allocator Heap = InitHeap(Megabytes(1));

  u8* First = HeapAllocate(&Heap, 32);
  u8* Second = HeapAllocate(&Heap, 32);

  TestThat(First);
  TestThat(Second);
  TestThat(First != Second);

  u8* ShouldBeNull = HeapAllocate(&Heap, Megabytes(1));
  TestThat(ShouldBeNull == 0);

  HeapDeallocate(First);
  HeapDeallocate(Second);


  TestSuiteEnd();
}
