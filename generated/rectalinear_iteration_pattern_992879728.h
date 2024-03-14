// src/engine/world_chunk.cpp:4371:0

DimIterator(x, y, z, UpdateDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceUpdateBounds.Min;
  voxel *V = CopiedChunk->Voxels + GetIndex(SimRelVoxP, UpdateDim);

  
        v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
        voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP);
        if (NewVoxelValue)
        {
          if ( (V->Flags&VoxelFaceMask) && (NewVoxelValue->Flags&Voxel_Filled) && Contains(SSRect, SimVoxP) )
          {
            *V = {};
          }
        }
      
}


