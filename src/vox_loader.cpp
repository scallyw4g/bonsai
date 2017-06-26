#include <stdio.h>
#include <string.h>
#include <bonsai.h>
#include <bonsai_math.h>
#include <bonsai_types.h>

// Number of bytes per int according to .vox file format
#define VOX_INT_BYTES 4
#define VOX_CHAR_BYTES 1


#define MV_ID( a, b, c, d ) \
  (( a ) | ( b << 8 ) | ( c << 16 ) | ( d << 24 ))

enum Chunk_ID
{
  ID_NONE = 0,

  ID_VOX  = MV_ID( 'V', 'O', 'X', ' ' ),
  ID_MAIN = MV_ID( 'M', 'A', 'I', 'N' ),
  ID_PACK = MV_ID( 'P', 'A', 'C', 'K' ),
  ID_SIZE = MV_ID( 'S', 'I', 'Z', 'E' ),
  ID_XYZI = MV_ID( 'X', 'Y', 'Z', 'I' ),
  ID_RGBA = MV_ID( 'R', 'G', 'B', 'A' )
};

inline unsigned char
ReadChar(FILE* File, int* byteCounter)
{
  unsigned char c;
  fread(&c, 1, VOX_CHAR_BYTES, File);
  *byteCounter -= VOX_CHAR_BYTES;
  return c;
}

inline int
ReadInt(FILE* File)
{
  int i;
  fread(&i, 1, VOX_INT_BYTES, File);
  return i;
}

inline int
ReadInt(FILE* File, int* byteCounter)
{
  *byteCounter -= VOX_INT_BYTES;
  return ReadInt(File);
}

inline Chunk_ID
GetHeaderType(FILE* File, int* byteCounter)
{
  int ID = ReadInt(File, byteCounter);

  Assert( ID == ID_VOX  ||
          ID == ID_MAIN ||
          ID == ID_PACK ||
          ID == ID_SIZE ||
          ID == ID_XYZI ||
          ID == ID_RGBA );

  return (Chunk_ID)ID;
}

inline void
ReadVoxChunk(FILE *File)
{
  int ID, Version = INT_MAX;

  ID = ReadInt(File);
  Assert( ID == ID_VOX );

  Version = ReadInt(File);
  Assert(Version == 150);

  return;
}

inline int
ReadMainChunk(FILE *File)
{
  int ID = ReadInt(File);
  Assert( ID == ID_MAIN );

  int selfSize = ReadInt(File);
  Assert(selfSize == 0 );

  int childSize = ReadInt(File);
  Assert(childSize > 0 );

  return childSize;
}

inline chunk_dimension
ReadSizeChunk(FILE *File, int* byteCounter)
{
  // Is throwing the chunk size away okay?
  ReadInt(File, byteCounter);
  ReadInt(File, byteCounter);

  int chunkX = ReadInt(File, byteCounter);
  int chunkY = ReadInt(File, byteCounter);
  int chunkZ = ReadInt(File, byteCounter);

  chunk_dimension Result = Chunk_Dimension(chunkX, chunkY, chunkZ);
  return Result;
}

inline int
ReadPackChunk(FILE *File, int* byteCounter)
{
  // Is throwing the chunk size away okay?
  ReadInt(File, byteCounter);
  ReadInt(File, byteCounter);

  int nChunks = ReadInt(File, byteCounter);

  return nChunks;
}

inline int
ReadXYZIChunk(FILE *File, int* byteCounter)
{
  // This is the size of teh XYZI chunk .. should always be 1 int (nVoxels)
  ReadInt(File, byteCounter);
  ReadInt(File, byteCounter);

  int nVoxels = ReadInt(File, byteCounter);
  return nVoxels;
}

chunk_data*
LoadVox(memory_arena *WorldStorage, char const *filepath, entity *Entity )
{
  chunk_data *Result = 0;
  s32 totalChunkBytes = 0;

  FILE *ModelFile = fopen(filepath, "r");

  if (ModelFile)
  {
    // Ensure we're dealing with a .vox file
    ReadVoxChunk(ModelFile);

    totalChunkBytes = ReadMainChunk(ModelFile);
    int bytesRemaining = totalChunkBytes;

    // TODO(Jesse) : Actually read all the data!
    while (bytesRemaining > 0)
    {
      /* Log("%d\n", bytesRemaining); */


      Chunk_ID CurrentId = GetHeaderType(ModelFile, &bytesRemaining);
      switch ( CurrentId )
      {
        case ID_RGBA:
        {
        } break;

        case ID_PACK:
        {
          int nChunks = ReadPackChunk(ModelFile, &bytesRemaining);
          Log("nChunks %d\n", nChunks);
        } break;

        case ID_SIZE:
        {
          // Instead of trusting the model (they're always wrong), we'll loop
          // through the data and figure it out ourselves.

          chunk_dimension Dim = ReadSizeChunk(ModelFile, &bytesRemaining);
          Print(Dim);
        } break;

        case ID_XYZI:
        {
          s32 numVoxels = ReadXYZIChunk(ModelFile, &bytesRemaining);

          s32 maxX = 0, maxY = 0, maxZ = 0;
          s32 minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX;

          unpacked_voxel *LocalVoxelCache = (unpacked_voxel *)calloc(numVoxels, sizeof(unpacked_voxel) );
          for( s32 VoxelCacheIndex = 0;
               VoxelCacheIndex < numVoxels;
               ++VoxelCacheIndex)
          {
            s32 X = (s32)ReadChar(ModelFile, &bytesRemaining);
            s32 Y = (s32)ReadChar(ModelFile, &bytesRemaining);
            s32 Z = (s32)ReadChar(ModelFile, &bytesRemaining);
            s32 W = (s32)ReadChar(ModelFile, &bytesRemaining); // Color

            maxX = Max(X, maxX);
            maxY = Max(Y, maxY);
            maxZ = Max(Z, maxZ);

            minX = Min(X, minX);
            minY = Min(Y, minY);
            minZ = Min(Z, minZ);

            LocalVoxelCache[VoxelCacheIndex] = GetUnpackedVoxel(X,Y,Z,W);
          }

          chunk_dimension Min = Chunk_Dimension(minX, minY, minZ);

          // Add 1 to the max dimension to convert from indicies to positions
          chunk_dimension Max = Chunk_Dimension(maxX+1, maxY+1, maxZ+1);

          Entity->ModelDim = Max - Min;;

          Print(Entity->ModelDim);

          // TODO(Jesse): Load models in multiple chunks instead of one
          // monolithic one. The storage for chunks must be as large as the
          // largest chunk we will EVER load, which should definately not be
          // decided at compile time.
          Result = AllocateChunk(WorldStorage, Entity->ModelDim);
          if(!Result) { return False; }

          for( int i = 0; i < numVoxels; ++ i)
          {
            unpacked_voxel V = LocalVoxelCache[i];

            voxel_position RealP = V.Offset - Min;
            Result->Voxels[ GetIndex(RealP, Result, Entity->ModelDim) ] = PackVoxel(&V);;
          }

          free(LocalVoxelCache);

          Result->flags = SetFlag(Result->flags, Chunk_Initialized);

          // TODO(Jesse): Are we really done?
          goto loaded;
        } break;

        InvalidDefaultCase;
      }
    }


  }
  else
  {
    Error("Couldn't read model file '%s' .", filepath);
  }

loaded:
  return Result;
}
