// NOTE(Jesse): These have to be here because they rely on WORLD_CHUNK_DIM.
// FeelsBadMan.

inline canonical_position
Canonicalize(canonical_position P )
{
  canonical_position Result = Canonicalize(WORLD_CHUNK_DIM, P);
  return Result;
}

inline float
Length( canonical_position P )
{
  v3 Offset = P.Offset + (P.WorldP * WORLD_CHUNK_DIM);
  float Result = sqrt(LengthSq(Offset));
  return Result;
}

canonical_position
Lerp(r32 t, canonical_position p1, canonical_position p2)
{
  Assert(t<=1);
  Assert(t>=0);

  canonical_position Result;
  Result.Offset = (1.0f-t)*p1.Offset + t*p2.Offset;

  NotImplemented;
  // This is buggy I believe.  We should be getting full double world position
  // coordinates, doing a lerp on those, then recanonicalizing.
  // Result.WorldP = (1.0f-t)*p1.WorldP + t*p2.WorldP;

  Result = Canonicalize(Result);

  return Result;
}

