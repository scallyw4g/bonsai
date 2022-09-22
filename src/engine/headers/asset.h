

//
// world_chunk_x_x_x file layout

// -- Header

// name : type : description
//
// f1   : u64  : 'WHNK'
// f2   : u32  : version number
// f3   : u64  : 0xdeadbeef <-- Future home of a checksum
//
// f5   : u32  : voxel element count
// f6   : u32  : size of voxel element
//
// f7   : u64  : element count in each (VERT, COLO, NORM) array
// f8   : u32  : 'VERT' (vertex) element size
// f9   : u32  : 'COLO' (colors) element size
// f10  : u32  : 'NORM' (normls) element size

// -- Data
// -- Array lengths refer to values in the header by their 'name' value

// type       : bytes  : description
//
// u32        : 4      : 'VOXD'
// voxel[f5]  : f5*f6  : voxel elements data
//
// u32        : 4      : 'VERT'
// vertex[f7] : f7*f8  : vertex elements data
//
// u32        : 4      : 'COLO'
// colors[f7] : f7*f9  : color elements data
//
// u32        : 4      : 'NORM'
// normls[f7] : f7*f10 : normal elements data
//

enum world_chunk_file_tag
{
  WorldChunkFileTag_WHNK = 'KNHW', // world_chunk header

  WorldChunkFileTag_VOXD = 'DXOV',
  WorldChunkFileTag_VERT = 'TREV',
  WorldChunkFileTag_COLO = 'OLOC',
  WorldChunkFileTag_NORM = 'MRON',
};

struct world_chunk_file_header
{
  u64 WHNK; // WorldChunkFileTag_WHNK
  u32 Version;
  u64 Checksum;

  u32 VoxelElementCount;
  u32 VoxelElementSize;

  u64 MeshElementCount;

  u32 VertexElementSize;
  u32 ColorElementSize;
  u32 NormalElementSize;
};

struct asset
{
  chunk_data *Data;
  untextured_3d_geometry_buffer *Mesh;
};

poof(buffer(asset));
#include <generated/buffer_asset.h>

link_internal counted_string
GetAssetFilenameFor(counted_string AssetPath, world_position WorldP, memory_arena *Memory)
{
  counted_string Result = FormatCountedString(Memory, CSz("%S/world_chunk_%u_%u_%u"), AssetPath, WorldP.x, WorldP.y, WorldP.z);
  return Result;
}

