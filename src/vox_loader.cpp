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

  chunk_dimension Result = Chunk_Dimension(chunkX+1, chunkY+1, chunkZ+1);
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
LoadVox(platform *Plat, memory_arena *WorldStorage, char const *filepath, Entity *entity )
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

      switch ( GetHeaderType(ModelFile, &bytesRemaining) )
      {
        case ID_RGBA:
        {
        } break;

        case ID_PACK:
        {
          /* int nChunks = */ ReadPackChunk(ModelFile, &bytesRemaining);
          /* Log("nChunks %d\n", nChunks); */
        } break;

        case ID_SIZE:
        {
          // Instead of trusting the model (they're always wrong), we'll loop
          // through the data and figure it out ourselves.

          /* chunk_dimension Dim = */ ReadSizeChunk(ModelFile, &bytesRemaining);
        } break;

        case ID_XYZI:
        {
          int numVoxels = ReadXYZIChunk(ModelFile, &bytesRemaining);

          int maxX = 0, maxY = 0, maxZ = 0;
          int minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX;

          voxel *LocalVoxelCache = (voxel *)calloc(numVoxels, sizeof(voxel) );
          for( int i = 0; i < numVoxels; ++ i)
          {
            int X = (int)ReadChar(ModelFile, &bytesRemaining);
            int Z = (int)ReadChar(ModelFile, &bytesRemaining);
            int Y = (int)ReadChar(ModelFile, &bytesRemaining);
            int W = (int)ReadChar(ModelFile, &bytesRemaining); // Color


            maxX = X > maxX ? X : maxX;
            maxY = Y > maxY ? Y : maxY;
            maxZ = Z > maxZ ? Z : maxZ;

            minX = X < minX ? X : minX;
            minY = Y < minY ? Y : minY;
            minZ = Z < minZ ? Z : minZ;

            // Offset info the chunk by 1 to avoid having to call BuildExteriorBoundaryVoxels
            LocalVoxelCache[i] = GetVoxel(X+1,Y+1,Z+1,W);
          }

          v3 Min = V3((r32)minX, (r32)minY, (r32)minZ);

          // Add 3 to the max dimension because the max values are indicies,
          // and we need a 1 voxel buffer around the model because
          // BuildExteriorBoundaryVoxels doesn't handle models correctly.
          // +1 to convert from index, +2 for a 1vox buffer on each side.
          chunk_dimension Dim = Chunk_Dimension(maxX+3, maxY+3, maxZ+3) - Min;
          entity->ModelDim = Dim;

          // TODO(Jesse): Load models in multiple chunks instead of one
          // monolithic one. The storage for chunks must be as large as the
          // largest chunk we will EVER load, which should definately not be
          // decided at compile time.
          Result = AllocateChunk(Plat, WorldStorage, Dim);
          if(!Result) { return False; }

          for( int i = 0; i < numVoxels; ++ i)
          {
            voxel V = {};
            V.flags = SetFlag(V.flags, Voxel_Filled);

            voxel_position RealP = GetVoxelP(LocalVoxelCache[i])-Min;
            V = SetVoxelP(V, RealP);

            V = SetVoxelColor(V, GetVoxelColor(LocalVoxelCache[i]));

            Result->Voxels[GetIndex(RealP, Result, Dim)] = V;

            Assert(GetVoxelColor(V) < PALETTE_SIZE);

            /* Log("%d\n", GetVoxelColor(V)); */
            /* Assert(GetVoxelColor(*V) == 121); */
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
