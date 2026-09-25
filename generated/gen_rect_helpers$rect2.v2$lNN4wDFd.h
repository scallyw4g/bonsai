// callsite
// external/bonsai_stdlib/src/rect.h:147:0

// def (gen_rect_helpers)
// external/bonsai_stdlib/src/rect.h:58:0
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

link_internal rect2
RectCenterRad(v2 Center, v2 Rad)
{
  auto Min = Center-Rad;
  auto Max = Center+Rad;
  rect2 Result = RectMinMax(Min, Max);
  return Result;
}

link_internal b32
IsInside(v2 P, rect2 Rect)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

// TODO(Jesse)(duplicate): make Contains primal and get rid of IsInside ..?
link_internal b32
Contains(rect2 Rect, v2 P)
{
  b32 Result = IsInside(P, Rect);
  return Result;
}


