link_internal rect3i
RectMinMax(v3i Min, v3i Max)
{
  rect3i Result = { .Min = Min, .Max = Max };
  return Result;
}

link_internal rect3i
RectMinDim(v3i Min, v3i Dim)
{
  rect3i Result = { Min, Min + Dim };
  return Result;
}

link_internal b32
IsInside(v3i P, rect3i Rect)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

