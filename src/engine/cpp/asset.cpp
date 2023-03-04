

link_internal world_chunk_file_header_v2
MakeWorldChunkFileHeader_v2(world_chunk *Chunk)
{
  world_chunk_file_header_v2 Result = {};

  Result.WHNK = WorldChunkFileTag_WHNK;
  Result.Version = 2;
  Result.Checksum = 0xdeadbeef;

  Result.VoxelElementCount        = Volume(Chunk);
  Result.StandingSpotElementCount = (u32)Chunk->StandingSpots.Count;
  Result.MeshElementCount         = Chunk->Meshes.E[MeshIndex_Main]->At;

  Result.VertexElementSize        = (u32)sizeof(v3);
  Result.ColorElementSize         = (u32)sizeof(v4);
  Result.NormalElementSize        = (u32)sizeof(v3);
  Result.StandingSpotElementSize  = (u32)sizeof(v3i);
  Result.VoxelElementSize         = (u32)sizeof(voxel);

  return Result;
}
link_internal world_chunk_file_header_v1
MakeWorldChunkFileHeader_v1(world_chunk *Chunk)
{
  world_chunk_file_header_v1 Result = {};

  Result.WHNK = WorldChunkFileTag_WHNK;
  Result.Version = 1;
  Result.Checksum = 0xdeadbeef;

  Result.VoxelElementCount = Volume(Chunk);
  Result.VoxelElementSize  = (u32)sizeof(voxel);

  Result.MeshElementCount = Chunk->Meshes.E[MeshIndex_Main]->At;

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
SerializeMesh(native_file *File, untextured_3d_geometry_buffer *Mesh, world_chunk_file_header_v2 *FileHeader)
{
  b32 Result = True;

  u64 TotalElements = FileHeader->MeshElementCount;

  if (TotalElements)
  {
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
  }

  return Result;
}

link_internal b32
SerializeMesh(native_file *File, untextured_3d_geometry_buffer *Mesh, world_chunk_file_header_v1 *FileHeader)
{
  b32 Result = True;

  u64 TotalElements = FileHeader->MeshElementCount;

  if (TotalElements)
  {
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
  }

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
  Assert(TotalElements < Result->End);
  Result->At = (u32)TotalElements;

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

  Result->Timestamp = __rdtsc();

  return Result;
}

global_variable counted_string
Global_AssetPrefixPath = CSz("");

link_internal world_chunk_file_header_v2
ReadWorldChunkFileHeader_v2(native_file *File)
{
  world_chunk_file_header_v2 Result = {};

  Ensure( ReadBytesIntoBuffer(File, sizeof(Result), (u8*)&Result) );

  Assert( Result.WHNK == WorldChunkFileTag_WHNK );
  Assert( Result.Version == 2 );
  Assert( Result.Checksum == 0xdeadbeef );

  Assert( Result.VertexElementSize == sizeof(v3) );
  Assert( Result.ColorElementSize  == sizeof(v4) );
  Assert( Result.NormalElementSize == sizeof(v3) );
  Assert( Result.StandingSpotElementSize  == sizeof(voxel_position) );
  Assert( Result.VoxelElementSize  == sizeof(voxel) );

  return Result;
}

link_internal world_chunk_file_header_v1
ReadWorldChunkFileHeader_v1(native_file *File)
{
  world_chunk_file_header_v1 Result = {};

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

link_internal u32
ReadWorldChunkVersion(native_file *AssetFile )
{
  u32 WHNK = Read_u32(AssetFile);
  Assert(WHNK == WorldChunkFileTag_WHNK);

  u32 Version = Read_u32(AssetFile);

  // In V1 of the header the WHNK is 64 bits, so if we get a 0 for the version
  // number it was the second 4 bytes of that field.  This should be removed
  // once we have no v1 chunks left in development
  if (Version == 0) { Version = Read_u32(AssetFile); }

  return Version;
}

link_internal void
DeserializeChunk(native_file *AssetFile, world_chunk *Result, mesh_freelist *MeshFreelist, memory_arena *PermMemory)
{
  world_chunk_file_header Header = {}; //ReadWorldChunkFileHeader(&AssetFile);
  Header.Version = ReadWorldChunkVersion(AssetFile);
  Rewind(AssetFile);

  switch(Header.Version)
  {
    case 1:
    {
      world_chunk_file_header_v1 v1Header = ReadWorldChunkFileHeader_v1(AssetFile);

      Header.WHNK = SafeTruncateToU32(v1Header.WHNK); // WorldChunkFileTag_WHNK
      Header.Version = v1Header.Version;
      Header.Checksum = v1Header.Checksum;

      Header.VoxelElementCount = v1Header.VoxelElementCount;
      Header.VoxelElementSize = SafeTruncateU8(v1Header.VoxelElementSize);

      Header.MeshElementCount = v1Header.MeshElementCount;

      Header.VertexElementSize = SafeTruncateU8(v1Header.VertexElementSize);
      Header.ColorElementSize = SafeTruncateU8(v1Header.ColorElementSize);
      Header.NormalElementSize = SafeTruncateU8(v1Header.NormalElementSize);
    } break;

    case 2:
    {
      world_chunk_file_header_v2 v2Header = ReadWorldChunkFileHeader_v2(AssetFile);

      Header.WHNK = v2Header.WHNK; // WorldChunkFileTag_WHNK
      Header.Version = v2Header.Version;
      Header.Checksum = v2Header.Checksum;

      Header.VoxelElementCount = v2Header.VoxelElementCount;
      Header.StandingSpotElementCount = v2Header.StandingSpotElementCount;

      Header.MeshElementCount = v2Header.MeshElementCount;

      Header.VertexElementSize = v2Header.VertexElementSize;
      Header.ColorElementSize = v2Header.ColorElementSize;
      Header.NormalElementSize = v2Header.NormalElementSize;
      Header.StandingSpotElementSize = v2Header.StandingSpotElementSize;

      Header.VoxelElementSize = v2Header.VoxelElementSize;
    } break;

    default:
    {
      SoftError("Invalid chunk version encountered (%u)", Header.Version);
    } break;
  }

  u32 Tag = Read_u32(AssetFile);
  Assert( Tag ==  WorldChunkFileTag_VOXD );
  umm VoxByteCount = Header.VoxelElementCount * Header.VoxelElementSize;
  ReadBytesIntoBuffer(AssetFile, VoxByteCount, (u8*)Result->Voxels);

  Result->FilledCount = Header.VoxelElementCount;

  if (Header.MeshElementCount)
  {
    /* Result->SelectedMeshes |= MeshBit_Main; */
    /* Result->Mesh = GetMeshForChunk(MeshFreelist, PermMemory); */
    untextured_3d_geometry_buffer *Mesh = GetMeshForChunk(MeshFreelist, PermMemory);
    DeserializeMesh(AssetFile, &Header, Mesh);
    Ensure( AtomicReplaceMesh(&Result->Meshes, MeshBit_Main, Mesh, Mesh->Timestamp) == 0);
  }

  if (Header.StandingSpotElementCount)
  {
    u64 TotalElements = Header.StandingSpotElementCount;
    Result->StandingSpots = V3iBuffer(TotalElements, PermMemory);

    DebugLine("Standing Spots (%u)", TotalElements);

    //
    // SPOT data
    //
    Tag = Read_u32(AssetFile);
    Assert(Tag ==  WorldChunkFileTag_SPOT);

    u32 SpotElementSize = Header.StandingSpotElementSize;
    Assert(SpotElementSize == (u32)sizeof(v3));
    umm ByteCount = SpotElementSize*TotalElements;
    ReadBytesIntoBuffer(AssetFile, ByteCount, (u8*)Result->StandingSpots.Start);
  }

}

link_internal b32
SerializeChunk(world_chunk *Chunk, counted_string AssetPath)
{
  b32 Result = True;

  auto WorldP = Chunk->WorldP;
  counted_string Filename = FormatCountedString(TranArena, CSz("%S/world_chunk_%u_%u_%u"), AssetPath, WorldP.x, WorldP.y, WorldP.z);

  native_file File = OpenFile(Filename, "w+b");

  world_chunk_file_header_v2 FileHeader = MakeWorldChunkFileHeader_v2(Chunk);

  Result &= WriteToFile(&File, (u8*)&FileHeader, sizeof(FileHeader));

  {
    u64 VoxByteCount = FileHeader.VoxelElementCount * FileHeader.VoxelElementSize;

    u32 Tag = WorldChunkFileTag_VOXD;
    Result &= WriteToFile(&File, Tag);
    Result &= WriteToFile(&File, (u8*)Chunk->Voxels, VoxByteCount);
  }

  auto Mesh = TakeOwnershipSync(&Chunk->Meshes, MeshBit_Main);
  Result &= SerializeMesh(&File, Mesh, &FileHeader);
  ReleaseOwnership(&Chunk->Meshes, MeshBit_Main, Mesh);

  {
    DebugLine("Writing (%u) StandingSpots", FileHeader.StandingSpotElementCount);
    u64 StandingSpotByteCount = FileHeader.StandingSpotElementSize * FileHeader.StandingSpotElementCount;
    u32 Tag = WorldChunkFileTag_SPOT;
    Result &= WriteToFile(&File, Tag);
    Result &= WriteToFile(&File, (u8*)Chunk->StandingSpots.Start, StandingSpotByteCount);
  }

  CloseFile(&File);

  return Result;
}

