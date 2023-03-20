link_internal r32
RandomBetween( r32 LowestPossibleValue, random_series *Entropy, r32 HighestPossibleValue)
{
  Assert(LowestPossibleValue <= HighestPossibleValue);
  r32 Value = RandomUnilateral(Entropy);
  r32 Result = MapValueToRange(LowestPossibleValue, Value, HighestPossibleValue);
  Assert(Result >= LowestPossibleValue);
  Assert(Result <= HighestPossibleValue);
  return Result;
}

