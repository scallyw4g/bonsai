inline v4
Lerp(r32 t, v4 P1, v4 P2)
{
  Assert(t<=1);
  Assert(t>=0);
  v4 Result = (1.0f-t)*P1 + t*P2;
  return Result;
}

