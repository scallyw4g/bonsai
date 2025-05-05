inline r32
Length( canonical_position P, chunk_dimension WorldChunkDim)
{
  v3 Offset = P.Offset + (P.WorldP * WorldChunkDim);
  r32 Result = SquareRoot(LengthSq(Offset));
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

// Canonical add
link_internal cp
CAdd(world *World, cp P1, cp P2)
{
  cp Result = {
    P1.Offset + P2.Offset,
    P1.WorldP + P2.WorldP
  };

  Canonicalize(World, &Result);
  return Result;
}

// Canonical add
link_internal cp
CSub(world *World, cp P1, cp P2)
{
  cp Result = {
    P1.Offset - P2.Offset,
    P1.WorldP - P2.WorldP
  };

  Canonicalize(World, &Result);
  return Result;
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
  Result.Min = SimSpaceToCanonical(World, V3(Rect->Min));
  Result.Max = SimSpaceToCanonical(World, V3(Rect->Max));
  return Result;
}

link_internal cp
GetCenter(world *World, rect3cp *Rect)
{
  cp Rad = GetRadius(Rect);
  cp Result = CAdd(World, Rect->Min, Rad);
  return Result;
}
