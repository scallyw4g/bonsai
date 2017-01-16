#include <stdio.h>
#include <string.h>
#include <bonsai.h>

// Number of bytes per int according to .vox file format
#define VOX_INT_BYTES 4

#define VOX_ID_LENGTH 4

enum Chunk_HeaderType
{
  Header_Uninitialized,

  Header_PACK,
  Header_SIZE,
  Header_XYZI
};

Chunk_HeaderType
GetHeaderType(FILE* File)
{
  fpos_t initialP;
  fgetpos( File, &initialP);

  char chunkID[VOX_ID_LENGTH+1] = {};

  fsetpos( File, &initialP);

  fread(chunkID, 1, VOX_ID_LENGTH, File);

  if (memcmp(chunkID, "PACK", VOX_ID_LENGTH) == 0 )
  {
    return Header_PACK;
  }
  else if (memcmp(chunkID, "SIZE", VOX_ID_LENGTH) == 0 )
  {
    return Header_SIZE;
  }
  else if (memcmp(chunkID, "XYZI", VOX_ID_LENGTH) == 0 )
  {
    return Header_XYZI;
  }
  else
  { assert(false); }

  return Header_Uninitialized;
}
void
ReadChunkHeader(char *header, FILE* File)
{
  fread(header, 1, VOX_ID_LENGTH, File);
  return;
}

void
ReadVoxChunk(FILE *File)
{
  char chunkID[VOX_ID_LENGTH+1] = {};
  int version = INT_MAX;

  ReadChunkHeader( chunkID, File);
  assert( memcmp(chunkID, "VOX ", VOX_ID_LENGTH) == 0 );
  fread(&version, VOX_INT_BYTES, 1, File);

  return;
}

void
ReadMainChunk(FILE *File, int *chunkBytes, int *childBytes)
{
  char chunkID[VOX_ID_LENGTH+1] = {};

  *chunkBytes = INT_MAX;
  *childBytes = INT_MAX;

  ReadChunkHeader( chunkID, File);
  assert( memcmp(chunkID, "MAIN", VOX_ID_LENGTH) == 0 );

  fread(chunkBytes, VOX_INT_BYTES, 1, File);
  assert( *chunkBytes != INT_MAX );

  fread( childBytes, VOX_INT_BYTES, 1, File);
  assert( *childBytes != INT_MAX );

  return;
}

chunk_dimension
ReadSizeChunk(FILE *File)
{
  char chunkID[VOX_ID_LENGTH+1] = {};

  int  size, sizeChild, chunkX, chunkY, chunkZ = INT_MAX;

  ReadChunkHeader(chunkID, File);
  assert( memcmp(chunkID, "SIZE", VOX_ID_LENGTH) == 0 );

  fread(&size, VOX_INT_BYTES, 1, File);
  fread(&sizeChild, VOX_INT_BYTES, 1, File);

  fread(&chunkX, VOX_INT_BYTES, 1, File);
  fread(&chunkY, VOX_INT_BYTES, 1, File);
  fread(&chunkZ, VOX_INT_BYTES, 1, File);

  chunk_dimension Result = Chunk_Dimension(chunkX+1, chunkY+1, chunkZ+1);

  return Result;
}

void
ReadPackChunk(FILE *File)
{
  char chunkID[VOX_ID_LENGTH+1] = {};

  ReadChunkHeader( chunkID, File);
  assert( memcmp(chunkID, "PACK", VOX_ID_LENGTH) == 0 );

  // TODO(Jesse) : What is this chunk for?
  int Gargbage;
  fread(&Gargbage, VOX_INT_BYTES, 1, File);
  fread(&Gargbage, VOX_INT_BYTES, 1, File);
  fread(&Gargbage, VOX_INT_BYTES, 1, File);

  return;
}

int
ReadXYZIChunk(FILE *File)
{
  char chunkID[VOX_ID_LENGTH+1] = {};

  int Result = INT_MAX;

  ReadChunkHeader(chunkID, File);
  assert( memcmp(chunkID, "XYZI", VOX_ID_LENGTH) == 0 );

  fread(&Result, VOX_INT_BYTES, 1, File);

  return Result;
}

Chunk
LoadVox(char const *filepath)
{
  Chunk Result;

  int chunkBytes = INT_MAX;
  int childChunkBytes = INT_MAX;

  FILE * ModelFile = fopen(filepath, "r");

  if (ModelFile)
  {
    // Ensure we're dealing with a .vox file
    ReadVoxChunk( ModelFile );
    ReadMainChunk( ModelFile, &chunkBytes, &childChunkBytes );
    ReadPackChunk( ModelFile);

    chunk_dimension Dim = ReadSizeChunk(ModelFile);
    Result = AllocateChunk(Dim, World_Position(0,0,0));

    unsigned char X, Y, Z, color;

    int numVoxels = ReadXYZIChunk(ModelFile);

    for( int i = 0; i < numVoxels; ++ i)
    {
      fread(&X, 1, 1, ModelFile);
      fread(&Y, 1, 1, ModelFile);
      fread(&Z, 1, 1, ModelFile);
      fread(&color, 1, 1, ModelFile);

      printf("Filled: x %d, y %d, z %d\n", X, Y, Z);

      int Index = GetIndex( Voxel_Position(X, Y, Z), &Result);
      assert(Index < Volume(Result.Dim));
      Result.Voxels[Index].flags = SetFlag(Result.Voxels[Index].flags, Voxel_Filled);

    }

    printf(" Dim %d %d %d\n", Dim.x, Dim.y, Dim.z);
    printf(" Voxels %d/%d \n", numVoxels, Dim.x*Dim.y*Dim.z);


  }
  else
  {
    printf("Couldn't read model file '%s' .", filepath);
  }

  return Result;
}
