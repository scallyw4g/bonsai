// callsite
// external/bonsai_stdlib/src/random.h:233:0

// def (gen_random_between)
// external/bonsai_stdlib/src/random.h:215:0
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

