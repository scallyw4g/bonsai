
enum voxel_occupancy
{
  VoxelOccupancy_Empty = 0,
  VoxelOccupancy_Filled = 1,
};


link_internal s32
GetOccupancyBit(u64 *Occupancy, s32 Index)
{
  s32 MaskIndex = Index/64;
  s32 BitIndex = Index%64;
  s32 Result = (Occupancy[MaskIndex] >> BitIndex) & 1;
  return Result;
}


link_internal void
SetOccupancyBit(world_chunk *Chunk, s32 Index, s32 BitValue);

link_internal s32
GetOccupancyBit(world_chunk *Chunk, s32 Index);
