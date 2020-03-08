inline r32
Length( canonical_position P, chunk_dimension WorldChunkDim)
{
  v3 Offset = P.Offset + (P.WorldP * WorldChunkDim);
  r32 Result = (r32)sqrt(LengthSq(Offset));
  return Result;
}

inline v3
ToV3(canonical_position P, chunk_dimension WorldChunkDim)
{
  v3 Result = P.Offset + (P.WorldP*WorldChunkDim);
  return Result;
}

canonical_position
Lerp(r32 t, canonical_position p1, canonical_position p2, chunk_dimension WorldChunkDim)
{
  Assert(t<=1);
  Assert(t>=0);

  v3 ToP2 = ToV3(p2 - p1, WorldChunkDim);

  canonical_position Result = Canonicalize(WorldChunkDim, p1 + (t*ToP2));
  return Result;
}

