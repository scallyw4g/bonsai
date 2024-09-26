link_internal s32
GetOccupancyBit(world_chunk *Chunk, s32 Index)
{
  /* Assert(Index < Volume(Chunk->Dim)); */
  s32 Result = GetOccupancyBit(Chunk->Occupancy, Index);
  return Result;
}

link_internal void
SetOccupancyBit(world_chunk *Chunk, s32 Index, s32 BitValue)
{
  s32 ByteIndex = Index/64;
  s32 BitIndex = Index%64;

  Chunk->Occupancy[ByteIndex] &= ~(u64(1) << BitIndex); // Unconditionally knock out the bit
  Chunk->Occupancy[ByteIndex] |=  (u64(BitValue) << BitIndex); // Set new value.. 0 just does nothing

  /* s32 Test = GetOccupancyBit(Chunk, Index); */
  /* Assert(Test == BitValue); */
}

link_internal void
SetOccupancyMask(world_chunk *Chunk, s32 Index, u64 MaskValue)
{
  s32 MaskIndex = Index/64;
  Chunk->Occupancy[MaskIndex] = MaskValue;
}
