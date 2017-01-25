#include <stdio.h>
#include <string.h>
#include <bonsai.h>
#include <math.h>

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
ReadChar(FILE* File)
{
  unsigned char c;
  fread(&c, 1, VOX_CHAR_BYTES, File);
  return c;
}

inline int
ReadInt(FILE* File)
{
  int i;
  fread(&i, 1, VOX_INT_BYTES, File);
  return i;
}

inline Chunk_ID
GetHeaderType(FILE* File)
{
  int ID = ReadInt(File);

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
ReadSizeChunk(FILE *File)
{
  // Is throwing the chunk size away okay?
  ReadInt(File);
  ReadInt(File);

  int chunkX = ReadInt(File);
  int chunkY = ReadInt(File);
  int chunkZ = ReadInt(File);

  chunk_dimension Result = Chunk_Dimension(chunkX+1, chunkY+1, chunkZ+1);
  return Result;
}

inline int
ReadPackChunk(FILE *File)
{
  // Is throwing the chunk size away okay?
  ReadInt(File);
  ReadInt(File);

  int nChunks = ReadInt(File);

  return nChunks;
}

inline int
ReadXYZIChunk(FILE *File)
{
  // Is throwing the chunk size away okay?
  int size = ReadInt(File);
  printf("size1 %d\n", size);

  size = ReadInt(File);
  printf("size2 %d\n", size);

  int nVoxels = ReadInt(File);
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
    ReadVoxChunk(ModelFile );

    totalChunkBytes = ReadMainChunk(ModelFile);
    int bytesRemaining = totalChunkBytes;

    // TODO(Jesse) : Actually read all the data!
    while (bytesRemaining > 0)
    {
      switch ( GetHeaderType(ModelFile) )
      {
        bytesRemaining -= VOX_INT_BYTES;

        case ID_PACK:
        {
          int nChunks = ReadPackChunk(ModelFile);
          printf("nChunks %d\n", nChunks);
          bytesRemaining -= VOX_INT_BYTES *4;
        } break;

        case ID_SIZE:
        {
          /* chunk_dimension Dim = */ ReadSizeChunk(ModelFile);
          /* printf(" Chunk Allocated : x %d y %d z %d\n", Dim.x, Dim.y, Dim.z); */
          bytesRemaining -= VOX_INT_BYTES *5;
        } break;

        case ID_XYZI:
        {
          int numVoxels = ReadXYZIChunk(ModelFile);

          bytesRemaining -= VOX_INT_BYTES *2;

          int maxX = 0, maxY = 0, maxZ = 0;
          int minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX;

          Voxel *Voxels = (Voxel *)calloc(numVoxels, sizeof(Voxel) );
          for( int i = 0; i < numVoxels; ++ i)
          {
            int X = (int)ReadChar(ModelFile);
            int Z = (int)ReadChar(ModelFile);
            int Y = (int)ReadChar(ModelFile);
            /* unsigned char color = */ ReadChar(ModelFile);

            maxX = X > maxX ? X : maxX;
            maxY = Y > maxY ? Y : maxY;
            maxZ = Z > maxZ ? Z : maxZ;

            minX = X < minX ? X : minX;
            minY = Y < minY ? Y : minY;
            minZ = Z < minZ ? Z : minZ;

            Voxels[i] = GetVoxel(X,Y,Z);
          }


          v3 Min = V3(minX, minY, minZ);
          chunk_dimension Dim = Chunk_Dimension(maxX, maxY, maxZ) - Min;

          Result = AllocateChunk(Dim + Voxel_Position(1,1,1), World_Position(0,0,0));

          for( int i = 0; i < numVoxels; ++ i)
          {
            bytesRemaining -= VOX_CHAR_BYTES *4;

            int Index = GetIndex( GetVoxelP(Voxels[i])-Min, &Result);

            Voxel *V = &Result.Voxels[Index];
            V->flags = SetFlag(V->flags, Voxel_Filled);

          }

          free(Voxels);

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
