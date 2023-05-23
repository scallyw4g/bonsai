link_internal s32
MapValueToRange( s32 LowestPossibleValue, r32 Value, s32 HighestPossibleValue)
{
  r32 Range = r32(HighestPossibleValue - LowestPossibleValue);
  s32 Result = s32(Value*Range) + LowestPossibleValue;
  Assert(Result >= LowestPossibleValue);
  Assert(Result <= HighestPossibleValue);
  return Result;
}

