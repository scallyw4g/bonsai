// external/bonsai_stdlib/src/vector.h:233:0

inline v3i
operator+( v3i P1, v3 P2 )
{
  v3i Result = {{
      Cast(s32, P1.E[0] + s32( P2.E[0] )),
      Cast(s32, P1.E[1] + s32( P2.E[1] )),
      Cast(s32, P1.E[2] + s32( P2.E[2] )),
    }};
  return Result;
}



inline v3i
operator-( v3i P1, v3 P2 )
{
  v3i Result = {{
      Cast(s32, P1.E[0] - s32( P2.E[0] )),
      Cast(s32, P1.E[1] - s32( P2.E[1] )),
      Cast(s32, P1.E[2] - s32( P2.E[2] )),
    }};
  return Result;
}



inline v3i
operator*( v3i P1, v3 P2 )
{
  v3i Result = {{
      Cast(s32, P1.E[0] * s32( P2.E[0] )),
      Cast(s32, P1.E[1] * s32( P2.E[1] )),
      Cast(s32, P1.E[2] * s32( P2.E[2] )),
    }};
  return Result;
}



inline v3i
operator/( v3i P1, v3 P2 )
{
  v3i Result = {{
      Cast(s32, P1.E[0] / s32( P2.E[0] )),
      Cast(s32, P1.E[1] / s32( P2.E[1] )),
      Cast(s32, P1.E[2] / s32( P2.E[2] )),
    }};
  return Result;
}






