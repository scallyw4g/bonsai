link_internal rect3cp
RectMinMax(cp Min, cp Max)
{
  rect3cp Result = { .Min = Min, .Max = Max };
  return Result;
}

link_internal rect3cp
RectMinDim(cp Min, cp Dim)
{
  rect3cp Result = { Min, Min + Dim };
  return Result;
}

link_internal b32
IsInside(cp P, rect3cp Rect)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

