// external/bonsai_stdlib/src/vector.h:229:0

inline v2i
operator+( v2i P1, v2 P2 )
{
  v2i Result;
  Result.E[0] = Cast(s32, P1.E[0] + Cast(s32, P2.E[0]));
  Result.E[1] = Cast(s32, P1.E[1] + Cast(s32, P2.E[1]));
  return Result;
}



inline v2i
operator-( v2i P1, v2 P2 )
{
  v2i Result;
  Result.E[0] = Cast(s32, P1.E[0] - Cast(s32, P2.E[0]));
  Result.E[1] = Cast(s32, P1.E[1] - Cast(s32, P2.E[1]));
  return Result;
}



inline v2i
operator*( v2i P1, v2 P2 )
{
  v2i Result;
  Result.E[0] = Cast(s32, P1.E[0] * Cast(s32, P2.E[0]));
  Result.E[1] = Cast(s32, P1.E[1] * Cast(s32, P2.E[1]));
  return Result;
}



inline v2i
operator/( v2i P1, v2 P2 )
{
  v2i Result;
  Result.E[0] = Cast(s32, P1.E[0] / Cast(s32, P2.E[0]));
  Result.E[1] = Cast(s32, P1.E[1] / Cast(s32, P2.E[1]));
  return Result;
}






