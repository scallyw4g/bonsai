inline v2
operator+( v2 P1, v2i P2 )
{
  v2 Result = {
    .E[0] = P1.E[0] + r32( P2.E[0] ),
    .E[1] = P1.E[1] + r32( P2.E[1] ),
  };
  return Result;
}



inline v2
operator-( v2 P1, v2i P2 )
{
  v2 Result = {
    .E[0] = P1.E[0] - r32( P2.E[0] ),
    .E[1] = P1.E[1] - r32( P2.E[1] ),
  };
  return Result;
}



inline v2
operator*( v2 P1, v2i P2 )
{
  v2 Result = {
    .E[0] = P1.E[0] * r32( P2.E[0] ),
    .E[1] = P1.E[1] * r32( P2.E[1] ),
  };
  return Result;
}



inline v2
operator/( v2 P1, v2i P2 )
{
  v2 Result = {
    .E[0] = P1.E[0] / r32( P2.E[0] ),
    .E[1] = P1.E[1] / r32( P2.E[1] ),
  };
  return Result;
}






