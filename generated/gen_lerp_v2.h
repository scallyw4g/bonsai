inline v2
Lerp(r32 t, v2 P1, v2 P2)
{
  Assert(t<=1);
  Assert(t>=0);
  v2 Result = (1.0f-t)*P1 + t*P2;
  return Result;
}

