// callsite
// external/bonsai_stdlib/src/bit_tricks.h:70:0

// def (gen_bit_operators)
// external/bonsai_stdlib/src/bit_tricks.h:4:0
link_internal u64
UnsetLeastSignificantSetBit(u64 *Input)
{
  u64 StartValue = *Input;
  u64 Cleared = StartValue & (StartValue - 1);
  *Input = Cleared;

  u64 Result = (StartValue & ~(Cleared));
  return Result;
}

link_internal u32
CountBitsSet_Kernighan(u64 Input)
{
  u32 Result = 0;
  while (Input != 0)
  {
    Input = Input & (Input - 1);
    Result++;
  }
  return Result;
}

link_internal u64
GetNthSetBit(u64 Target, u32 NBit)
{
  u64 Result = u32_MAX;
  for (u64 BitIndex = 0; BitIndex < NBit; ++BitIndex)
  {
    Result = UnsetLeastSignificantSetBit(&Target);
  }
  return Result;
}

link_internal u32
GetIndexOfNthSetBit(u64 Target, u64 NBit)
{
  Assert(NBit > 0);

  u32 Result = u32_MAX;
  u64 TypeWidth = sizeof(u64)*8;
  u32 Hits = 0;
  for (u32 BitIndex = 0; BitIndex < TypeWidth; ++BitIndex)
  {
    if (Target & (1llu<<BitIndex))
    {
      ++Hits;
    }
    if (Hits == NBit)
    {
      Result = BitIndex;
      break;
    }
  }
  return Result;
}



