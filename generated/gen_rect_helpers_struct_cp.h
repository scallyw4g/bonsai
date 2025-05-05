// src/engine/canonical_position.h:273:0

link_internal rect3cp
RectMinMax(cp Min, cp Max)
{
  rect3cp Result = { .Min = Min, .Max = Max };
  return Result;
}

link_internal rect3cp
RectMinRad(cp Min, cp Rad)
{
  rect3cp Result = { .Min = Min, .Max = Min+(Rad*2.f) };
  return Result;
}

link_internal cp
GetRadius(rect3cp *Rect)
{
  cp Dim = Rect->Max - Rect->Min;
  cp Result = Dim/2;
  return Result;
}

link_internal cp
GetCenter(rect3cp *Rect)
{
  cp Rad = GetRadius(Rect);
  cp Result = Rect->Min + Rad;
  return Result;
}

link_internal rect3cp
RectMinDim(cp Min, cp Dim)
{
  rect3cp Result = { Min, Min + Dim };
  return Result;
}

link_internal rect3cp
RectCenterRad(cp Center, cp Rad)
{
  auto Min = Center-Rad;
  auto Max = Center+Rad;
  rect3cp Result = RectMinMax(Min, Max);
  return Result;
}

link_internal b32
IsInside(cp P, rect3cp Rect)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

// TODO(Jesse)(duplicate): make Contains primal and get rid of IsInside ..?
link_internal b32
Contains(rect3cp Rect, cp P)
{
  b32 Result = IsInside(P, Rect);
  return Result;
}


