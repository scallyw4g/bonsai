// external/bonsai_stdlib/src/vector.h:236:0

inline v3_u8
operator+( v3_u8 P1, v3i P2 )
{
  v3_u8 Result = {{
      Cast(u8, P1.E[0] + u8( P2.E[0] )),
      Cast(u8, P1.E[1] + u8( P2.E[1] )),
      Cast(u8, P1.E[2] + u8( P2.E[2] )),
    }};
  return Result;
}



inline v3_u8
operator-( v3_u8 P1, v3i P2 )
{
  v3_u8 Result = {{
      Cast(u8, P1.E[0] - u8( P2.E[0] )),
      Cast(u8, P1.E[1] - u8( P2.E[1] )),
      Cast(u8, P1.E[2] - u8( P2.E[2] )),
    }};
  return Result;
}



inline v3_u8
operator*( v3_u8 P1, v3i P2 )
{
  v3_u8 Result = {{
      Cast(u8, P1.E[0] * u8( P2.E[0] )),
      Cast(u8, P1.E[1] * u8( P2.E[1] )),
      Cast(u8, P1.E[2] * u8( P2.E[2] )),
    }};
  return Result;
}



inline v3_u8
operator/( v3_u8 P1, v3i P2 )
{
  v3_u8 Result = {{
      Cast(u8, P1.E[0] / u8( P2.E[0] )),
      Cast(u8, P1.E[1] / u8( P2.E[1] )),
      Cast(u8, P1.E[2] / u8( P2.E[2] )),
    }};
  return Result;
}






