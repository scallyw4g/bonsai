// callsite
// external/bonsai_stdlib/src/rect.h:154:0

// def (gen_rect_helpers)
// external/bonsai_stdlib/src/rect.h:58:0
link_internal rect3i
RectMinMax(v3i Min, v3i Max)
{
  rect3i Result = { .Min = Min, .Max = Max };
  return Result;
}

link_internal rect3i
RectMinRad(v3i Min, v3i Rad)
{
  rect3i Result = { .Min = Min, .Max = Min+(Rad*2.f) };
  return Result;
}

link_internal v3i
GetRadius(rect3i *Rect)
{
  v3i Dim = Rect->Max - Rect->Min;
  v3i Result = Dim/2;
  return Result;
}

link_internal v3i
GetCenter(rect3i *Rect)
{
  v3i Rad = GetRadius(Rect);
  v3i Result = Rect->Min + Rad;
  return Result;
}

link_internal rect3i
RectMinDim(v3i Min, v3i Dim)
{
  rect3i Result = { Min, Min + Dim };
  return Result;
}

link_internal rect3i
RectCenterRad(v3i Center, v3i Rad)
{
  auto Min = Center-Rad;
  auto Max = Center+Rad;
  rect3i Result = RectMinMax(Min, Max);
  return Result;
}

link_internal b32
IsInside(v3i P, rect3i Rect)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

// TODO(Jesse)(duplicate): make Contains primal and get rid of IsInside ..?
link_internal b32
Contains(rect3i Rect, v3i P)
{
  b32 Result = IsInside(P, Rect);
  return Result;
}


