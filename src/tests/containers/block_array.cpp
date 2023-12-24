

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>
#include <bonsai_stdlib/test/utils.h>


link_internal void
TrivialAddRemove()
{
  u32_block_array Array = {};

  u32 Element = 69;

  Push(&Array, &Element);

  auto Last = LastIndex(&Array);

  RemoveUnordered(&Array, Last);

  Assert( AtElements(&Array).BlockIndex == 0);
  Assert( AtElements(&Array).ElementIndex == 0);
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Block Array", ArgCount, Args);

  TrivialAddRemove();

  {
    u32_block_array Array = {};

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

      TestThat( Array.First.Next == GetBlock(&Last) );
      TestThat( Array.First.Next == Array.Current );
    }

    TestThat( Array.Current->At == 1 );

    {
      auto Last = LastIndex(&Array);

      TestThat( Last.BlockIndex == 1 );
      TestThat( Last.ElementIndex == 0 );

      RemoveUnordered(&Array, Last);

      /* TestThat( Array.First.Next == GetBlock(&Last) ); */
      TestThat( Array.Current == &Array.First);
    }

    TestThat( Array.Current->At == 8 );

    {
      auto Last = LastIndex(&Array);

      TestThat( Last.BlockIndex == 0 );
      TestThat( Last.ElementIndex == 7 );

      RemoveUnordered(&Array, Last);

      TestThat( Array.Current == &Array.First);
    }

    /* TestThat( Last.ElementIndex = 1 ); */
  }

  TestSuiteEnd();
  exit(TestsFailed);
}


