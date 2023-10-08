
#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>
#include <bonsai_stdlib/test/utils.h>

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


  TestSuiteEnd();
}
