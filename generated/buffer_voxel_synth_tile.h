struct voxel_synth_tile_buffer
{
  voxel_synth_tile *Start;
  umm Count;
};

link_internal voxel_synth_tile_buffer
VoxelSynthTileBuffer(umm ElementCount, memory_arena* Memory)
{
  voxel_synth_tile_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( voxel_synth_tile, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate voxel_synth_tile_buffer of 0 length.");
  }

  return Result;
}

link_inline umm
AtElements(voxel_synth_tile_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(voxel_synth_tile_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline voxel_synth_tile *
GetPtr(voxel_synth_tile_buffer *Buf, umm Index)
{
  voxel_synth_tile *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline voxel_synth_tile *
Get(voxel_synth_tile_buffer *Buf, umm Index)
{
  voxel_synth_tile *Result = GetPtr(Buf, Index);
  return Result;
}

