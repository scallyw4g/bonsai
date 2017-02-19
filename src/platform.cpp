#include <stdio.h>
#include <string.h>
#include <bonsai.h>
#include <math.h>
#include <types.h>
#include <assert.h>

// Number of bytes per int according to .vox file format
#define VOX_INT_BYTES 4
#define VOX_CHAR_BYTES 1

constexpr int
MV_ID( const int a, const int b, const int c, const int d ) {
  return ( a ) | ( b << 8 ) | ( c << 16 ) | ( d << 24 );
}

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

  assert( ID == ID_VOX  ||
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
  assert( ID == ID_VOX );

  Version = ReadInt(File);
  assert(Version == 150);

  return;
}

inline int
ReadMainChunk(FILE *File)
{
  int ID = ReadInt(File);
  assert( ID == ID_MAIN );

  int selfSize = ReadInt(File);
  assert(selfSize == 0 );

  int childSize = ReadInt(File);
  assert(childSize > 0 );

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

Chunk
LoadVox(char const *filepath)
{
  Chunk Result;
  int totalChunkBytes;

  FILE * ModelFile = fopen(filepath, "r");
  if (ModelFile)
  {
    // Ensure we're dealing with a .vox file
    ReadVoxChunk(ModelFile);

    totalChunkBytes = ReadMainChunk(ModelFile);

    int bytesRemaining = totalChunkBytes;

    // TODO(Jesse) : Actually read all the data!
    while (bytesRemaining > 0)
    {
      /* printf("%d\n", bytesRemaining); */

      switch ( GetHeaderType(ModelFile, &bytesRemaining) )
      {
        case ID_RGBA:
        {
        } break;

        case ID_PACK:
        {
          /* int nChunks = */ ReadPackChunk(ModelFile, &bytesRemaining);
          /* printf("nChunks %d\n", nChunks); */
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

          Voxel *LocalVoxelCache = (Voxel *)calloc(numVoxels, sizeof(Voxel) );
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

            LocalVoxelCache[i] = GetVoxel(X,Y,Z,W);
            /* printf("%d\n", W); */
          }


          v3 Min = V3(minX, minY, minZ);
          chunk_dimension Dim = Chunk_Dimension(maxX+1, maxY+1, maxZ+1) - Min;

          Result = AllocateChunk(Dim);

          for( int i = 0; i < numVoxels; ++ i)
          {
            Voxel V = {};
            V.flags = SetFlag(V.flags, Voxel_Filled);

            voxel_position RealP = GetVoxelP(LocalVoxelCache[i])-Min;
            V = SetVoxelP(V, RealP);

            V = SetVoxelColor(V, GetVoxelColor(LocalVoxelCache[i]));

            int Index = GetIndex( RealP, &Result);
            Result.Voxels[Index] = V;

            assert(GetVoxelColor(V) < PALETTE_SIZE);

            /* printf("%d\n", GetVoxelColor(V)); */
            /* assert(GetVoxelColor(*V) == 121); */
          }

          free(LocalVoxelCache);

          goto loaded;
        } break;

        InvalidDefaultCase;
      }
    }


  }
  else
  {
    printf("Couldn't read model file '%s' .", filepath);
  }

loaded:
  return Result;
}
