

link_internal world_chunk_file_header
MakeWorldChunkFileHeader(world_chunk *Chunk)
{
  world_chunk_file_header Result = {};

  Result.WHNK = WorldChunkFileTag_WHNK;
  Result.Version = 1;
  Result.Checksum = 0xdeadbeef;

  Result.VoxelElementCount = Volume(Chunk);
  Result.VoxelElementSize  = (u32)sizeof(voxel);

  untextured_3d_geometry_buffer *Mesh = Chunk->Mesh;

  Result.MeshElementCount = Mesh->At;

  Result.VertexElementSize = (u32)sizeof(v3);
  Result.ColorElementSize  = (u32)sizeof(v4);
  Result.NormalElementSize = (u32)sizeof(v3);

  return Result;
}

#if 0
link_internal _3d_geometry_buffer_sizes
GetSizeRequirements(untextured_3d_geometry_buffer *Mesh)
{
  _3d_geometry_buffer_sizes Result = {};
  Result.TotalElements = Mesh->At;

  Result.VertElementSize = (u32)sizeof(v3);
  Result.VertByteCount = Result.TotalElements*Result.VertElementSize;

  Result.ColorElementSize = (u32)sizeof(v4);
  Result.ColorByteCount = Result.TotalElements*Result.ColorElementSize;

  Result.NormalElementSize = (u32)sizeof(v3);
  Result.NormalByteCount = Result.TotalElements*Result.NormalElementSize;

  Result.TotalBytes = Result.VertByteCount + Result.ColorByteCount + Result.NormalByteCount;

  return Result;
}
#endif

link_internal b32
Serialize(native_file *File, untextured_3d_geometry_buffer *Mesh, world_chunk_file_header *FileHeader)
{
  b32 Result = True;

  u64 TotalElements = FileHeader->MeshElementCount;

  u32 VertElementSize = FileHeader->VertexElementSize;
  u64 VertByteCount = VertElementSize * TotalElements;

  u32 ColorElementSize = FileHeader->ColorElementSize;
  u64 ColorByteCount = ColorElementSize * TotalElements;

  u32 NormalElementSize = FileHeader->NormalElementSize;
  u64 NormalByteCount = NormalElementSize * TotalElements;

  u32 Tag = WorldChunkFileTag_VERT;
  Result &= WriteToFile(File, Tag);
  Result &= WriteToFile(File, (u8*)Mesh->Verts, VertByteCount);

  Tag = WorldChunkFileTag_COLO;
  Result &= WriteToFile(File, Tag);
  Result &= WriteToFile(File, (u8*)Mesh->Colors, ColorByteCount);

  Tag = WorldChunkFileTag_NORM;
  Result &= WriteToFile(File, Tag);
  Result &= WriteToFile(File, (u8*)Mesh->Normals, NormalByteCount);

  return Result;
}

#if 0
link_internal void*
ReadBuffer(native_file *File, umm ByteCount, memory_arena *Memory)
{
  void* Result = 0;
  NotImplemented;
  return Result;
}
#endif

link_internal u64
Read_u64(native_file *File)
{
  u64 Result = 0;
  Ensure(ReadBytesIntoBuffer(File, sizeof(u64), (u8*)&Result));
  return Result;
}

link_internal u32
Read_u32(native_file *File)
{
  u32 Result = 0;
  Ensure(ReadBytesIntoBuffer(File, sizeof(u32), (u8*)&Result));
  return Result;
}

link_internal untextured_3d_geometry_buffer*
DeserializeMesh(native_file *File, world_chunk_file_header *Header, untextured_3d_geometry_buffer *Result)
{
  u64 TotalElements = Header->MeshElementCount;

  Assert(Result->At == 0);
  Assert(TotalElements == Result->End);
  Result->At = Result->End;

  //
  // Vertex data
  //
  u32 Tag = Read_u32(File);
  Assert(Tag ==  WorldChunkFileTag_VERT);

  u32 VertElementSize = Header->VertexElementSize;
  Assert(VertElementSize == (u32)sizeof(v3));
  ReadBytesIntoBuffer(File, VertElementSize*TotalElements, (u8*)Result->Verts);

  //
  // Color data
  Tag = Read_u32(File);
  Assert(Tag == WorldChunkFileTag_COLO);

  u32 ColorElementSize = Header->ColorElementSize;
  Assert(ColorElementSize == (u32)sizeof(v4));
  ReadBytesIntoBuffer(File, ColorElementSize*TotalElements, (u8*)Result->Colors);

  //
  // Normal data
  Tag = Read_u32(File);
  Assert(Tag == WorldChunkFileTag_NORM);

  u32 NormalElementSize = Header->NormalElementSize;
  Assert(NormalElementSize == (u32)sizeof(v3));
  ReadBytesIntoBuffer(File, NormalElementSize*TotalElements, (u8*)Result->Normals);

  return Result;
}

global_variable counted_string
Global_AssetPrefixPath = CSz("");

link_internal world_chunk_file_header
ReadWorldChunkFileHeader(native_file *File)
{
  world_chunk_file_header Result = {};

  Ensure( ReadBytesIntoBuffer(File, sizeof(Result), (u8*)&Result) );

  Assert( Result.WHNK == WorldChunkFileTag_WHNK );
  Assert( Result.Version == 1 );
  Assert( Result.Checksum == 0xdeadbeef );

  Assert( Result.VoxelElementSize  == sizeof(voxel) );
  Assert( Result.VertexElementSize == sizeof(v3) );
  Assert( Result.ColorElementSize  == sizeof(v4) );
  Assert( Result.NormalElementSize == sizeof(v3) );

  return Result;
}

link_internal void
DeserializeChunk(const char *zAssetPath, world_chunk *Result)
{
  native_file AssetFile = OpenFile(zAssetPath, "r");
  world_chunk_file_header Header = ReadWorldChunkFileHeader(&AssetFile);

  u32 Tag = Read_u32(&AssetFile);
  Assert( Tag ==  WorldChunkFileTag_VOXD );
  umm VoxByteCount = Header.VoxelElementCount * Header.VoxelElementSize;
  ReadBytesIntoBuffer(&AssetFile, VoxByteCount, (u8*)Result->Data->Voxels);

  if (Header.MeshElementCount)
  {
    DeserializeMesh(&AssetFile, &Header, Result->Mesh);
  }

  CloseFile(&AssetFile);
}

link_internal b32
SerializeChunk(world_chunk *Chunk, counted_string AssetPath)
{
  b32 Result = True;

  auto WorldP = Chunk->WorldP;
  counted_string Filename = FormatCountedString(TranArena, CSz("%S/world_chunk_%u_%u_%u"), AssetPath, WorldP.x, WorldP.y, WorldP.z);

  native_file File = OpenFile(Filename, "w");

  world_chunk_file_header FileHeader = MakeWorldChunkFileHeader(Chunk);

  Result &= WriteToFile(&File, (u8*)&FileHeader, sizeof(FileHeader));

  {
    u64 VoxByteCount = FileHeader.VoxelElementCount * FileHeader.VoxelElementSize;

    u32 Tag = WorldChunkFileTag_VOXD;
    Result &= WriteToFile(&File, Tag);
    Result &= WriteToFile(&File, (u8*)Chunk->Data->Voxels, VoxByteCount);
  }

  Result &= Serialize(&File, Chunk->Mesh, &FileHeader);

  CloseFile(&File);

  return Result;
}

