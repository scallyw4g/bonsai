// callsite
// external/bonsai_stdlib/src/vector.h:785:0

// def (vector_interpolation_functions)
// external/bonsai_stdlib/src/poof_functions.h:386:0
inline v2
Lerp(r32 t, v2 P1, v2 P2)
{
  Assert(t<=1);
  Assert(t>=0);
  v2 Result = (1.0f-t)*P1 + t*P2;
  return Result;
}

// https://paulbourke.net/miscellaneous/interpolation/
//
link_internal v2
CosineInterpolate( f32 t, v2 y1, v2 y2 )
{
  f32 t2 = (1.f-Cos(t*PI32))/2.f;
  return(y1*(1.f-t2)+y2*t2);
}

