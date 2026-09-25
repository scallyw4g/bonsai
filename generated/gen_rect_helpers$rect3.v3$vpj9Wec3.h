// callsite
// external/bonsai_stdlib/src/rect.h:151:0

// def (gen_rect_helpers)
// external/bonsai_stdlib/src/rect.h:58:0
link_internal rect3
RectMinMax(v3 Min, v3 Max)
{
  rect3 Result = { .Min = Min, .Max = Max };
  return Result;
}

link_internal rect3
RectMinRad(v3 Min, v3 Rad)
{
  rect3 Result = { .Min = Min, .Max = Min+(Rad*2.f) };
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

link_internal rect3
RectCenterRad(v3 Center, v3 Rad)
{
  auto Min = Center-Rad;
  auto Max = Center+Rad;
  rect3 Result = RectMinMax(Min, Max);
  return Result;
}

link_internal b32
IsInside(v3 P, rect3 Rect)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

// TODO(Jesse)(duplicate): make Contains primal and get rid of IsInside ..?
link_internal b32
Contains(rect3 Rect, v3 P)
{
  b32 Result = IsInside(P, Rect);
  return Result;
}


