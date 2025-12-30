link_internal s32
GetOccupancyBit(world_chunk *Chunk, s32 Index)
{
  /* Assert(Index < Volume(Chunk->Dim)); */
  s32 Result = GetOccupancyBit(Chunk->Occupancy, Index);
  return Result;
}

link_internal void
SetOccupancyBit(u64 *Occupancy, v3i Dim, v3i VoxelP, s32 BitValue)
{
  Assert(VoxelP.x < Dim.x);
  Assert(VoxelP.y < Dim.y);
  Assert(VoxelP.z < Dim.z);

  s32 yOff = VoxelP.y;
  s32 zOff = VoxelP.z * Dim.y;

  s32 u64Index = yOff + zOff;
  s32 BitIndex = VoxelP.x;

  Occupancy[u64Index] &= ~(u64(1) << BitIndex); // Unconditionally knock out the bit
  Occupancy[u64Index] |=  (u64(BitValue) << BitIndex); // Set new value.. 0 just does nothing
}

link_internal void
SetOccupancyBit(world_chunk *Chunk, v3i VoxelP, s32 BitValue)
{
  SetOccupancyBit(Chunk->Occupancy, Chunk->Dim, VoxelP, BitValue);
}

link_internal void
SetOccupancyMask(world_chunk *Chunk, s32 Index, u64 MaskValue)
{
  /* s32 MaskIndex = Index/64; */
  Chunk->Occupancy[Index] = MaskValue;
}
