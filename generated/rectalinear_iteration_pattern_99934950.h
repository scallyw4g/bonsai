// src/engine/world_chunk.cpp:4315:0

DimIterator(x, y, z, UpdateDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceUpdateBounds.Min;
  voxel *V = CopiedChunk->Voxels + GetIndex(SimRelVoxP, UpdateDim);

  
        if (Contains(SSRect, SimVoxP)) { if (V->Flags & Voxel_Filled) { V->Color = NewVoxelValue->Color; } }
      
}


