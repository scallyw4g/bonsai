inline v3
Lerp(r32 t, v3 P1, v3 P2)
{
  Assert(t<=1);
  Assert(t>=0);
  v3 Result = (1.0f-t)*P1 + t*P2;
  return Result;
}

