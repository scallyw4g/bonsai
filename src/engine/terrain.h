
link_internal f32
MapNoiseValueToFinal(f32 Value)
{
  Assert(Value <= 1.f);
  Assert(Value >= 0.f);

  // NOTE(Jesse): Descending order so we can scan from the front and find the interval we care about
  /* v2 Points[] = */
  /* { */
  /*   {{1.0f, 1.0f }}, */
  /*   {{0.7f, 0.85f }}, */
  /*   {{0.6f, 0.5f }}, */
  /*   {{0.3f, 0.4f }}, */
  /*   {{0.2f, 0.1f }}, */
  /*   {{0.0f, 0.0f }}, */
  /* }; */

  v2 Points[] =
  {
    {{1.0f, 1.0f }},
    {{0.5f, 0.7f }},
    {{0.2f, 0.1f }},
    {{0.0f, 0.0f }},
  };


  b32 Hit = False;;
  r32 Result = Value;
  s32 PointCount = s32(ArrayCount(Points));
  RangeIterator(PointIndex, PointCount)
  {
    v2 *P = Points + PointIndex;
    if (Value > P->x)
    {
      Hit = True;
      Assert(PointIndex > 0);

      v2 *PNext = Points + PointIndex - 1;

      r32 Range = PNext->x - P->x;
      Assert(Range > 0.f);

      r32 t = (Value-P->x) / Range;
      Result = Lerp(t, P->y, PNext->y);
      Assert(Result >= 0.f);
      Assert(Result <= 1.f);

      break;
    }
  }

  Assert(Hit);
  Assert(Result <= 1.f);
  Assert(Result >= 0.f);
  return Smoothstep(Result);
}

