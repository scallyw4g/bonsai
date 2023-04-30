link_internal voxel_synth_tile
VoxelSynthTile( u32 RuleId , u32 VoxelOffset , u64 HashValue , chunk_data SrcChunk  )
{
  voxel_synth_tile Reuslt = {
    .RuleId = RuleId,
    .VoxelOffset = VoxelOffset,
    .HashValue = HashValue,
    .SrcChunk = SrcChunk
  };
  return Reuslt;
}

