// external/bonsai_stdlib/src/vector.h:230:0

inline v3
operator+( v3 P1, v3i P2 )
{
  v3 Result = {{
      Cast(r32, P1.E[0] + r32( P2.E[0] )),
      Cast(r32, P1.E[1] + r32( P2.E[1] )),
      Cast(r32, P1.E[2] + r32( P2.E[2] )),
    }};
  return Result;
}



inline v3
operator-( v3 P1, v3i P2 )
{
  v3 Result = {{
      Cast(r32, P1.E[0] - r32( P2.E[0] )),
      Cast(r32, P1.E[1] - r32( P2.E[1] )),
      Cast(r32, P1.E[2] - r32( P2.E[2] )),
    }};
  return Result;
}



inline v3
operator*( v3 P1, v3i P2 )
{
  v3 Result = {{
      Cast(r32, P1.E[0] * r32( P2.E[0] )),
      Cast(r32, P1.E[1] * r32( P2.E[1] )),
      Cast(r32, P1.E[2] * r32( P2.E[2] )),
    }};
  return Result;
}



inline v3
operator/( v3 P1, v3i P2 )
{
  v3 Result = {{
      Cast(r32, P1.E[0] / r32( P2.E[0] )),
      Cast(r32, P1.E[1] / r32( P2.E[1] )),
      Cast(r32, P1.E[2] / r32( P2.E[2] )),
    }};
  return Result;
}






