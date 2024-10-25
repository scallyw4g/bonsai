// external/bonsai_stdlib/src/random.h:238:0

link_internal s32
RandomBetween( s32 LowestPossibleValue, random_series *Entropy, s32 HighestPossibleValue)
{
  Assert(LowestPossibleValue <= HighestPossibleValue);
  r32 Value = RandomUnilateral(Entropy);
  s32 Result = MapValueToRange(LowestPossibleValue, Value, HighestPossibleValue);
  Assert(Result >= LowestPossibleValue);
  Assert(Result <= HighestPossibleValue);
  return Result;
}

