// external/bonsai_stdlib/src/vector.h:224:0

inline v2
operator+( v2 P1, v2i P2 )
{
  v2 Result = {{
      Cast(r32, P1.E[0] + r32( P2.E[0] )),
      Cast(r32, P1.E[1] + r32( P2.E[1] )),
    }};
  return Result;
}



inline v2
operator-( v2 P1, v2i P2 )
{
  v2 Result = {{
      Cast(r32, P1.E[0] - r32( P2.E[0] )),
      Cast(r32, P1.E[1] - r32( P2.E[1] )),
    }};
  return Result;
}



inline v2
operator*( v2 P1, v2i P2 )
{
  v2 Result = {{
      Cast(r32, P1.E[0] * r32( P2.E[0] )),
      Cast(r32, P1.E[1] * r32( P2.E[1] )),
    }};
  return Result;
}



inline v2
operator/( v2 P1, v2i P2 )
{
  v2 Result = {{
      Cast(r32, P1.E[0] / r32( P2.E[0] )),
      Cast(r32, P1.E[1] / r32( P2.E[1] )),
    }};
  return Result;
}






