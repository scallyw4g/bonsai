inline f32
Lerp(r32 t, f32 P1, f32 P2)
{
  Assert(t<=1);
  Assert(t>=0);
  f32 Result = (1.0f-t)*P1 + t*P2;
  return Result;
}

