// callsite
// src/engine/voxel_synthesis.h:85:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct voxel_synth_tile_buffer
{
  umm Count;
  voxel_synth_tile *Start; poof(@array_length(Element->Count))
};

link_internal voxel_synth_tile_buffer
VoxelSynthTileBuffer( umm ElementCount, memory_arena* Memory);

link_internal voxel_synth_tile_buffer
VoxelSynthTileBuffer( voxel_synth_tile *Start, umm ElementCount)
{
  voxel_synth_tile_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(voxel_synth_tile_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(voxel_synth_tile_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(voxel_synth_tile_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(voxel_synth_tile_buffer *Buf)
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
TryGetPtr(voxel_synth_tile_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline voxel_synth_tile *
Get(voxel_synth_tile_buffer *Buf, umm Index)
{
  voxel_synth_tile *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal voxel_synth_tile_buffer
VoxelSynthTileBuffer( umm ElementCount, memory_arena* Memory)
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


