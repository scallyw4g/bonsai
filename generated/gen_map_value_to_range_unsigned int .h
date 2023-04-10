link_internal u32
MapValueToRange( u32 LowestPossibleValue, r32 Value, u32 HighestPossibleValue)
{
  r32 Range = r32(HighestPossibleValue - LowestPossibleValue);
  u32 Result = u32(Value*Range) + LowestPossibleValue;
  Assert(Result >= LowestPossibleValue);
  Assert(Result <= HighestPossibleValue);
  return Result;
}

