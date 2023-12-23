

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>
#include <bonsai_stdlib/test/utils.h>

link_internal b32 
Sorted(sort_key *Keys, s32 KeyCount, b32 DebugPrint = False)
{
  b32 Result = True;
  if (DebugPrint) { LogDirect("Keys(%u)\n", KeyCount); }
  RangeIterator(Index, KeyCount)
  {
    if (DebugPrint) { LogDirect("Index(%d) SortIndex(%lu) Value(%lu)\n", Index, Keys[Index].Index, Keys[Index].Value); }
    s32 NextIndex = Min(Index+1, KeyCount-1);
    Assert(NextIndex < KeyCount);
    Result |= (Keys[Index].Value <= Keys[NextIndex].Value);
  }

  return Result;
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Sort", ArgCount, Args);

  {
    u64 SortKeyIndex = 0;
    sort_key Keys[] = 
    {
      {SortKeyIndex++, 42},
      {SortKeyIndex++, 420},
      {SortKeyIndex++, 69},
      {SortKeyIndex++, 1},
      {SortKeyIndex++, 6969},
      {SortKeyIndex++, 1},
      {SortKeyIndex++, 0},
      {SortKeyIndex++, 7},
    };

    BubbleSort(Keys, ArrayCount(Keys));

    b32 DebugPrint = True;
    TestThat( Sorted(Keys, ArrayCount(Keys), DebugPrint) );
  }

  {
    random_series Entropy = {654365745693};

    u64 KeyCount = 1000;
    sort_key *Keys = Allocate(sort_key, GetTranArena(), KeyCount);

    RangeIterator_t(u64, KeyIndex, KeyCount)
    {
      Keys[KeyIndex].Index = KeyIndex;
      Keys[KeyIndex].Value = u64(RandomU32(&Entropy));
    }

    BubbleSort(Keys, u32(KeyCount));

    TestThat( Sorted(Keys, s32(KeyCount), True) );
  }

  TestSuiteEnd();
  exit(TestsFailed);
}


