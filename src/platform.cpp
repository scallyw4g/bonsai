#include <stdio.h>
#include <string.h>
#include <bonsai.h>

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

unsigned char
ReadChar(FILE* File)
{
  unsigned char c;
  fread(&c, 1, VOX_CHAR_BYTES, File);
  return c;
}

int
ReadInt(FILE* File)
{
  int i;
  fread(&i, 1, VOX_INT_BYTES, File);
  return i;
}

Chunk_ID
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

void
ReadVoxChunk(FILE *File)
{
  int ID, Version = INT_MAX;

  ID = ReadInt(File);
  assert( ID == ID_VOX );

  Version = ReadInt(File);
  assert(Version == 150);

  return;
}

int
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

chunk_dimension
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

void
ReadPackChunk(FILE *File)
{
  // TODO(Jesse) : What is this chunk for?
  ReadInt(File);
  ReadInt(File);
  ReadInt(File);

  return;
}

int
ReadXYZIChunk(FILE *File)
{
  // Is throwing the chunk size away okay?
  ReadInt(File);
  ReadInt(File);

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
          ReadPackChunk(ModelFile);
          bytesRemaining -= VOX_INT_BYTES *4;
        } break;

        case ID_SIZE:
        {
          chunk_dimension Dim = ReadSizeChunk(ModelFile);
          printf(" Chunk Allocated : x %d y %d z %d\n", Dim.x, Dim.y, Dim.z);

          Result = AllocateChunk(Dim, World_Position(0,0,0));
          bytesRemaining -= VOX_INT_BYTES *5;
        } break;

        case ID_XYZI:
        {
          int numVoxels = ReadXYZIChunk(ModelFile);
          printf(" Voxels in model : %d \n", numVoxels);

          bytesRemaining -= VOX_INT_BYTES *2;

          for( int i = 0; i < numVoxels; ++ i)
          {
            unsigned char X = ReadChar(ModelFile);
            unsigned char Y = ReadChar(ModelFile);
            unsigned char Z = ReadChar(ModelFile);
            /* unsigned char color = */ ReadChar(ModelFile);

            bytesRemaining -= VOX_CHAR_BYTES *4;

            printf("Filled: x %d, y %d, z %d\n", X, Y, Z);

            int Index = GetIndex( Voxel_Position(X, Y, Z), &Result);
            assert(Index < Volume(Result.Dim));
            Result.Voxels[Index] = SetVoxelP( Result.Voxels[Index], Voxel_Position(X,Y,Z) );
            Result.Voxels[Index].flags = SetFlag(Result.Voxels[Index].flags, Voxel_Filled);

          }
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
