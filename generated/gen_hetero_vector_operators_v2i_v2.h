// external/bonsai_stdlib/src/vector.h:227:0

inline v2i
operator+( v2i P1, v2 P2 )
{
  v2i Result = {{
      Cast(s32, P1.E[0] + s32( P2.E[0] )),
      Cast(s32, P1.E[1] + s32( P2.E[1] )),
    }};
  return Result;
}



inline v2i
operator-( v2i P1, v2 P2 )
{
  v2i Result = {{
      Cast(s32, P1.E[0] - s32( P2.E[0] )),
      Cast(s32, P1.E[1] - s32( P2.E[1] )),
    }};
  return Result;
}



inline v2i
operator*( v2i P1, v2 P2 )
{
  v2i Result = {{
      Cast(s32, P1.E[0] * s32( P2.E[0] )),
      Cast(s32, P1.E[1] * s32( P2.E[1] )),
    }};
  return Result;
}



inline v2i
operator/( v2i P1, v2 P2 )
{
  v2i Result = {{
      Cast(s32, P1.E[0] / s32( P2.E[0] )),
      Cast(s32, P1.E[1] / s32( P2.E[1] )),
    }};
  return Result;
}






