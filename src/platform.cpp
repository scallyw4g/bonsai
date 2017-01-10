#include <stdio.h>
#include <string.h>
#include <bonsai.h>

// Number of bytes per int according to .vox file format
#define VOX_INT_BYTES 4

#define VOX_ID_LENGTH 4

Chunk
LoadVox(char const *filepath)
{
  Chunk Result;

  char chunkID[VOX_ID_LENGTH+1] = {};

  int version = INT_MAX;

  int chunkBytes = INT_MAX;
  int childChunkBytes = INT_MAX;

  int numVoxels, size, sizeChild, chunkX, chunkY, chunkZ = INT_MAX;


  FILE * ModelFile = fopen(filepath, "r");

  if (ModelFile)
  {
    // Ensure we're dealing with a .vox file
    fread(&chunkID, 1, VOX_ID_LENGTH, ModelFile);
    assert( memcmp(chunkID, "VOX ", VOX_ID_LENGTH) == 0 );

    fread(&version, VOX_INT_BYTES, 1, ModelFile);

    fread(&chunkID, 1, VOX_ID_LENGTH, ModelFile);
    assert( memcmp(chunkID, "MAIN", VOX_ID_LENGTH) == 0 );

    fread(&chunkBytes, VOX_INT_BYTES, 1, ModelFile);
    assert( chunkBytes != INT_MAX );

    fread(&childChunkBytes, VOX_INT_BYTES, 1, ModelFile);
    assert( childChunkBytes != INT_MAX );

    fread(&chunkID, 1, VOX_ID_LENGTH, ModelFile);
    assert( memcmp(chunkID, "SIZE", VOX_ID_LENGTH) == 0 );

    fread(&size, VOX_INT_BYTES, 1, ModelFile);
    fread(&sizeChild, VOX_INT_BYTES, 1, ModelFile);

    fread(&chunkX, VOX_INT_BYTES, 1, ModelFile);
    fread(&chunkY, VOX_INT_BYTES, 1, ModelFile);
    fread(&chunkZ, VOX_INT_BYTES, 1, ModelFile);

    Result = AllocateChunk(Chunk_Dimension(chunkX+1, chunkY+1, chunkZ+1), World_Position(0,0,0));

    fread(&chunkID, 1, VOX_ID_LENGTH, ModelFile);
    assert( memcmp(chunkID, "XYZI", VOX_ID_LENGTH) == 0 );

    fread(&numVoxels, VOX_INT_BYTES, 1, ModelFile);
    unsigned char X, Y, Z, color;
    for( int i = 0; i < numVoxels; ++ i)
    {
      fread(&X, 1, 1, ModelFile);
      fread(&Y, 1, 1, ModelFile);
      fread(&Z, 1, 1, ModelFile);
      fread(&color, 1, 1, ModelFile);

      int Index = GetIndex( Voxel_Position(chunkX, chunkY, chunkZ), &Result);
      Result.Voxels[Index].flags = SetFlag(Result.Voxels[Index].flags, Voxel_Filled);
    }

    printf(" Dim %d %d %d\n", chunkX, chunkY, chunkZ);
    printf(" Voxels %d/%d \n", numVoxels, chunkX*chunkY*chunkZ);


  }
  else
  {
    printf("Couldn't read model file '%s' .", filepath);
  }

  return Result;
}
