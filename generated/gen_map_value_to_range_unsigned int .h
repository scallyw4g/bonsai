link_internal u32
MapValueToRange( u32 LowestPossibleValue, r32 Value, u32 HighestPossibleValue, r32 ValueMin = 0.f, r32 ValueMax = 1.f)
{
  /* r32 Range = r32(HighestPossibleValue - LowestPossibleValue); */
  /* t.name Result = t.name(Value*Range) + LowestPossibleValue; */
  auto Result = u32(LowestPossibleValue + (HighestPossibleValue - LowestPossibleValue) * (Value - ValueMin) / (ValueMax - ValueMin));
  return Result;
}

