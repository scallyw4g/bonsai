
enum world_chunk_file_tag
{
  WorldChunkFileTag_VOXD = 'DXOV', // Voxel data

  WorldChunkFileTag_MESH = 'HSEM', // Mesh data
  WorldChunkFileTag_VERT = 'TREV',
  WorldChunkFileTag_COLO = 'OLOC',
  WorldChunkFileTag_NORM = 'MRON',
};


link_internal b32
Serialize(native_file *File, untextured_3d_geometry_buffer *Mesh)
{
  b32 Result = True;
  u64 TotalElements = Mesh->At;

  u32 VertElementSize = (u32)sizeof(v3);
  u64 VertByteCount = TotalElements*VertElementSize;

  u32 ColorElementSize = (u32)sizeof(v4);
  u64 ColorByteCount = TotalElements*ColorElementSize;

  u32 NormalElementSize = (u32)sizeof(v3);
  u64 NormalByteCount = TotalElements*NormalElementSize;

  u64 TotalBytes = VertByteCount + ColorByteCount + NormalByteCount;

  u32 Header = WorldChunkFileTag_MESH;
  Result &= WriteToFile(File, Header);
  Result &= WriteToFile(File, TotalElements);

  Header = WorldChunkFileTag_VERT;
  Result &= WriteToFile(File, Header);
  Result &= WriteToFile(File, VertElementSize);
  Result &= WriteToFile(File, (u8*)Mesh->Verts, VertByteCount);

  Header = WorldChunkFileTag_COLO;
  Result &= WriteToFile(File, Header);
  Result &= WriteToFile(File, ColorElementSize);
  Result &= WriteToFile(File, (u8*)Mesh->Colors, ColorByteCount);

  Header = WorldChunkFileTag_NORM;
  Result &= WriteToFile(File, Header);
  Result &= WriteToFile(File, NormalElementSize);
  Result &= WriteToFile(File, (u8*)Mesh->Normals, NormalByteCount);

  return Result;
}

link_internal void*
ReadBuffer(native_file *File, umm ByteCount, memory_arena *Memory)
{
  void* Result = 0;
  NotImplemented;
  return Result;
}

link_internal u64
Read_u64(native_file *File)
{
  u64 Result = 0;
  NotImplemented;
  return Result;
}

link_internal u32
Read_u32(native_file *File)
{
  u32 Result = 0;
  NotImplemented;
  return Result;
}

link_internal untextured_3d_geometry_buffer*
DeserializeMesh(native_file *File, memory_arena *Memory)
{
  untextured_3d_geometry_buffer *Result = Allocate(untextured_3d_geometry_buffer, Memory, 1);

  u32 Header = Read_u32(File);
  Assert(Header == WorldChunkFileTag_MESH);

  u32 TotalElements = Read_u32(File);

  //
  // Vertex data
  Header = Read_u32(File);
  Assert(Header ==  WorldChunkFileTag_VERT);

  u32 VertElementSize = Read_u32(File);
  Assert(VertElementSize == (u32)sizeof(v3));
  Result->Verts = (v3*)ReadBuffer(File, VertElementSize*TotalElements, Memory);

  //
  // Color data
  Header = Read_u32(File);
  Assert(Header == WorldChunkFileTag_COLO);

  u32 ColorElementSize = Read_u32(File);
  Assert(ColorElementSize == (u32)sizeof(v4));
  Result->Colors = (v4*)ReadBuffer(File, ColorElementSize*TotalElements, Memory);

  //
  // Normal data
  Header = Read_u32(File);
  Assert(Header == WorldChunkFileTag_NORM);

  u32 NormalElementSize = Read_u32(File);
  Assert(NormalElementSize == (u32)sizeof(v3));
  Result->Normals = (v3*)ReadBuffer(File, NormalElementSize*TotalElements, Memory);

  return Result;
}

link_internal world_chunk *
DeserializeChunk(counted_string AssetPath, world_position WorldP, memory_arena *Memory)
{
  world_chunk *Result = Allocate(world_chunk, Memory, 1);
  counted_string Filename = FormatCountedString(TranArena, CSz("%S/world_chunk_%u_%u_%u"), AssetPath, WorldP.x, WorldP.y, WorldP.z);

  native_file File = OpenFile(Filename, "w");

  u32 WRLD = Read_u32(&File);
  u32 CHNK = Read_u32(&File);
  Assert( WRLD == 'DLRW' );
  Assert( CHNK == 'KNHC' );

  u32 VersionNumber = Read_u32(&File);
  u64 Checksum = Read_u64(&File);
  Assert( VersionNumber == 0 );
  Assert( Checksum == 0xdeadbeef );


  {
    u32 Header = Read_u32(&File);
    Assert( Header ==  WorldChunkFileTag_VOXD );

    u32 VoxelCount = Read_u32(&File);
    u32 VoxSize = Read_u32(&File);
    Assert(VoxSize == (u32)sizeof(voxel));

    u32 VoxByteCount = Read_u32(&File);

    Result->Data->Voxels = (voxel*)ReadBuffer(&File, VoxByteCount, Memory);
  }

  Result->Mesh = DeserializeMesh(&File, Memory);

  return Result;
}

link_internal void
SerializeChunk(world_chunk *Chunk, counted_string AssetPath)
{
  auto WorldP = Chunk->WorldP;
  counted_string Filename = FormatCountedString(TranArena, CSz("%S/world_chunk_%u_%u_%u"), AssetPath, WorldP.x, WorldP.y, WorldP.z);

  native_file File = OpenFile(Filename, "w");

  Ensure( WriteToFile(&File, CSz("WRLDCHNK")) );

  u32 VersionNumber = 0;
  u64 Checksum = 0xdeadbeef;
  Ensure( WriteToFile(&File, VersionNumber) );
  Ensure( WriteToFile(&File, Checksum) );


  {
    u32 Header = WorldChunkFileTag_VOXD;

    u32 VoxelCount = Volume(Chunk);
    u32 VoxSize = (u32)sizeof(voxel);
    u32 VoxByteCount = VoxelCount*VoxSize;

    Ensure( WriteToFile(&File, Header) );
    Ensure( WriteToFile(&File, VoxelCount) );
    Ensure( WriteToFile(&File, VoxSize) );
    Ensure( WriteToFile(&File, (u8*)Chunk->Data->Voxels, VoxByteCount) );
  }

  Ensure( Serialize(&File, Chunk->Mesh) );

}

