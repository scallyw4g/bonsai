// Used when loading .vox files
//

struct vox_voxel
{
  voxel V;
  v3i Offset;

  vox_voxel(s32 x, s32 y, s32 z, u8 w)
  {
    this->Offset.x = x;
    this->Offset.y = y;
    this->Offset.z = z;

    this->V.Color = w;

    /* this->V.Flags = Voxel_Empty; */
    this->V.Transparency = 0;
  }

  vox_voxel(voxel *V_in, voxel_position Offset_in)
  {
    this->V = *V_in;
    this->Offset = Offset_in;
  }
};

struct vox_loader_buffer
{
  chunk_data *ChunkData[4*4*4];
};

poof(block_array(vox_loader_buffer, {8}))
#include <generated/block_array_vox_loader_buffer_688856411.h>
