

link_internal world_chunk_file_header_v2
MakeWorldChunkFileHeader_v2(world_chunk *Chunk)
{
  world_chunk_file_header_v2 Result = {};

  Result.WHNK = WorldChunkFileTag_WHNK;
  Result.Version = 2;
  Result.Checksum = 0xdeadbeef;

  Result.VoxelElementCount        = Volume(Chunk);
  Result.StandingSpotElementCount = (u32)AtElements(&Chunk->StandingSpots);

  NotImplemented;
  /* if (HasMesh(&Chunk->Meshes, MeshBit_Lod0)) */
  {
    /* Result.MeshElementCount       = Chunk->Meshes.E[MeshIndex_Lod0]->At; */
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

  NotImplemented;
  /* if (HasMesh(&Chunk->Meshes, MeshBit_Lod0)) */
  {
    /* Result.MeshElementCount = Chunk->Meshes.E[MeshIndex_Lod0]->At; */
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
  NotImplemented;
#if 0

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
  ReadBytesIntoBuffer(File, (u8*)Result->Verts, VertElementSize*TotalElements);

  //
  // Color data
  Tag = Read_u32(File);
  Assert(Tag == WorldChunkFileTag_COLO);
  

  // TODO(Jesse): Copy into temp buffer and scatter to material props
  NotImplemented;

  u32 ColorElementSize = Header->ColorElementSize;
  Assert(ColorElementSize == (u32)sizeof(v4));
  /* ReadBytesIntoBuffer(File, (u8*)Result->Colors,  ColorElementSize*TotalElements); */

  //
  // Normal data
  Tag = Read_u32(File);
  Assert(Tag == WorldChunkFileTag_NORM);

  u32 NormalElementSize = Header->NormalElementSize;
  Assert(NormalElementSize == (u32)sizeof(v3));
  ReadBytesIntoBuffer(File, (u8*)Result->Normals,  NormalElementSize*TotalElements);

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
  ReadBytesIntoBuffer(File, (u8*)Result->Verts,  VertElementSize*TotalElements);

  //
  // Color data
  Tag = Read_u32(File);
  Assert(Tag == WorldChunkFileTag_MATL);

  u32 MaterialElementSize = Header->MaterialElementSize;
  Assert(MaterialElementSize == (u32)sizeof(vertex_material));
  ReadBytesIntoBuffer(File, (u8*)Result->Mat,  MaterialElementSize*TotalElements);

  //
  // Normal data
  Tag = Read_u32(File);
  Assert(Tag == WorldChunkFileTag_NORM);

  u32 NormalElementSize = Header->NormalElementSize;
  Assert(NormalElementSize == (u32)sizeof(v3));
  ReadBytesIntoBuffer(File, (u8*)Result->Normals,  NormalElementSize*TotalElements);

  Result->Timestamp = __rdtsc();

  return Result;
}

global_variable counted_string
Global_AssetPrefixPath = CSz("");


link_internal world_chunk_file_header_v3
ReadWorldChunkFileHeader_v3(u8_cursor *File)
{
  world_chunk_file_header_v3 Result = {};
  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result,  sizeof(Result)) );

  Assert( Result.WHNK == WorldChunkFileTag_WHNK );
  Assert( Result.Version == 3 );
  Assert( Result.Checksum == 0xdeadbeef );

  Assert( Result.VertexElementSize       == sizeof(v3) );
  Assert( Result.NormalElementSize       == sizeof(v3) );
  Assert( Result.StandingSpotElementSize == sizeof(voxel_position) );
  Assert( Result.VoxelElementSize        == sizeof(voxel) );

  return Result;
}

link_internal world_chunk_file_header_v2
ReadWorldChunkFileHeader_v2(u8_cursor *File)
{
  world_chunk_file_header_v2 Result = {};

  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result,  sizeof(Result)) );

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

  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result,  sizeof(Result)) );

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
ReadWorldChunkVersion(u8_stream *Cursor)
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
DeserializeChunk(u8_stream *FileBytes, world_chunk *Result, memory_arena *PermMemory)
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

  /* u32 Tag = Read_u32(FileBytes); */
  /* Assert( Tag ==  WorldChunkFileTag_VOXD ); */
  /* umm VoxByteCount = Header.VoxelElementCount * Header.VoxelElementSize; */
  /* ReadBytesIntoBuffer(FileBytes, (u8*)Result->Voxels,  VoxByteCount); */

  Result->FilledCount = s32(Header.VoxelElementCount);

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
    u32 Tag = Read_u32(FileBytes);
    Assert(Tag ==  WorldChunkFileTag_SPOT);

    umm ByteCount = SpotElementSize*TotalElements;
    ReadBytesIntoBuffer(FileBytes, (u8*)Result->StandingSpots.Start,  ByteCount);
    Result->StandingSpots.At = Result->StandingSpots.Start + Header.StandingSpotElementCount;
  }

  /* Tag = Read_u32(FileBytes); */
  /* Assert(Tag ==  WorldChunkFileTag_END); */

  /* Result->Flags = Chunk_VoxelsInitialized; */

  /* DebugLine("Loaded Chunk : P (%d,%d,%d) Standing Spots (%d)", Result->WorldP.x, Result->WorldP.y, Result->WorldP.z, Header.StandingSpotElementCount); */
}

link_internal b32
SerializeChunk(world_chunk *Chunk, u8_cursor_block_array *Bytes)
{
  b32 Result = True;

  world_chunk_file_header_v3 FileHeader = MakeWorldChunkFileHeader_v3(Chunk);

  Result &= Write(Bytes, (u8*)&FileHeader, sizeof(FileHeader));

  {
    /* u64 VoxByteCount = FileHeader.VoxelElementCount * FileHeader.VoxelElementSize; */
    /* u32 Tag = WorldChunkFileTag_VOXD; */
    /* Result &= Write(Bytes, Tag); */
    /* Result &= Write(Bytes, (u8*)Chunk->Voxels, VoxByteCount); */
  }

#if 0
  if (FileHeader.MeshElementCount)
  {
    auto Mesh = TakeOwnershipSync(&Chunk->Meshes, MeshBit_Lod0);
    Result &= SerializeMesh(Bytes, Mesh);
    ReleaseOwnership(&Chunk->Meshes, MeshBit_Lod0, Mesh);
  }
#endif

  if (FileHeader.StandingSpotElementCount)
  {
    DebugLine("Writing (%u) StandingSpots", FileHeader.StandingSpotElementCount);
    u64 StandingSpotByteCount = FileHeader.StandingSpotElementSize * FileHeader.StandingSpotElementCount;
    u32 Tag = WorldChunkFileTag_SPOT;
    Result &= Write(Bytes, Tag);
    Result &= Write(Bytes, (u8*)Chunk->StandingSpots.Start, StandingSpotByteCount);
  }

  /* Result &= Write(Bytes, (u32)WorldChunkFileTag_END); */

  return Result;
}

link_internal b32
SerializeChunk(world_chunk *Chunk, counted_string Filename)
{
  auto WorldP = Chunk->WorldP;
  /* counted_string Filename = GetAssetFilenameFor(AssetPath, WorldP, GetTranArena()); */

  u8_cursor_block_array Bytes = {};
  b32 Result = SerializeChunk(Chunk, &Bytes);

  native_file File = OpenFile(Filename, FilePermission_Write);
    Ensure( WriteToFile(&File, &Bytes) ); // TODO(Jesse)(completeness, error_handling): SoftError here?
  CloseFile(&File);
  return Result;
}

link_internal void
FreeModelBuffer(heap_allocator *ModelMemory, model_buffer *Models)
{
  /* NotImplemented; */
  Leak("Implement asset model freeing!");
}

link_internal void
FreeAsset(engine_resources *Engine, asset *Asset)
{
  Assert(Asset->LoadState == AssetLoadState_Loaded ||
         Asset->LoadState == AssetLoadState_Error  );

  switch (Asset->Type)
  {
    InvalidCase(AssetType_Undefined);

    case AssetType_Models:
    {
      FreeModelBuffer(&Engine->AssetSystem.AssetMemory, &Asset->Models);
    } break;

    case AssetType_WorldChunk:
    {
      NotImplemented;
    } break;
  }

  HeapDeallocate(&Engine->AssetSystem.AssetMemory, Cast(void*, Asset->Id.FileNode.Dir.Start));
  HeapDeallocate(&Engine->AssetSystem.AssetMemory, Cast(void*, Asset->Id.FileNode.Name.Start));

  Clear(Asset);
}

link_internal void
QueueAssetForLoad(work_queue *Queue, asset *Asset)
{
  Asset("Queuing Asset Slot(%d) Filenode(%S/%S)", Asset->Id.Index, Asset->Id.FileNode.Dir, Asset->Id.FileNode.Name);

  Assert(Asset->LoadState == AssetLoadState_Allocated);
  Asset->LoadState = AssetLoadState_Queued;

  work_queue_entry_init_asset AssetJob = {
    .Asset = Asset
  };

  auto Job = WorkQueueEntry(AssetJob);
  PushWorkQueueEntry(Queue, &Job);
}

link_internal maybe_asset_ptr
AllocateAsset(engine_resources *Engine, file_traversal_node *FileNode, u64 FrameIndex = 0)
{
  Assert(FileNode->Type);
  Assert(FileNode->Dir.Count);
  Assert(FileNode->Name.Count);

  AcquireFutex(&Engine->AssetSystem.AssetFutex);
  u16 FinalAssetIndex   = INVALID_ASSET_INDEX;

  u64 LruAssetFrame     = ASSET_LOCKED_FRAME_INDEX;
  u16 LruAssetIndex     = INVALID_ASSET_INDEX;

  RangeIterator_t(u16, TestAssetIndex, ASSET_TABLE_COUNT)
  {
    asset *Asset = Engine->AssetSystem.AssetTable + TestAssetIndex;
    if (Asset->LoadState == AssetLoadState_Unloaded)
    {
      FinalAssetIndex = TestAssetIndex; break;
    }

    if (Asset->LoadState != AssetLoadState_Queued)
    {
      if (Asset->LRUFrameIndex < LruAssetFrame)
      {
        LruAssetFrame = Asset->LRUFrameIndex;
        LruAssetIndex = TestAssetIndex;
      }
    }
  }

  if (FinalAssetIndex == INVALID_ASSET_INDEX)
  {
    if (LruAssetFrame < ASSET_LOCKED_FRAME_INDEX)
    {
      asset *LruAsset = Engine->AssetSystem.AssetTable+LruAssetIndex;
      Assert(LruAsset->LoadState == AssetLoadState_Loaded || LruAsset->LoadState == AssetLoadState_Error);
      FreeAsset(Engine, LruAsset);

      FinalAssetIndex = LruAssetIndex;
    }
  }

  maybe_asset_ptr Result = {};

  if (FinalAssetIndex != INVALID_ASSET_INDEX)
  {
    asset *Asset = Engine->AssetSystem.AssetTable+FinalAssetIndex;
    Asset->LoadState = AssetLoadState_Allocated;

    Asset->Id.Index = FinalAssetIndex;
    Asset->Id.FileNode = DeepCopy(&Engine->AssetSystem.AssetMemory, FileNode);

    Result.Tag = Maybe_Yes;
    Result.Value = Asset;
  }
  ReleaseFutex(&Engine->AssetSystem.AssetFutex);


  if (Result.Tag)
  {
    u64 FrameIndexToWrite = FrameIndex ? FrameIndex : Engine->FrameIndex; 
    /* Asset("(%p) Setting Asset (%p) LRU Index (%p)", ThreadLocal_ThreadIndex, Result.Value->Id.Index, FrameIndexToWrite); */
    Result.Value->LRUFrameIndex = FrameIndexToWrite;
    /* Result.Value->LRUFrameIndex = FrameIndex ? FrameIndex : Engine->FrameIndex; */
    Assert(Result.Value->Id.Index != INVALID_ASSET_INDEX);

    Assert(Result.Value->Id.FileNode.Dir.Start);
    Assert(Result.Value->Id.FileNode.Name.Start);

    Assert(Result.Value->Id.FileNode.Dir.Start != FileNode->Dir.Start);
    Assert(Result.Value->Id.FileNode.Name.Start != FileNode->Name.Start);
  }

  return Result;
}

link_internal maybe_asset_ptr
AllocateAsset(engine_resources *Engine, u64 FrameIndex = 0)
{
  cs AssetName = CS(Engine->AssetSystem.CurrentUnnamedAssetIndex++);

  file_traversal_node FileNode = {FileTraversalType_File, AssetName, AssetName} ;
  maybe_asset_ptr Result = AllocateAsset(Engine,  &FileNode, FrameIndex);
  return Result;
}


link_internal void
InitAsset(engine_resources *Engine, asset *Asset, thread_local_state *Thread)
{
  Assert(Asset->LoadState == AssetLoadState_Queued);
  /* Asset->LoadState = AssetLoadState_Loading; */

  cs Ext = Extension(Asset->Id.FileNode.Name);

  cs AssetFilepath = Concat(Asset->Id.FileNode.Dir, CSz("/"), Asset->Id.FileNode.Name, Thread->TempMemory, 1);

  Asset("Started Asset Load (%S)", AssetFilepath);

  if ( AreEqual(Ext, CSz("vox")) )
  {
    maybe_model_buffer Maybe = LoadVoxModels(Engine, Thread->PermMemory, 0, AssetFilepath.Start, Thread->TempMemory);

    if (Maybe.Tag == Maybe_Yes)
    {
      Asset->Type = AssetType_Models;
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
  else if ( AreEqual(Ext, CSz("chunk")) )
  {
    u8_stream Bytes = U8_StreamFromFile(AssetFilepath, Thread->TempMemory);

    world_chunk _Chunk = {};
    world_chunk *Chunk = &_Chunk;

    // TODO(Jesse)(memory, leak): This leaks the chunk memory; we do not have a
    // way of reclaiming it when it's loaded from disk.  The chunk buffers for
    // world chunks are just never freed, but the chunks are recycled.  This
    // chunk is a weird one as the buffers can be of arbitrary size.
    Deserialize(&Bytes, Chunk, Thread->PermMemory, 1);

    NotImplemented;
    voxel *VoxelBuffer = 0;
    MarkBoundaryVoxels_MakeExteriorFaces(Chunk->Occupancy, VoxelBuffer, Chunk->Dim, V3i(0), Chunk->Dim);

    /* FinalizeChunkInitialization(Chunk); */

    data_type Type = GetMeshDatatypeForDimension(Chunk->Dim);
    auto *TempMesh = AllocateTempMesh(Thread->TempMemory, Type);

    NotImplemented;
    /* RebuildWorldChunkMesh(Thread, Chunk, V3i(0), Chunk->Dim, MeshBit_Lod0, TempMesh, Thread->TempMemory, Chunk->Dim/-2.f); */

    Asset->Type = AssetType_WorldChunk;
    Asset->Chunk = *Chunk;

    Asset->LoadState = AssetLoadState_Loaded;
  }
  else
  {
    SoftError("Unsupported file format while initializing asset (%S).", AssetFilepath);
    Asset->LoadState = AssetLoadState_Error;
  }
}

link_internal maybe_asset_ptr
GetOrAllocateAsset(engine_resources *Engine, file_traversal_node *FileNode, u64 FrameIndex = 0)
{
  asset *Asset  = 0;

  AcquireFutex(&Engine->AssetSystem.AssetFutex);
  {
    RangeIterator(AssetIndex, ASSET_TABLE_COUNT)
    {
      asset *Query = Engine->AssetSystem.AssetTable + AssetIndex;
      if (AreEqual(FileNode, &Query->Id.FileNode))
      {
        Asset = Query;
        break;
      }
    }
  }
  ReleaseFutex(&Engine->AssetSystem.AssetFutex);

  if (Asset == 0)
  {
    Asset = AllocateAsset(Engine, FileNode, FrameIndex).Value;
  }


  maybe_asset_ptr Result = {};

  if (Asset)
  {
    u64 FrameIndexToWrite = FrameIndex ? FrameIndex : Engine->FrameIndex; 
    /* Asset("(%p) Setting Asset (%p) LRU Index (%p)", ThreadLocal_ThreadIndex, Asset->Id.Index, FrameIndexToWrite); */
    Asset->LRUFrameIndex = FrameIndexToWrite;
    Assert(Asset->LoadState != AssetLoadState_Unloaded);

    Result.Tag = Maybe_Yes;
    Result.Value = Asset;
  }

  return Result;
}

link_internal asset_id
GetOrAllocateAssetId(engine_resources *Engine, file_traversal_node *FileNode, u64 FrameIndex = 0)
{
  asset_id Result = {};
  Result.Index = INVALID_ASSET_INDEX;

  maybe_asset_ptr MaybeAsset = GetOrAllocateAsset(Engine, FileNode, FrameIndex);
  if (MaybeAsset.Tag)
  {
    Result = MaybeAsset.Value->Id;
  }
  return Result;
}

link_internal asset_id
GetOrAllocateAssetId(engine_resources *Engine, file_traversal_node FileNode, u64 FrameIndex = 0)
{
  return GetOrAllocateAssetId(Engine, &FileNode, FrameIndex);
}

link_internal maybe_asset_ptr
GetAssetPtr(engine_resources *Engine, asset_id *AID, u64 FrameIndex = 0)
{
  maybe_asset_ptr Result = {};

  if (AID->Index != INVALID_ASSET_INDEX)
  {
    AcquireFutex(&Engine->AssetSystem.AssetFutex);

    asset *Asset = Engine->AssetSystem.AssetTable + AID->Index;

    if (Asset->LRUFrameIndex != ASSET_LOCKED_FRAME_INDEX)
    {
      Assert(Asset->Id.Index == AID->Index);

      // LoadState must be one of Allocated, Queued, Loaded or Error
      Assert(Asset->LoadState != AssetLoadState_Unloaded);

      u64 FrameIndexToWrite = FrameIndex ? FrameIndex : Engine->FrameIndex; 
      /* Asset("(%p) Setting Asset (%p) LRU Index (%p)", ThreadLocal_ThreadIndex, AID->Index, FrameIndexToWrite); */
      AtomicWrite(&Asset->LRUFrameIndex, FrameIndexToWrite);

      Result.Tag = Maybe_Yes;
      Result.Value = Asset;
    }

    ReleaseFutex(&Engine->AssetSystem.AssetFutex);
  }
  else
  {
    if (AID->FileNode.Type)
    {
      asset_id NewAssetId = GetOrAllocateAssetId(Engine, AID->FileNode, FrameIndex);
      if (NewAssetId.Index != INVALID_ASSET_INDEX)
      {
        Result = GetAssetPtr(Engine, &NewAssetId, FrameIndex);
      }
    }
  }

  if (Result.Tag)
  {
    AID->Index = Result.Value->Id.Index;
    Assert(StringsMatch(AID->FileNode.Dir,  Result.Value->Id.FileNode.Dir));
    Assert(StringsMatch(AID->FileNode.Name, Result.Value->Id.FileNode.Name));
    Assert(Result.Value->LoadState);
  }

  return Result;
}



#if 0
link_internal maybe_asset_ptr
GetAssetPtr(engine_resources *Engine, file_traversal_node *FileNode, u64 FrameIndex = 0)
{
  maybe_asset_ptr Result = {};


  if (FileNode->Dir || FileNode->Name)
  {
    maybe_asset_id MaybeId = GetAssetId(FileNode);

    if (Result.Tag)
    {
      maybe_asset_slot MaybeSlot = AllocateAssetSlot(Engine);

      if (MaybeSlot.Tag)
      {
        Result = GetAssetPtr(Engine, MaybeSlot.Value);
        if (Result.Tag)
        {
          Result.Value->Id.FileNode = DeepCopy(&Engine->AssetMemory, FileNode);
          Result.Value->LRUFrameIndex = FrameIndex ? FrameIndex : Engine->FrameIndex;
          QueueAssetForLoad(&Engine->Stdlib.Plat.LowPriority, Result.Value);
        }
      }

    }
    else
    {
      Assert(Result.Value->LoadState);
    }
  }

  return Result;
}
#endif

link_internal model *
GetModel(asset *Asset, asset_id *AID, u64 ModelIndex)
{
  Assert(Asset->Type == AssetType_Models);
  Assert(ModelIndex < Asset->Models.Count);
  Assert(AID->Index == Asset->Id.Index);
  Assert(AreEqual(&AID->FileNode, &Asset->Id.FileNode));

  model *Result = Asset->Models.Start + ModelIndex;
  return Result;
}

link_internal asset *
GetAndLockAssetSync(engine_resources *Engine, asset_id *AID)
{
  /* Asset("(%p) Locking Asset (%p)", ThreadLocal_ThreadIndex, AID->Index); */
  maybe_asset_ptr Result = GetAssetPtr(Engine, AID, ASSET_LOCKED_FRAME_INDEX);

  while (Result.Tag == Maybe_No)
  {
    /* Info("Missed Locking Asset (%p)", AID->Index); */
    SleepMs(1);
    Result = GetAssetPtr(Engine, AID, ASSET_LOCKED_FRAME_INDEX);
  }


  Assert(Result.Value->LRUFrameIndex == ASSET_LOCKED_FRAME_INDEX);

  /* Asset("(%p) Locked Asset (%p) (%p)", ThreadLocal_ThreadIndex, AID->Index, Result.Value->LRUFrameIndex); */
  return Result.Value;
}

link_internal void
UnlockAsset(engine_resources *Engine, asset *Asset)
{
  /* Asset("(%p) Unlocking Asset (%p) (%p)", ThreadLocal_ThreadIndex, Asset->Id.Index, Asset->LRUFrameIndex); */
  Assert(Asset->LRUFrameIndex == ASSET_LOCKED_FRAME_INDEX);
  Asset->LRUFrameIndex = Engine->FrameIndex;
  /* Asset("(%p) Unlocked Asset (%p) (%p)", ThreadLocal_ThreadIndex, Asset->Id.Index, Asset->LRUFrameIndex); */
}

#if 0
link_internal maybe_asset_ptr
NewAsset(engine_resources *Engine)
{
  maybe_asset_ptr Result = {};

  maybe_asset_slot MaybeAssetSlot = AllocateAssetSlot(Engine);
  if (MaybeAssetSlot.Tag)
  {
    Result = GetAssetPtr(Engine, MaybeAssetSlot.Value);

    // NOTE(Jesse): Somewhat questionable...
    if (Result.Tag)
    {
      Result.Value->LoadState = AssetLoadState_Loaded;
    }
  }
  else
  {
    SoftError("Unable to allocate asset slot.");
  }

  return Result;
}

link_internal maybe_asset_ptr
NewAssetForGeneratedModel(engine_resources *Engine, model *Model)
{
  // NOTE(Jesse): This is the same as NewAsset so I'm removing it 
  NotImplemented;

  maybe_asset_ptr Result = {};

  maybe_asset_slot MaybeAssetSlot = AllocateAssetSlot(Engine);
  if (MaybeAssetSlot.Tag)
  {
    Result = GetAssetPtr(Engine, MaybeAssetSlot.Value);

    // NOTE(Jesse): Somewhat questionable...
    if (Result.Tag)
    {
      Result.Value->LoadState = AssetLoadState_Loaded;
    }
  }
  else
  {
    SoftError("Unable to allocate asset slot for Model (%p).", Model);
  }

  return Result;
}
#endif
