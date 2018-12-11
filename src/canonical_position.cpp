// NOTE(Jesse): These have to be here because they rely on WORLD_CHUNK_DIM.
// FeelsBadMan.

inline canonical_position
Canonicalize(canonical_position P )
{
  canonical_position Result = Canonicalize(WORLD_CHUNK_DIM, P);
  return Result;
}

inline r32
Length( canonical_position P )
{
  v3 Offset = P.Offset + (P.WorldP * WORLD_CHUNK_DIM);
  r32 Result = (r32)sqrt(LengthSq(Offset));
  return Result;
}

inline v3
ToV3(canonical_position P)
{
  v3 Result = P.Offset + (P.WorldP*WORLD_CHUNK_DIM);
  return Result;
}

canonical_position
Lerp(r32 t, canonical_position p1, canonical_position p2)
{
  Assert(t<=1);
  Assert(t>=0);

  v3 ToP2 = ToV3(p2 - p1);

  canonical_position Result = Canonicalize(p1 + (t*ToP2));
  return Result;
}

