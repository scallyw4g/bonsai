link_internal u32
RandomBetween( u32 LowestPossibleValue, random_series *Entropy, u32 HighestPossibleValue)
{
  Assert(LowestPossibleValue <= HighestPossibleValue);
  r32 Value = RandomUnilateral(Entropy);
  u32 Result = MapValueToRange(LowestPossibleValue, Value, HighestPossibleValue);
  Assert(Result >= LowestPossibleValue);
  Assert(Result <= HighestPossibleValue);
  return Result;
}

