link_internal rect3
RectMinMax(v3 Min, v3 Max)
{
  rect3 Result = { .Min = Min, .Max = Max };
  return Result;
}

link_internal v3
GetRadius(rect3 *Rect)
{
  v3 Dim = Rect->Max - Rect->Min;
  v3 Result = Dim/2;
  return Result;
}

link_internal v3
GetCenter(rect3 *Rect)
{
  v3 Rad = GetRadius(Rect);
  v3 Result = Rect->Min + Rad;
  return Result;
}

link_internal rect3
RectMinDim(v3 Min, v3 Dim)
{
  rect3 Result = { Min, Min + Dim };
  return Result;
}

link_internal b32
IsInside(v3 P, rect3 Rect)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

