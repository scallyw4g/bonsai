// src/engine/world_chunk.cpp:4217:0

DimIterator(x, y, z, UpdateDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceUpdateBounds.Min;
  voxel *V = CopiedChunk->Voxels + GetIndex(SimRelVoxP, UpdateDim);

  
        if ( (V->Flags&VoxelFaceMask) && Contains(SSRect, SimVoxP)) { V->Color = NewVoxelValue->Color; }
      
}


