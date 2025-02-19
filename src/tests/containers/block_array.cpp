
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

  Assert( AtElements(&Array).BlockIndex == 0);
  Assert( AtElements(&Array).ElementIndex == 0);
}

link_internal void
TwoBlocksRemoveFromEnd()
{
  u32_block_array Array = U32BlockArray(Memory);

  // Insert 8 elements
  RangeIterator_t(u32, Index, 8)
  {
    Push(&Array, &Index);

    TestThat(LastIndex(&Array).BlockIndex == 0);
    TestThat(LastIndex(&Array).ElementIndex == Index);
  }

  {
    u32 Index = 8;
    Push(&Array, &Index);

    Index = 9;
    Push(&Array, &Index);
  }


  IterateOver(&Array, Value, Index)
  {
    umm IntIndex = GetIndex(&Index);
    auto P = GetPtr(&Array, Index);
    TestThat( *P == IntIndex );
  }

  // 8 elements per block, (10-8 == 2)
  TestThat( Array.Current->At == 2 );

  {
    auto Last = LastIndex(&Array);

    TestThat( Last.BlockIndex == 1 );
    TestThat( Last.ElementIndex == 1 );

    RemoveUnordered(&Array, Last);

    TestThat( Array.First->Next == Last.Block );
    TestThat( Array.First->Next == Array.Current );
  }

  TestThat( Array.Current->At == 1 );

  {
    auto Last = LastIndex(&Array);

    TestThat( Last.BlockIndex == 1 );
    TestThat( Last.ElementIndex == 0 );

    RemoveUnordered(&Array, Last);

    TestThat( Array.Current == Array.First);
  }

  TestThat( Array.Current->At == 8 );

  {
    auto Last = LastIndex(&Array);

    TestThat( Last.BlockIndex == 0 );
    TestThat( Last.ElementIndex == 7 );

    RemoveUnordered(&Array, Last);

    TestThat( Array.Current == Array.First);
  }

  /* TestThat( Last.ElementIndex = 1 ); */
}

link_internal void
TwoBlocksRemoveFromFront()
{
  u32_block_array Array = U32BlockArray(Memory);

  // Insert 8 elements
  RangeIterator_t(u32, Index, 8)
  {
    Push(&Array, &Index);

    TestThat(LastIndex(&Array).BlockIndex == 0);
    TestThat(LastIndex(&Array).ElementIndex == Index);
  }

  {
    u32 Index = 8;
    Push(&Array, &Index);

    Index = 9;
    Push(&Array, &Index);
  }


  IterateOver(&Array, Value, Index)
  {
    umm IntIndex = GetIndex(&Index);
    auto P = GetPtr(&Array, Index);
    TestThat( *P == IntIndex );
  }

  // 8 elements per block, (10-8 == 2)
  TestThat( Array.Current->At == 2 );

  {
    auto I = ZerothIndex(&Array);
    RemoveUnordered(&Array, I);

    auto Last = LastIndex(&Array);
    TestThat( Array.First->Next == Last.Block );
    TestThat( Array.First->Next == Array.Current );
    TestThat( Array.Current->At == 1 );
  }


  {
    auto I = ZerothIndex(&Array);
    RemoveUnordered(&Array, I);

    TestThat( Array.Current == Array.First);
    TestThat( Array.Current->At == 8 );
  }


  {
    auto I = ZerothIndex(&Array);
    RemoveUnordered(&Array, I);

    TestThat( Array.Current == Array.First);
    TestThat( Array.Current->At == 7 );
  }
}


link_internal void
PrintThreeBlocksRemoveFromMiddle()
{
  auto Array = MakeOrderedBlockArray(32);
  TestThat( Array.Current->At == 8 );

  auto I = ZerothIndex(&Array);

  ++I;

  {
    RemoveUnordered(&Array, I);
    TestThat( Array.Current->At == 7 );
    Print(&Array);
    Info("--");
  }

  {
    RemoveUnordered(&Array, I);
    TestThat( Array.Current->At == 6 );
    Print(&Array);
    Info("--");
  }

  {
    I.ElementIndex = 7;
    RemoveUnordered(&Array, I);
    TestThat( Array.Current->At == 5 );
    Print(&Array);
    Info("--");
  }

  {
    ++I;
    RemoveUnordered(&Array, I);
    /* TestThat( Array.Current->At == 6 ); */
    Print(&Array);
    Info("--");
  }

  {
    I = LastIndex(&Array);;
    RemoveUnordered(&Array, I);
    /* TestThat( Array.Current->At == 6 ); */
    Print(&Array);
    Info("--");
  }

  {
    I.ElementIndex = 0;
    RemoveUnordered(&Array, I);
    /* TestThat( Array.Current->At == 6 ); */
    Print(&Array);
    Info("--");
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
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Block Array", ArgCount, Args);

  TrivialAddRemove();

  TwoBlocksRemoveFromEnd();

  TwoBlocksRemoveFromFront();

  PrintThreeBlocksRemoveFromMiddle();

  RemoveAllBlocks();

  Remove0thElementInTwoElementList();

  TestSuiteEnd();
  exit(TestsFailed);
}


