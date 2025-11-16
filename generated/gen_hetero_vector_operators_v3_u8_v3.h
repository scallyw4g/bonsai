// callsite
// external/bonsai_stdlib/src/vector.h:253:0

// def (gen_hetero_vector_operators)
// external/bonsai_stdlib/src/poof_functions.h:367:0
inline v3_u8
operator+( v3_u8 P1, v3 P2 )
{
  v3_u8 Result;
    Result.E[0] = Cast(u8, P1.E[0] + Cast(u8, P2.E[0]));
  Result.E[1] = Cast(u8, P1.E[1] + Cast(u8, P2.E[1]));
  Result.E[2] = Cast(u8, P1.E[2] + Cast(u8, P2.E[2]));

  return Result;
}



inline v3_u8
operator-( v3_u8 P1, v3 P2 )
{
  v3_u8 Result;
    Result.E[0] = Cast(u8, P1.E[0] - Cast(u8, P2.E[0]));
  Result.E[1] = Cast(u8, P1.E[1] - Cast(u8, P2.E[1]));
  Result.E[2] = Cast(u8, P1.E[2] - Cast(u8, P2.E[2]));

  return Result;
}



inline v3_u8
operator*( v3_u8 P1, v3 P2 )
{
  v3_u8 Result;
    Result.E[0] = Cast(u8, P1.E[0] * Cast(u8, P2.E[0]));
  Result.E[1] = Cast(u8, P1.E[1] * Cast(u8, P2.E[1]));
  Result.E[2] = Cast(u8, P1.E[2] * Cast(u8, P2.E[2]));

  return Result;
}



inline v3_u8
operator/( v3_u8 P1, v3 P2 )
{
  v3_u8 Result;
    Result.E[0] = Cast(u8, P1.E[0] / Cast(u8, P2.E[0]));
  Result.E[1] = Cast(u8, P1.E[1] / Cast(u8, P2.E[1]));
  Result.E[2] = Cast(u8, P1.E[2] / Cast(u8, P2.E[2]));

  return Result;
}






