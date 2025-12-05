// callsite
// external/bonsai_stdlib/src/maff.h:24:0

// def (vector_interpolation_functions)
// external/bonsai_stdlib/src/poof_functions.h:386:0
inline f32
Lerp(r32 t, f32 P1, f32 P2)
{
  Assert(t<=1);
  Assert(t>=0);
  f32 Result = (1.0f-t)*P1 + t*P2;
  return Result;
}

// https://paulbourke.net/miscellaneous/interpolation/
//
link_internal f32
CosineInterpolate( f32 t, f32 y1, f32 y2 )
{
  f32 t2 = (1.f-Cos(t*PI32))/2.f;
  return(y1*(1.f-t2)+y2*t2);
}

