// callsite
// external/bonsai_stdlib/src/vector.h:247:0

// def (gen_hetero_vector_operators)
// external/bonsai_stdlib/src/poof_functions.h:367:0
inline v3i
operator+( v3i P1, v3 P2 )
{
  v3i Result;
    Result.E[0] = Cast(s32, P1.E[0] + Cast(s32, P2.E[0]));
  Result.E[1] = Cast(s32, P1.E[1] + Cast(s32, P2.E[1]));
  Result.E[2] = Cast(s32, P1.E[2] + Cast(s32, P2.E[2]));

  return Result;
}



inline v3i
operator-( v3i P1, v3 P2 )
{
  v3i Result;
    Result.E[0] = Cast(s32, P1.E[0] - Cast(s32, P2.E[0]));
  Result.E[1] = Cast(s32, P1.E[1] - Cast(s32, P2.E[1]));
  Result.E[2] = Cast(s32, P1.E[2] - Cast(s32, P2.E[2]));

  return Result;
}



inline v3i
operator*( v3i P1, v3 P2 )
{
  v3i Result;
    Result.E[0] = Cast(s32, P1.E[0] * Cast(s32, P2.E[0]));
  Result.E[1] = Cast(s32, P1.E[1] * Cast(s32, P2.E[1]));
  Result.E[2] = Cast(s32, P1.E[2] * Cast(s32, P2.E[2]));

  return Result;
}



inline v3i
operator/( v3i P1, v3 P2 )
{
  v3i Result;
    Result.E[0] = Cast(s32, P1.E[0] / Cast(s32, P2.E[0]));
  Result.E[1] = Cast(s32, P1.E[1] / Cast(s32, P2.E[1]));
  Result.E[2] = Cast(s32, P1.E[2] / Cast(s32, P2.E[2]));

  return Result;
}






