inline r32
Length( canonical_position P, chunk_dimension WorldChunkDim)
{
  v3 Offset = P.Offset + (P.WorldP * WorldChunkDim);
  r32 Result = (r32)sqrt(r64(LengthSq(Offset)));
  return Result;
}

inline v3
ToV3(canonical_position P, chunk_dimension WorldChunkDim)
{
  v3 Result = P.Offset + (P.WorldP*WorldChunkDim);
  return Result;
}

#if 1
canonical_position
Lerp(r32 t, canonical_position p1, canonical_position p2, chunk_dimension WorldChunkDim)
{
  Assert(t<=1);
  Assert(t>=0);

  v3 ToP2 = ToV3(p2 - p1, WorldChunkDim);

  canonical_position Result = Canonicalize(WorldChunkDim, p1 + (t*ToP2));
  return Result;
}
#endif

link_internal void
Canonicalize( world *World, canonical_position *P)
{
  *P = Canonicalize(World->ChunkDim, P->Offset, P->WorldP);
}

link_internal cp
SimSpaceToCanonical(world *World, v3 P)
{
  cp Result = Canonical_Position(World->ChunkDim, P, World->Center);
  return Result;
}

link_internal rect3cp
SimSpaceToCanonical(world *World, rect3i *Rect)
{
  rect3cp Result = {};
  Result.Min = Canonical_Position(World->ChunkDim, V3(Rect->Min), {});
  Result.Max = Canonical_Position(World->ChunkDim, V3(Rect->Max), {});
  return Result;
}

