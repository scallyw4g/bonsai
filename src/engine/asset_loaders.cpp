#include <engine/loaders/vox.cpp>
#include <engine/loaders/obj.cpp>
#include <engine/loaders/collada.cpp>

#if 0
link_internal world_chunk
DeserializeChunk(const char *zAssetPath, memory_arena *Memory, chunk_dimension WorldChunkDim)
{
  native_file AssetFile = OpenFile(zAssetPath, "r");
  world_chunk_file_header Header = ReadWorldChunkFileHeader(&AssetFile);

  world_chunk Result = {};
  AllocateWorldChunk(&Result, Memory, {}, WorldChunkDim);
  Result.Mesh = AllocateMesh(Memory, (u32)Header.MeshElementCount);

  u32 Tag = Read_u32(&AssetFile);
  Assert( Tag ==  WorldChunkFileTag_VOXD );
  umm VoxByteCount = Header.VoxelElementCount * Header.VoxelElementSize;
  ReadBytesIntoBuffer(&AssetFile, VoxByteCount, (u8*)Result.Data->Voxels);

  BuildEntityMesh(Result.Data, Result.Mesh, DefaultPalette, WorldChunkDim);

  CloseFile(&AssetFile);
  return Result;
}

#endif
