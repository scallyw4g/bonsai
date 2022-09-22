
//
// world_chunk_x_x_x file layout
//
// u64 : 'WRLDCHNK'
// u32 : version number
// u64 : 0xdeadbeef <-- Future home of a checksum
//
// u32     : 'VOXD'
// u32     : number of voxels
// u32     : size of voxel
// voxel[] : voxels
//
//
// u32      : 'MESH'
// u64      : Number of elements of each (VERT, COLO, NORM) array
//
// u32      : 'VERT'
// u32      : element size
// vertex[] : elements
//
// u32      : 'COLO'
// u32      : element size
// colors[] : elements
//
// u32      : 'NORM'
// u32      : element size
// normls[] : elements
//


struct asset_pack
{
  counted_string BAPFilename;
};

