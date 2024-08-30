// NOTE(Jesse): These chunk identifiers are actually present in the file in the
// reverse byte order.. ie. ID_VOX == "VOX " in the file.
enum Chunk_ID
{
  ID_NONE = 0,

  ID_VOX  = ' XOV',
  ID_MAIN = 'NIAM',
  ID_PACK = 'KCAP',
  ID_SIZE = 'EZIS',
  ID_XYZI = 'IZYX',
  ID_RGBA = 'ABGR',

  // chunks that are present in vox version 200

  ID_nTRN = 'NRTn',
  ID_nGRP = 'PRGn',
  ID_nSHP = 'PHSn',
  ID_MATL = 'LTAM',
  ID_LAYR = 'RYAL',
  ID_rOBJ = 'JBOr',
  ID_rCAM = 'MACr',
  ID_NOTE = 'ETON',
  ID_IMAP = 'PAMI',
};

poof(generate_string_table(Chunk_ID))
#include <generated/generate_string_table_Chunk_ID.h>

inline u8
ReadChar(native_file* File, s32* byteCounter)
{
  u8 c;
  ReadBytesIntoBuffer(File, &c, sizeof(u8));
  *byteCounter -= sizeof(u8);
  return c;
}

inline s32
ReadInt(native_file* File)
{
  s32 i;
  ReadBytesIntoBuffer(File, (u8*)&i, sizeof(s32));
  return i;
}

inline s32
ReadInt(native_file* File, s32* byteCounter)
{
  *byteCounter -= sizeof(s32);
  return ReadInt(File);
}

link_internal void
ParseVoxString(native_file* File, s32* byteCounter)
{
  s32 Count = ReadInt(File, byteCounter);
  Assert(*byteCounter >= Count);
  *byteCounter -= Count;
  u8 Garbage;
  for (s32 Index = 0; Index < Count; ++Index)
  {
    ReadBytesIntoBuffer(File, &Garbage,  1);
  }
}

link_internal void
ParseVoxDict(native_file* File, s32* byteCounter)
{
  s32 KVPairCount = ReadInt(File, byteCounter);
  for (s32 Index = 0; Index < KVPairCount; ++Index)
  {
    ParseVoxString(File, byteCounter);
    ParseVoxString(File, byteCounter);
  }
}

link_internal void
ParseVoxRotation(native_file* File, s32* byteCounter)
{
  ReadChar(File, byteCounter);
}

inline Chunk_ID
GetHeaderType(native_file* File, s32* byteCounter)
{
  s32 ID = ReadInt(File, byteCounter);

  /* if ( ID == ID_VOX  || */
  /*      ID == ID_MAIN || */
  /*      ID == ID_PACK || */
  /*      ID == ID_SIZE || */
  /*      ID == ID_XYZI || */
  /*      ID == ID_RGBA ) */
  /* { */
  /* } */
  /* else */
  /* { */
  /*   Error("Unknown Header type (%d)", ID); */
  /* } */

  return (Chunk_ID)ID;
}

inline void
ReadVoxChunk(native_file *File)
{
  s32 Version = s32_MAX;
  s32 ID = ReadInt(File);
  Assert( ID == ID_VOX );

  Version = ReadInt(File);
  if (Version > 200)
  {
    Error("Unsupported VOX version (%d)", Version);
  }

  return;
}

inline s32
ReadMainChunk(native_file *File)
{
  s32 ID = ReadInt(File);

  if ( ID != ID_MAIN )
  {
    Error("Invalid Main Chunk");
    return 0;
  }

  s32 selfSize = ReadInt(File);
  if ( selfSize != 0 ) 
  {
    Error("Invalid Main Chunk");
    return 0;
  }

  s32 childSize = ReadInt(File);
  if (childSize == 0 )
  {
    Error("Invalid Main Chunk");
    return 0;
  }

  return childSize;
}

inline chunk_dimension
ReadSizeChunk(native_file *File, s32* byteCounter)
{
  s32 ChunkContentBytes = ReadInt(File, byteCounter);
  s32 ChildChunkCount = ReadInt(File, byteCounter);

  s32 chunkX = ReadInt(File, byteCounter);
  s32 chunkY = ReadInt(File, byteCounter);
  s32 chunkZ = ReadInt(File, byteCounter);

  chunk_dimension Result = Chunk_Dimension(chunkX, chunkY, chunkZ);
  return Result;
}

inline s32
ReadPackChunk(native_file *File, s32* byteCounter)
{
  s32 ChunkContentBytes = ReadInt(File, byteCounter);
  s32 ChildChunkCount = ReadInt(File, byteCounter);

  s32 TotalChunkCount = ReadInt(File, byteCounter);
  return TotalChunkCount;
}

inline s32
ReadXYZIChunk(native_file *File, s32* byteCounter)
{
  s32 ChunkContentBytes = ReadInt(File, byteCounter);
  s32 ChildChunkCount = ReadInt(File, byteCounter);

  s32 nVoxels = ReadInt(File, byteCounter);
  return nVoxels;
}

enum vox_loader_clip_behavior
{
  VoxLoaderClipBehavior_ClipToVoxels,
  VoxLoaderClipBehavior_NoClipping,
};


global_variable random_series TMP = {54235432543};

link_internal vox_data_block_array
LoadVoxData(v3_cursor *ColorPalette, memory_arena *TempMemory, memory_arena *PermMemory, heap_allocator *Heap, const char *filepath, vox_loader_clip_behavior ClipBehavior, v3i HalfApronMin = {}, v3i HalfApronMax = {}, v3i ModDim = {})
{
  vox_data_block_array Result = { {}, {}, TempMemory };

  v3i ReportedDim = {};
  b32 HadPaletteData = False;


  native_file _ModelFile = OpenFile(filepath, FilePermission_Read);
  native_file *ModelFile = &_ModelFile;


  s32_cursor ColorIndexBuffer = S32Cursor(256, TempMemory);

  vox_data Current = {};
  if (ModelFile->Handle)
  {
    // Ensure we're dealing with a .vox file
    ReadVoxChunk(ModelFile);

    s32 totalChunkBytes = ReadMainChunk(ModelFile);
    s32 bytesRemaining = totalChunkBytes;

    s32 TotalChunkCount = 1; // Sometimes overwritten if theres a 'PACK' chunk
    // s32 TotalChunksRead = 0;

    b32 Error = False;
    while (Error == False && bytesRemaining > 0)
    {
      Chunk_ID CurrentId = GetHeaderType(ModelFile, &bytesRemaining);
      /* DebugLine("%S", ToString(CurrentId)); */
      switch ( CurrentId )
      {
        case ID_RGBA:
        {
          HadPaletteData = True;
          s32 ChunkContentBytes = ReadInt(ModelFile, &bytesRemaining);
          s32 ChildChunkCount   = ReadInt(ModelFile, &bytesRemaining);

          Assert(ChunkContentBytes == 256*4);
          Assert(ChildChunkCount == 0);

          /* Result.Palette = (v4*)HeapAllocate(Heap, 256); */
          /* Palette = Allocate(v3, PermMemory, 256); */

          for ( s32 PaletteIndex = 0;
                    PaletteIndex < 256;
                  ++PaletteIndex )
          {
            u8 R = ReadChar(ModelFile, &bytesRemaining);
            u8 G = ReadChar(ModelFile, &bytesRemaining);
            u8 B = ReadChar(ModelFile, &bytesRemaining);
            u8 A = ReadChar(ModelFile, &bytesRemaining);

            v3 ThisColor = V3(R,G,B)/255.f;
            /* v3 ThisColor = V3(R,G,B); */
            s32 Found = -1;
            {
              IterateOver(ColorPalette, QueryColor, QueryColorIndex)
              {
                if (WithinTolerance(0.015f, ThisColor, *QueryColor))
                {
                  Found = s32(QueryColorIndex); 
                  v3 PaletteColor = GetPaletteData(u32(Found));
                  Assert(PaletteColor == *QueryColor);
                  Assert(WithinTolerance(0.015f, ThisColor, PaletteColor));
                  break;
                }
              }
            }

            if (Found == -1)
            {
              s32 NewColorIndex = s32(AtElements(ColorPalette));
              Assert(NewColorIndex >= 0);

              Push(ColorPalette, ThisColor);

              {
                v3 *Last = LastElement(ColorPalette);
                Assert(*Last == ThisColor);
              }

              Push(&ColorIndexBuffer, NewColorIndex);

              {
                s32 *I = LastElement(&ColorIndexBuffer);
                Assert(*I == NewColorIndex);
                Assert(*I == LastIndex(ColorPalette));
              }
            }
            else
            {
              Assert(Found >= 0);
              s32 *Index = Push(&ColorIndexBuffer, Found);

              {
                Assert(*Index == Found);
                v3 QueryColor = GetPaletteData(u32(Found));
                Assert(WithinTolerance(0.015f, ThisColor, QueryColor));
              }
            }

            /* Global_ColorPalette[PaletteIndex].r = R; */
            /* Global_ColorPalette[PaletteIndex].g = G; */
            /* Global_ColorPalette[PaletteIndex].b = B; */
            /* Current.Palette[PaletteIndex].a = A; */
          }

          /* Global_ColorPaletteAt += 256; */
          Info("ColorPalette->At %d", AtElements(ColorPalette));
        } break;

        case ID_PACK:
        {
          TotalChunkCount = ReadPackChunk(ModelFile, &bytesRemaining);
        } break;

        case ID_SIZE:
        {
          ReportedDim = ReadSizeChunk(ModelFile, &bytesRemaining);
        } break;

        case ID_XYZI:
        {
          Current = {};

          // ++TotalChunksRead;

          s32 ReportedVoxelCount = ReadXYZIChunk(ModelFile, &bytesRemaining);

          s32 ActualVoxelCount = 0;

          s32 maxX = s32_MIN;
          s32 maxY = s32_MIN;
          s32 maxZ = s32_MIN;

          s32 minX = s32_MAX;
          s32 minY = s32_MAX;
          s32 minZ = s32_MAX;

          boundary_voxel *LocalVoxelCache = Allocate(boundary_voxel, GetTranArena(), (umm)ReportedVoxelCount);
          for( s32 VoxelCacheIndex = 0;
                   VoxelCacheIndex < ReportedVoxelCount;
                 ++VoxelCacheIndex)
          {
            s32 X    = (s32)ReadChar(ModelFile, &bytesRemaining);
            s32 Y    = (s32)ReadChar(ModelFile, &bytesRemaining);
            s32 Z    = (s32)ReadChar(ModelFile, &bytesRemaining);
            u8 Color =      ReadChar(ModelFile, &bytesRemaining);

            // NOTE(Jesse): This is necessary because of a somewhat
            // questionable decision in the .vox file format .. indices 0->254
            // are mapped to 1->255 in the color palette, presumably such that
            // 0 is always black.  I have no idea why the fuck this would be
            // the way that it is, but.. there you go.
            //
            // https://github.com/ephtracy/voxel-model/blob/8044f9eb086216f3485cdaa525a52120d72274e9/MagicaVoxel-file-format-vox.txt#L76C1-L90C1
            //
            // Relevant table pasted here for posterity for when that link dies.
            //
            // 7. Chunk id 'RGBA' : palette
            // -------------------------------------------------------------------------------
            // # Bytes  | Type       | Value
            // -------------------------------------------------------------------------------
            // 4 x 256  | int        | (R, G, B, A) : 1 byte for each component
            //                       | * <NOTICE>
            //                       | * color [0-254] are mapped to palette index [1-255], e.g : 
            //                       | 
            //                       | for ( int i = 0; i <= 254; i++ ) {
            //                       |     palette[i + 1] = ReadRGBA(); 
            //                       | }
            // -------------------------------------------------------------------------------
            //
            if (Color > 0) { Color -= 1; }

            voxel_position TestP = Voxel_Position(X,Y,Z);
            if (IsInsideDim(ReportedDim, TestP))
            {
              ++ActualVoxelCount;
              maxX = Max(TestP.x, maxX);
              maxY = Max(TestP.y, maxY);
              maxZ = Max(TestP.z, maxZ);

              minX = Min(TestP.x, minX);
              minY = Min(TestP.y, minY);
              minZ = Min(TestP.z, minZ);

              LocalVoxelCache[VoxelCacheIndex] = boundary_voxel(TestP.x, TestP.y, TestP.z, Color);
              SetFlag(&LocalVoxelCache[VoxelCacheIndex], Voxel_Filled);
            }
            else
            {
              BUG("Voxel detected outside the dimention it should be in while loading (%s)!", filepath);
              Error = True;
              Assert(Current.ChunkData == 0);
              break;
            }
          }

          if (Error) break;

          v3i Max = {};
          v3i Min = {};
          v3i ModelDim = {};
          switch (ClipBehavior)
          {
            case VoxLoaderClipBehavior_ClipToVoxels:
            {
              s32 IndexToPosition = 1;  // max(X,Y,Z) are indicies, convert to positions
              Max = V3i(maxX+IndexToPosition, maxY+IndexToPosition, maxZ+IndexToPosition);
              Min = V3i(minX, minY, minZ);
              ModelDim = Max - Min + HalfApronMin + HalfApronMax;
            } break;

            case VoxLoaderClipBehavior_NoClipping:
            {
              ModelDim = ReportedDim + HalfApronMin + HalfApronMax;
            } break;
          }

          if (ModDim.x || ModDim.y || ModDim.z)
          {
            v3i Fixup = ModDim - (ModelDim % ModDim);
            ModelDim += Fixup;
            Assert(ModelDim % ModDim == V3i(0));
          }


          Current.ChunkData = AllocateChunkData(PermMemory, ModelDim);
          Current.ChunkData->Dim = ModelDim;

          for( s32 VoxelCacheIndex = 0;
                   VoxelCacheIndex < ActualVoxelCount;
                 ++VoxelCacheIndex)
          {
            boundary_voxel *Voxel = &LocalVoxelCache[VoxelCacheIndex];
            Voxel->Offset = Voxel->Offset - Min + HalfApronMin;
            s32 Index = GetIndex(Voxel->Offset, ModelDim);

            if (Voxel->V.Flags & Voxel_Filled)
            /* if (Voxel->V.Flags & Voxel_Filled && VoxelCacheIndex == 16) */
            {
              /* if (RandomUnilateral(&TMP) > 0.25f) { Voxel->V.Transparency = 255; } */
              /* Current.ChunkData->Voxels[Index].Transparency = 128; */
              /* Voxel->V.Transparency = 255; */
            }
            else
            {
              /* Voxel->V.Transparency = 0; */
            Assert(Voxel->V.Transparency == 0);
            Voxel->V.Transparency = 128;
            /* Voxel->V.Emission = 128; */
            }
            /* Voxel->V.Transparency = 0; */

            Current.ChunkData->Voxels[Index] = Voxel->V;
            /* Result.ChunkData->VoxelLighting[Index] = VoxelLighting(0xff); */
          }

          FullBarrier;

          Current.ChunkData->Flags = Chunk_VoxelsInitialized;

          /* MarkBoundaryVoxels_NoExteriorFaces( Current.ChunkData->Voxels, Current.ChunkData->Dim, {}, Current.ChunkData->Dim); */
          MarkBoundaryVoxels_MakeExteriorFaces( Current.ChunkData->Voxels, Current.ChunkData->Dim, {}, Current.ChunkData->Dim);

          Push(&Result, &Current);
        } break;

        // Transform node chunk
        case ID_nTRN:
        {
          s32 ChunkContentBytes = ReadInt(ModelFile, &bytesRemaining);
          s32 ChildChunkCount = ReadInt(ModelFile, &bytesRemaining);

          s32 NodeId = ReadInt(ModelFile, &bytesRemaining);

          // Transform node chunk
          ParseVoxDict(ModelFile, &bytesRemaining);
          s32 ChildNodeId = ReadInt(ModelFile, &bytesRemaining);
          Ensure(ReadInt(ModelFile, &bytesRemaining) == -1); // reserved field
          s32 LayerId = ReadInt(ModelFile, &bytesRemaining);
          s32 FrameCount = ReadInt(ModelFile, &bytesRemaining);
          Assert(FrameCount); // Apparently this has to be greater than 0

          for (s32 FrameIndex = 0; FrameIndex < FrameCount; ++FrameIndex)
          {
            ParseVoxDict(ModelFile, &bytesRemaining);
          }
        } break;

        case ID_rOBJ:
        case ID_nSHP:
        case ID_nGRP:
        case ID_rCAM:
        case ID_NOTE:
        case ID_IMAP:
        case ID_LAYR:
        case ID_MATL:
        {
          s32 ChunkContentBytes = ReadInt(ModelFile, &bytesRemaining);
          s32 ChildChunkCount = ReadInt(ModelFile, &bytesRemaining);

          for (s32 ByteIndex = 0; ByteIndex < ChunkContentBytes; ++ByteIndex)
          {
            ReadChar(ModelFile, &bytesRemaining);
          }

          /* Info("Skipping unsupported Chunk type (%S) while parsing (%s)", CS((const char*)&CurrentId, 4), filepath); */
        } break;

        InvalidDefaultCase;
      }
    }

    if (Error == False)
    {
      Assert(bytesRemaining == 0);
    }
    else
    {
      /* Assert(Result.ChunkData == 0); */
      Assert(False);
    }

    CloseFile(ModelFile);
  }
  else
  {
    Error("Couldn't read model file '%s' .", filepath);
  }

  if (HadPaletteData)
  {
    /* s32 PaletteBase = s32(AtElements(ColorPalette))-256; */

    IterateOver(&Result, VoxData, VoxDataIndex)
    {
      DimIterator(x, y, z, VoxData->ChunkData->Dim)
      {
        s32 Index = GetIndex(x, y, z, VoxData->ChunkData->Dim);

        u16 IndexBufferIndex = VoxData->ChunkData->Voxels[Index].Color;
        Assert(IndexBufferIndex < 256);

        s32 ColorIndex = ColorIndexBuffer.Start[IndexBufferIndex];
        VoxData->ChunkData->Voxels[Index].Color = SafeTruncateToU16(umm(ColorIndex));
        /* DebugLine("wee %d (%d,%d,%d)", VoxDataIndex, x, y, z ); */
      }
    }
  }

  return Result;
}

link_internal void
AllocateAndBuildMesh(vox_data *Vox, model *DestModel, memory_arena *TempMemory, memory_arena *PermMemory)
{
  chunk_data *ChunkData = Vox->ChunkData;
  DestModel->Dim = ChunkData->Dim;

  data_type Type = GetMeshDatatypeForDimension(ChunkData->Dim);
  geo_u3d *TempMesh = AllocateTempMesh(TempMemory, Type);

  BuildWorldChunkMeshFromMarkedVoxels_Greedy(Vox, TempMesh, 0, TempMemory, DestModel->Dim/-2.f);

  engine_resources *Engine = GetEngineResources();

  geo_u3d *FinalMesh = GetPermMeshForChunk(&Engine->geo_u3d_MeshFreelist, TempMesh, PermMemory);

  DeepCopy(TempMesh, FinalMesh);

  AtomicReplaceMesh(&DestModel->Meshes, MeshBit_Lod0, FinalMesh, __rdtsc());
}

link_internal maybe_model_buffer
LoadVoxModels(memory_arena *PermMemory, heap_allocator *Heap, const char *filepath, memory_arena *TempMemory)
{
  TIMED_FUNCTION();

  maybe_model_buffer Result =  {};

  vox_data_block_array VoxArray = LoadVoxData(GetColorPalette(), TempMemory, PermMemory, Heap, filepath, VoxLoaderClipBehavior_ClipToVoxels);

  umm VoxElements = TotalElements(&VoxArray);

  if (VoxElements)
  {
    Result.Tag = Maybe_Yes;

    Result.Value = ModelBuffer(VoxElements, PermMemory);
    IterateOver(&VoxArray, Vox, VoxIndex)
    {
      if (Vox->ChunkData)
      {
        model *Model = &Result.Value.Start[GetIndex(&VoxIndex)];
        Model->Vox = *Vox;
        AllocateAndBuildMesh(Vox, Model, TempMemory, PermMemory );
      }
    }
  }

  return Result;
}

