
#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>
#include <bonsai_stdlib/test/utils.h>

global_variable memory_arena _Memory;

global_variable memory_arena *Memory = &_Memory;


link_internal void
Print(u32_block_array *Array)
{
  IterateOver(Array, Element, Index)
  {
    DebugChars("Index(%d) Element(%d)\n", GetIndex(&Index), *Element);
  }
}

link_internal u32_block_array
MakeOrderedBlockArray(u32 Count)
{
  u32_block_array Array = U32BlockArray(Memory);

  // Insert 8 elements
  RangeIterator_t(u32, Index, Count)
  {
    Push(&Array, &Index);
  }

  return Array;
}

link_internal void
TrivialAddRemove()
{
  u32_block_array Array = U32BlockArray(Memory);

  u32 Element = 69;

  Push(&Array, &Element);

  {
    u32 Got = 0;
    IterateOver(&Array, E, ElementIndex)
    {
      ++Got;
    }
    TestThat(Got == 1);
  }

  auto Last = LastIndex(&Array);

  RemoveUnordered(&Array, Last);

  TestThat( AtElements(&Array).Index == 0);
}

link_internal void
TwoBlocksRemoveFromEnd()
{
  u32_block_array Array = U32BlockArray(Memory);

  // Insert 8 elements
  RangeIterator_t(u32, Index, 8)
  {
    Push(&Array, &Index);
    TestThat(LastIndex(&Array).Index == Index);
  }
  TestThat(Array.BlockCount == 1);

  {
    u32 Index = 8;
    Push(&Array, &Index);
    TestThat(Array.BlockCount == 2);

    Index = 9;
    Push(&Array, &Index);
    TestThat(Array.BlockCount == 2);
  }

  TestThat(Array.ElementCount == 10 );

  IterateOver(&Array, Value, Index)
  {
    umm IntIndex = GetIndex(&Index);
    auto P = GetPtr(&Array, Index);
    TestThat( *P == IntIndex );
  }

  {
    auto Last = LastIndex(&Array);
    RemoveUnordered(&Array, Last);

    TestThat(Array.ElementCount == 9 );
    TestThat(Array.BlockCount == 2);
  }

  {
    auto Last = LastIndex(&Array);
    RemoveUnordered(&Array, Last);

    TestThat(Array.ElementCount == 8 );
    TestThat(Array.BlockCount == 2);
  }

  {
    auto Last = LastIndex(&Array);
    RemoveUnordered(&Array, Last);

    TestThat(Array.ElementCount == 7 );
    TestThat(Array.BlockCount == 2);
  }
}

link_internal void
TwoBlocksRemoveFromFront()
{
  u32_block_array Array = U32BlockArray(Memory);

  // Insert 8 elements
  RangeIterator_t(u32, Index, 8)
  {
    Push(&Array, &Index);
    TestThat(LastIndex(&Array).Index == Index);
  }
  TestThat(Array.BlockCount == 1);

  {
    u32 Index = 8;
    Push(&Array, &Index);
    TestThat(Array.BlockCount == 2);

    Index = 9;
    Push(&Array, &Index);
    TestThat(Array.BlockCount == 2);
  }


  IterateOver(&Array, Value, Index)
  {
    umm IntIndex = GetIndex(&Index);
    auto P = GetPtr(&Array, Index);
    TestThat( *P == IntIndex );
  }

  {
    auto I = ZerothIndex(&Array);
    RemoveUnordered(&Array, I);
    TestThat(Array.ElementCount == 9);
    TestThat(Array.BlockCount == 2);
  }


  {
    auto I = ZerothIndex(&Array);
    RemoveUnordered(&Array, I);
    TestThat(Array.ElementCount == 8);
    TestThat(Array.BlockCount == 2);
  }


  {
    auto I = ZerothIndex(&Array);
    RemoveUnordered(&Array, I);
    TestThat(Array.ElementCount == 7);
    TestThat(Array.BlockCount == 2);
  }
}

link_internal void
RemoveAllBlocks()
{
  auto Array = MakeOrderedBlockArray(32);

  RangeIterator(Index, 32)
  {
    RemoveUnordered(&Array, ZerothIndex(&Array));

    auto AtE = AtElements(&Array);
    s32 Count = s32(GetIndex(&AtE));
    Assert( Count == 31 - Index );
  }

}

link_internal void
Remove0thElementInTwoElementList()
{
  auto Array = MakeOrderedBlockArray(2);

  auto Z = ZerothIndex(&Array);

  RemoveUnordered(&Array, Z);
  TestThat(Array.ElementCount == 1);

  RemoveUnordered(&Array, Z);
  TestThat(Array.ElementCount == 0);
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Block Array", ArgCount, Args);

  TrivialAddRemove();

  TwoBlocksRemoveFromEnd();

  TwoBlocksRemoveFromFront();

  RemoveAllBlocks();

  Remove0thElementInTwoElementList();

  TestSuiteEnd();
  exit(TestsFailed);
}


