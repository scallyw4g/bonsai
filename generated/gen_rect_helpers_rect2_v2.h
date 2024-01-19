link_internal rect2
RectMinMax(v2 Min, v2 Max)
{
  rect2 Result = { .Min = Min, .Max = Max };
  return Result;
}

link_internal rect2
RectMinRad(v2 Min, v2 Rad)
{
  rect2 Result = { .Min = Min, .Max = Min+(Rad*2.f) };
  return Result;
}

link_internal v2
GetRadius(rect2 *Rect)
{
  v2 Dim = Rect->Max - Rect->Min;
  v2 Result = Dim/2;
  return Result;
}

link_internal v2
GetCenter(rect2 *Rect)
{
  v2 Rad = GetRadius(Rect);
  v2 Result = Rect->Min + Rad;
  return Result;
}

link_internal rect2
RectMinDim(v2 Min, v2 Dim)
{
  rect2 Result = { Min, Min + Dim };
  return Result;
}

link_internal b32
IsInside(v2 P, rect2 Rect)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

