inline v2i
operator+( v2i P1, v2 P2 )
{
  v2i Result = {
    .E[0] = P1.E[0] + s32( P2.E[0] ),
    .E[1] = P1.E[1] + s32( P2.E[1] ),
  };
  return Result;
}



inline v2i
operator-( v2i P1, v2 P2 )
{
  v2i Result = {
    .E[0] = P1.E[0] - s32( P2.E[0] ),
    .E[1] = P1.E[1] - s32( P2.E[1] ),
  };
  return Result;
}



inline v2i
operator*( v2i P1, v2 P2 )
{
  v2i Result = {
    .E[0] = P1.E[0] * s32( P2.E[0] ),
    .E[1] = P1.E[1] * s32( P2.E[1] ),
  };
  return Result;
}



inline v2i
operator/( v2i P1, v2 P2 )
{
  v2i Result = {
    .E[0] = P1.E[0] / s32( P2.E[0] ),
    .E[1] = P1.E[1] / s32( P2.E[1] ),
  };
  return Result;
}






