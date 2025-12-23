// callsite
// external/bonsai_stdlib/src/maff.h:431:0

// def (scalar_math_fuctions)
// external/bonsai_stdlib/src/maff.h:416:0
inline r32
Clamp( r32 Min, r32 Value, r32 Max )
{
  if (Value > Max) { Value = Max; }
  if (Value < Min) { Value = Min; }
  return Value;
}
inline r64
Clamp( r64 Min, r64 Value, r64 Max )
{
  if (Value > Max) { Value = Max; }
  if (Value < Min) { Value = Min; }
  return Value;
}
inline u32
Clamp( u32 Min, u32 Value, u32 Max )
{
  if (Value > Max) { Value = Max; }
  if (Value < Min) { Value = Min; }
  return Value;
}
inline u64
Clamp( u64 Min, u64 Value, u64 Max )
{
  if (Value > Max) { Value = Max; }
  if (Value < Min) { Value = Min; }
  return Value;
}
inline s32
Clamp( s32 Min, s32 Value, s32 Max )
{
  if (Value > Max) { Value = Max; }
  if (Value < Min) { Value = Min; }
  return Value;
}
inline s64
Clamp( s64 Min, s64 Value, s64 Max )
{
  if (Value > Max) { Value = Max; }
  if (Value < Min) { Value = Min; }
  return Value;
}


