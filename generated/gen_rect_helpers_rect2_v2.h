link_internal rect2
RectMinMax(v2 Min, v2 Max)
{
  rect2 Result = { .Min = Min, .Max = Max };
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

