// src/engine/world_chunk.cpp:4177:0

DimIterator(x, y, z, UpdateDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceUpdateBounds.Min;
  V = CopiedChunk->Voxels + GetIndex(SimRelVoxP, UpdateDim);

  
                v3i CenterToVoxP = SimVoxP - EditCenterP;
                if (LengthSq(CenterToVoxP) < RadiusSquared) { V->Flags = Voxel_Empty; }
              
}

