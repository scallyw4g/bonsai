link_internal r32
MapValueToRange( r32 LowestPossibleValue, r32 Value, r32 HighestPossibleValue, r32 ValueMin = 0.f, r32 ValueMax = 1.f)
{
  /* r32 Range = r32(HighestPossibleValue - LowestPossibleValue); */
  /* t.name Result = t.name(Value*Range) + LowestPossibleValue; */
  auto Result = r32(LowestPossibleValue + (HighestPossibleValue - LowestPossibleValue) * (Value - ValueMin) / (ValueMax - ValueMin));
  return Result;
}

