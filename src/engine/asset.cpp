

link_internal world_chunk_file_header_v2
MakeWorldChunkFileHeader_v2(world_chunk *Chunk)
{
  world_chunk_file_header_v2 Result = {};

  Result.WHNK = WorldChunkFileTag_WHNK;
  Result.Version = 2;
  Result.Checksum = 0xdeadbeef;

  Result.VoxelElementCount        = Volume(Chunk);
  Result.StandingSpotElementCount = (u32)AtElements(&Chunk->StandingSpots);

  if (HasMesh(&Chunk->Meshes, MeshBit_Lod0))
  {
    Result.MeshElementCount       = Chunk->Meshes.E[MeshIndex_Lod0]->At;
  }

  Result.VertexElementSize        = (u32)sizeof(v3);
  Result.ColorElementSize         = (u32)sizeof(v4);
  Result.NormalElementSize        = (u32)sizeof(v3);
  Result.StandingSpotElementSize  = (u32)sizeof(v3i);
  Result.VoxelElementSize         = (u32)sizeof(voxel);

  return Result;
}

link_internal world_chunk_file_header_v3
MakeWorldChunkFileHeader_v3(world_chunk *Chunk)
{
  world_chunk_file_header_v3 Result = {};

  Result.WHNK = WorldChunkFileTag_WHNK;
  Result.Version = 3;
  Result.Checksum = 0xdeadbeef;

  Result.VoxelElementCount        = Volume(Chunk);
  Result.StandingSpotElementCount = (u32)AtElements(&Chunk->StandingSpots);

  Result.Px = Chunk->WorldP.x;
  Result.Py = Chunk->WorldP.y;
  Result.Pz = Chunk->WorldP.z;

  /* if (HasMesh(&Chunk->Meshes, MeshBit_Lod0)) { Result.MeshElementCount       = Chunk->Meshes.E[MeshIndex_Lod0]->At; } */

  Result.VertexElementSize        = (u32)sizeof(v3);
  Result.NormalElementSize        = (u32)sizeof(v3);
  Result.MaterialElementSize      = (u32)sizeof(vertex_material);
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

  if (HasMesh(&Chunk->Meshes, MeshBit_Lod0))
  {
    Result.MeshElementCount = Chunk->Meshes.E[MeshIndex_Lod0]->At;
  }


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
SerializeMeshData(native_file *File, untextured_3d_geometry_buffer *Mesh)
{
  b32 Result = True;
#if 1

  umm TotalElements = umm(Mesh->At);

  umm VertByteCount = TotalElements*sizeof(v3);
  umm MatlByteCount = TotalElements*sizeof(vertex_material);
  umm NormalByteCount = TotalElements*sizeof(v3);

  u32 Tag = WorldChunkFileTag_VERT;
  Result &= WriteToFile(File, Tag);
  Result &= WriteToFile(File, (u8*)Mesh->Verts, VertByteCount);

  Tag = WorldChunkFileTag_MATL;
  Result &= WriteToFile(File, Tag);
  Result &= WriteToFile(File, (u8*)Mesh->Mat, MatlByteCount);

  Tag = WorldChunkFileTag_NORM;
  Result &= WriteToFile(File, Tag);
  Result &= WriteToFile(File, (u8*)Mesh->Normals, NormalByteCount);
#endif

  return Result;
}

link_internal b32
SerializeMesh(native_file *File, untextured_3d_geometry_buffer *Mesh)
{
  b32 Result = SerializeMeshData(File, Mesh);
  return Result;
}



link_internal untextured_3d_geometry_buffer*
DeserializeMesh(native_file *File, world_chunk_file_header_v2 *Header, untextured_3d_geometry_buffer *Result)
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
  

  // TODO(Jesse): Copy into temp buffer and scatter to material props
  NotImplemented;

  u32 ColorElementSize = Header->ColorElementSize;
  Assert(ColorElementSize == (u32)sizeof(v4));
  /* ReadBytesIntoBuffer(File, ColorElementSize*TotalElements, (u8*)Result->Colors); */

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

link_internal untextured_3d_geometry_buffer*
DeserializeMesh(u8_cursor *File, world_chunk_file_header *Header, untextured_3d_geometry_buffer *Result)
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
  Assert(Tag == WorldChunkFileTag_MATL);

  u32 MaterialElementSize = Header->MaterialElementSize;
  Assert(MaterialElementSize == (u32)sizeof(vertex_material));
  ReadBytesIntoBuffer(File, MaterialElementSize*TotalElements, (u8*)Result->Mat);

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

link_internal world_chunk_file_header_v3
ReadWorldChunkFileHeader_v3(u8_cursor *File)
{
  world_chunk_file_header_v3 Result = {};
  Ensure( ReadBytesIntoBuffer(File, sizeof(Result), (u8*)&Result) );

  Assert( Result.WHNK == WorldChunkFileTag_WHNK );
  Assert( Result.Version == 3 );
  Assert( Result.Checksum == 0xdeadbeef );

  Assert( Result.VertexElementSize       == sizeof(v3) );
  Assert( Result.MaterialElementSize     == sizeof(vertex_material) );
  Assert( Result.NormalElementSize       == sizeof(v3) );
  Assert( Result.StandingSpotElementSize == sizeof(voxel_position) );
  Assert( Result.VoxelElementSize        == sizeof(voxel) );


  return Result;
}

link_internal world_chunk_file_header_v2
ReadWorldChunkFileHeader_v2(u8_cursor *File)
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
ReadWorldChunkFileHeader_v1(u8_stream *File)
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

link_internal world_chunk_file_header
ReadWorldChunkFileHeader(u8_stream *File)
{
  return ReadWorldChunkFileHeader_v3(File);
}

link_internal u32
ReadWorldChunkVersion(u8_stream *Cursor )
{
  u32 WHNK = Read_u32(Cursor);
  Assert(WHNK == WorldChunkFileTag_WHNK);

  u32 Version = Read_u32(Cursor);

  // In V1 of the header the WHNK is 64 bits, so if we get a 0 for the version
  // number it was the second 4 bytes of that field.  This should be removed
  // once we have no v1 chunks left in development
  if (Version == 0) { Version = Read_u32(Cursor); }

  return Version;
}

link_internal void
DeserializeChunk(u8_stream *FileBytes, world_chunk *Result, tiered_mesh_freelist *MeshFreelist, memory_arena *PermMemory)
{
  TIMED_FUNCTION();

  world_chunk_file_header Header = ReadWorldChunkFileHeader(FileBytes);
  Assert(Header.Version == 3);

  Result->WorldP.x = Header.Px;
  Result->WorldP.y = Header.Py;
  Result->WorldP.z = Header.Pz;
  /* Header.Version = ReadWorldChunkVersion(FileBytes); */
  /* Rewind(FileBytes); */

  Assert(Header.VoxelElementCount == Volume(Result));

  u32 Tag = Read_u32(FileBytes);
  Assert( Tag ==  WorldChunkFileTag_VOXD );
  umm VoxByteCount = Header.VoxelElementCount * Header.VoxelElementSize;
  ReadBytesIntoBuffer(FileBytes, VoxByteCount, (u8*)Result->Voxels);

  Result->FilledCount = Header.VoxelElementCount;

  if (Header.StandingSpotElementCount)
  {
    u32 SpotElementSize = Header.StandingSpotElementSize;
    Assert(SpotElementSize == (u32)sizeof(v3));

    u64 TotalElements = Header.StandingSpotElementCount;

    Result->StandingSpots = V3iCursor(WORLD_CHUNK_STANDING_SPOT_COUNT, PermMemory);
    /* Result->StandingSpots = V3iCursor(TotalElements, PermMemory); */

    //
    // SPOT data
    //
    Tag = Read_u32(FileBytes);
    Assert(Tag ==  WorldChunkFileTag_SPOT);

    umm ByteCount = SpotElementSize*TotalElements;
    ReadBytesIntoBuffer(FileBytes, ByteCount, (u8*)Result->StandingSpots.Start);
    Result->StandingSpots.At = Result->StandingSpots.Start + Header.StandingSpotElementCount;
  }

  /* Tag = Read_u32(FileBytes); */
  /* Assert(Tag ==  WorldChunkFileTag_END); */

  Result->Flags = Chunk_VoxelsInitialized;

  /* DebugLine("Loaded Chunk : P (%d,%d,%d) Standing Spots (%d)", Result->WorldP.x, Result->WorldP.y, Result->WorldP.z, Header.StandingSpotElementCount); */
}

link_internal b32
SerializeChunk(world_chunk *Chunk, native_file *File)
{
  b32 Result = True;

  world_chunk_file_header_v3 FileHeader = MakeWorldChunkFileHeader_v3(Chunk);

  Result &= WriteToFile(File, (u8*)&FileHeader, sizeof(FileHeader));

  {
    u64 VoxByteCount = FileHeader.VoxelElementCount * FileHeader.VoxelElementSize;

    u32 Tag = WorldChunkFileTag_VOXD;
    Result &= WriteToFile(File, Tag);
    Result &= WriteToFile(File, (u8*)Chunk->Voxels, VoxByteCount);
  }

#if 0
  if (FileHeader.MeshElementCount)
  {
    auto Mesh = TakeOwnershipSync(&Chunk->Meshes, MeshBit_Lod0);
    Result &= SerializeMesh(File, Mesh);
    ReleaseOwnership(&Chunk->Meshes, MeshBit_Lod0, Mesh);
  }
#endif

  if (FileHeader.StandingSpotElementCount)
  {
    DebugLine("Writing (%u) StandingSpots", FileHeader.StandingSpotElementCount);
    u64 StandingSpotByteCount = FileHeader.StandingSpotElementSize * FileHeader.StandingSpotElementCount;
    u32 Tag = WorldChunkFileTag_SPOT;
    Result &= WriteToFile(File, Tag);
    Result &= WriteToFile(File, (u8*)Chunk->StandingSpots.Start, StandingSpotByteCount);
  }

  /* Result &= WriteToFile(File, (u32)WorldChunkFileTag_END); */

  return Result;
}

link_internal b32
SerializeChunk(world_chunk *Chunk, counted_string AssetPath)
{

  auto WorldP = Chunk->WorldP;
  counted_string Filename = GetAssetFilenameFor(AssetPath, WorldP, GetTranArena());

  native_file File = OpenFile(Filename, "w+b");
  b32 Result = SerializeChunk(Chunk, &File);
  CloseFile(&File);
  return Result;
}

link_internal void
FreeModelBuffer(heap_allocator *ModelMemory, model_buffer *Models)
{
  /* NotImplemented; */
  Leak("Implement model freeing!");
}

link_internal void
FreeAsset(engine_resources *Engine, asset *Asset)
{
  Assert(Asset->LoadState != AssetLoadState_Queued);
  u16 Generation = Asset->Id.Slot.Generation;
  FreeModelBuffer(&Engine->AssetMemory, &Asset->Models);

  Clear(Asset);
  Asset->Id.Slot.Generation = Generation + 1;
}

link_internal maybe_asset_slot
AllocateAssetSlot(engine_resources *Engine)
{
  u16 FinalAssetIndex   = INVALID_ASSET_INDEX;

  u64 LruAssetFrame     = u64_MAX;
  u16 LruAssetIndex     = INVALID_ASSET_INDEX;

  RangeIterator_t(u16, TestAssetIndex, ASSET_TABLE_COUNT)
  {
    asset *Asset = Engine->AssetTable + TestAssetIndex;

    if (Asset->LoadState != AssetLoadState_Queued)
    {
      if (Asset->LRUFrameIndex < LruAssetFrame)
      {
        LruAssetFrame = Asset->LRUFrameIndex;
        LruAssetIndex = TestAssetIndex;

        if (Asset->LoadState == AssetLoadState_Unloaded)
        {
          FinalAssetIndex = TestAssetIndex; break;
        }
      }
    }
  }

  if (FinalAssetIndex == INVALID_ASSET_INDEX)
  {
    if (LruAssetFrame < u64_MAX)
    {
      asset *LruAsset = Engine->AssetTable+LruAssetIndex;
      Assert(LruAsset->LoadState != AssetLoadState_Unloaded);
      FreeAsset(Engine, LruAsset);

      FinalAssetIndex = LruAssetIndex;
    }
  }

  maybe_asset_slot Result = {};

  if (FinalAssetIndex != INVALID_ASSET_INDEX)
  {
    asset *Asset = Engine->AssetTable+FinalAssetIndex;
    Result.Tag = Maybe_Yes;
    Result.Value = { FinalAssetIndex, ++Asset->Id.Slot.Generation };
  }

  return Result;
}

link_internal void
QueueAssetForLoad(work_queue *Queue, asset *Asset)
{
  Asset("Queuing Asset Slot(%d/%d) Filenode(%S/%S)", Asset->Id.Slot.Index, Asset->Id.Slot.Generation, Asset->Id.FileNode.Dir, Asset->Id.FileNode.Name);

  Assert(Asset->LoadState == AssetLoadState_Unloaded);
  Asset->LoadState = AssetLoadState_Queued;

  work_queue_entry_init_asset AssetJob = {
    .Asset = Asset
  };

  auto Job = WorkQueueEntry(AssetJob);
  PushWorkQueueEntry(Queue, &Job);
}

link_internal void
InitAsset(asset *Asset, thread_local_state *Thread)
{
  Assert(Asset->LoadState == AssetLoadState_Queued);
  /* Asset->LoadState = AssetLoadState_Loading; */

  cs Ext = Extension(Asset->Id.FileNode.Name);

  string_builder Builder = {};

  Append(&Builder, Asset->Id.FileNode.Dir);
  Append(&Builder, CSz("/"));
  Append(&Builder, Asset->Id.FileNode.Name);

  cs AssetFilepath = Finalize(&Builder, Thread->TempMemory, True);

  Asset("Started Asset Load (%S)", AssetFilepath);

  if ( AreEqual(Ext, CSz("vox")) )
  {
    maybe_model_buffer Maybe = LoadVoxModels(Thread->PermMemory, 0, AssetFilepath.Start, Thread->TempMemory);

    if (Maybe.Tag == Maybe_Yes)
    {
      Asset->Models = Maybe.Value;
      FullBarrier;
      Asset->LoadState = AssetLoadState_Loaded;
    }
    else
    {
      Asset->LoadState = AssetLoadState_Error;
    }
  }
  else if ( AreEqual(Ext, CSz("obj")) )
  {
    Asset->LoadState = AssetLoadState_Error;
  }
  else
  {
    SoftError("Unsupported file format while initializing asset (%S).", AssetFilepath);
    Asset->LoadState = AssetLoadState_Error;
  }
}

link_internal maybe_asset_ptr
GetAssetPtr(engine_resources *Engine, asset_slot Slot)
{
  maybe_asset_ptr Result = {};

  if (Slot.Index || Slot.Generation)
  {
    asset *Asset = Engine->AssetTable + Slot.Index;
    if (Asset->Id.Slot.Generation == Slot.Generation)
    {
      Result.Tag = Maybe_Yes;
      Result.Value = Asset;
    }
  }

  return Result;
}

link_internal maybe_asset_ptr
GetAssetPtr(engine_resources *Engine, file_traversal_node *FileNode)
{
  maybe_asset_ptr Result = {};
  RangeIterator(AssetIndex, ASSET_TABLE_COUNT)
  {
    asset *Query = Engine->AssetTable + AssetIndex;
    if (AreEqual(FileNode, &Query->Id.FileNode))
    {
      Result.Tag = Maybe_Yes;
      Result.Value = Query;
      break;
    }
  }

  if (Result.Tag == Maybe_No)
  {
    maybe_asset_slot MaybeSlot = AllocateAssetSlot(Engine);

    if (MaybeSlot.Tag)
    {
      Result = GetAssetPtr(Engine, MaybeSlot.Value);
      if (Result.Tag)
      {
        Result.Value->Id.FileNode = *FileNode;
        Result.Value->LRUFrameIndex = Engine->FrameIndex;
        QueueAssetForLoad(&Engine->Stdlib.Plat.LowPriority, Result.Value);
      }
    }

  }
  else
  {
    Assert(Result.Value->LoadState);
  }


  return Result;
}


link_internal maybe_asset_ptr
GetAssetPtr(engine_resources *Engine, asset_id *AID)
{
  maybe_asset_ptr Result = GetAssetPtr(Engine, AID->Slot);
  if (Result.Tag == Maybe_No) { Result = GetAssetPtr(Engine, &AID->FileNode); }
  return Result;
}
