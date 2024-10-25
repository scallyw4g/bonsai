// external/bonsai_stdlib/src/random.h:212:0

link_internal s32
MapValueToRange( s32 LowestPossibleValue, r32 Value, s32 HighestPossibleValue)
{
  r32 Range = r32(HighestPossibleValue - LowestPossibleValue);
  s32 Result = s32(Value*Range) + LowestPossibleValue;
  Assert(Result >= LowestPossibleValue);
  Assert(Result <= HighestPossibleValue);
  return Result;
}

link_internal r32
MapValueToUnilateral( s32 LowestPossibleValue, r32 Value, s32 HighestPossibleValue)
{
  Assert(Value >= LowestPossibleValue);
  Assert(Value <= HighestPossibleValue);
  r32 Range = r32(HighestPossibleValue - LowestPossibleValue);
  r32 Result = (Value-r32(LowestPossibleValue)) / Range;
  Assert(Result >= 0.f);
  Assert(Result <= 1.f);
  return Result;
}

