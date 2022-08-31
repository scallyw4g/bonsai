
#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>
#include <bonsai_stdlib/test/utils.h>

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("RNG", ArgCount, Args);

#if 0
  random_series Entropy = {43215426453};

  const u32 TableSize = 256;
  u32 HitTable[TableSize] = {};
  u32 MaxValue = 0;

  memory_arena* Memory = AllocateArena(Megabytes(8));

  ansi_stream WordStream = AnsiStreamFromFile(CS(TEST_FIXTURES_PATH "/words.txt"), Memory);

  while (Remaining(&WordStream))
  {
    counted_string Word = PopWordCounted(&WordStream);
    u32 HashValue = Hash(&Word) % TableSize;
    ++HitTable[HashValue];

    if (HitTable[HashValue] > MaxValue)
    {
      MaxValue = HitTable[HashValue];
    }
  }
  DebugLine("Max: %u", MaxValue);

  u32 MappedRowSize = 128;
  for (u32 TableIndex = 0;
      TableIndex < TableSize;
      ++TableIndex)
  {
    u32 TableValue = HitTable[TableIndex];
    r32 Value = (r32)TableValue / (r32)MaxValue;
    u32 Mapped = MapValueToRange(0, Value, MappedRowSize);

    for (u32 ValueIndex = 0;
        ValueIndex < MappedRowSize;
        ++ValueIndex)
    {
      if (ValueIndex < Mapped)
      {
        DebugChars(".");
      }
      else
      {
        DebugChars(" ");
      }
    }
    DebugLine(" | (%u %u) ", TableValue, Mapped);
  }
#endif


  TestSuiteEnd();
  exit(TestsFailed);
}


