

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>
#include <bonsai_stdlib/test/utils.h>


s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Block Array", ArgCount, Args);

  {
    u32_block_array Array = {};

    // Insert 10 elements
    RangeIterator_t(u32, Index, 10)
    {
      Push(&Array, &Index);
    }


    IterateOver(&Array, Value, Index)
    {
      umm IntIndex = GetIndex(&Index);
      TestThat( *GetPtr(&Array, Index) == IntIndex );
    }

    // 8 elements per block, (10-8 == 2)
    TestThat( Array.Current->At == 2 );

    {
      auto Last = AtElements(&Array);

      TestThat( Last.BlockIndex == 1 );
      TestThat( Last.ElementIndex == 1 );

      RemoveUnordered(&Array, Last);

      TestThat( Array.First.Next == GetBlock(&Last) );
      TestThat( Array.First.Next == Array.Current );
    }

    TestThat( Array.Current->At == 1 );

    {
      auto Last = AtElements(&Array);

      TestThat( Last.BlockIndex == 1 );
      TestThat( Last.ElementIndex == 0 );

      RemoveUnordered(&Array, Last);

      /* TestThat( Array.First.Next == GetBlock(&Last) ); */
      TestThat( Array.Current == &Array.First);
    }

    TestThat( Array.Current->At == 8 );

    {
      auto Last = AtElements(&Array);

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


