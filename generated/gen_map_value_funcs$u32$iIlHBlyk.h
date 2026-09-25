// callsite
// external/bonsai_stdlib/src/random.h:207:0

// def (gen_map_value_funcs)
// external/bonsai_stdlib/src/random.h:177:0
link_internal u32
MapValueToRange( u32 LowestPossibleValue, r32 Value, u32 HighestPossibleValue)
{
  r32 Range = r32(HighestPossibleValue - LowestPossibleValue);
  u32 Result = u32(Value*Range) + LowestPossibleValue;
  Assert(Result >= LowestPossibleValue);
  Assert(Result <= HighestPossibleValue);
  return Result;
}

link_internal r32
MapValueToUnilateral( u32 LowestPossibleValue, r32 Value, u32 HighestPossibleValue)
{
  Assert(Value >= LowestPossibleValue);
  Assert(Value <= HighestPossibleValue);
  r32 Range = r32(HighestPossibleValue - LowestPossibleValue);
  r32 Result = (Value-r32(LowestPossibleValue)) / Range;
  Assert(Result >= 0.f);
  Assert(Result <= 1.f);
  return Result;
}

