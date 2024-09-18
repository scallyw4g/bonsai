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
  /* Assert(BitValue == 0 || BitValue == 1); */
  s32 ByteIndex = Index/8;
  s32 BitIndex = Index%8;

  Chunk->Occupancy[ByteIndex] &= ~(1 << BitIndex); // Unconditionally knock out the bit
  Chunk->Occupancy[ByteIndex] |=  (BitValue << BitIndex); // Set new value.. 0 just does nothing

  /* s32 Test = GetOccupancyBit(Chunk, Index); */
  /* Assert(Test == BitValue); */
}

link_internal void
SetOccupancyByte(world_chunk *Chunk, s32 Index, u8 ByteValue)
{
  /* Assert(Index % 8 == 0); */
  s32 ByteIndex = Index/8;
  Chunk->Occupancy[ByteIndex] = ByteValue;
}
