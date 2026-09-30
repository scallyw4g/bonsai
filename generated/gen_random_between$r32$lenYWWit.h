// callsite
// external/bonsai_stdlib/src/random.h:230:0

// def (gen_random_between)
// external/bonsai_stdlib/src/random.h:215:0
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

