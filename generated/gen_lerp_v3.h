// callsite
// external/bonsai_stdlib/src/vector.h:788:0

// def (vector_interpolation_functions)
// external/bonsai_stdlib/src/poof_functions.h:386:0
inline v3
Lerp(r32 t, v3 P1, v3 P2)
{
  Assert(t<=1);
  Assert(t>=0);
  v3 Result = (1.0f-t)*P1 + t*P2;
  return Result;
}

// https://paulbourke.net/miscellaneous/interpolation/
//
link_internal v3
CosineInterpolate( f32 t, v3 y1, v3 y2 )
{
  f32 t2 = (1.f-Cos(t*PI32))/2.f;
  return(y1*(1.f-t2)+y2*t2);
}

