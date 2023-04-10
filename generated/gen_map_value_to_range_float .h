link_internal r32
MapValueToRange( r32 LowestPossibleValue, r32 Value, r32 HighestPossibleValue)
{
  r32 Range = r32(HighestPossibleValue - LowestPossibleValue);
  r32 Result = r32(Value*Range) + LowestPossibleValue;
  Assert(Result >= LowestPossibleValue);
  Assert(Result <= HighestPossibleValue);
  return Result;
}

