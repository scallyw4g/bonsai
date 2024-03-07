// src/engine/world_chunk.cpp:4473:0

DimIterator(x, y, z, SimSpaceQueryDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceQueryAABB.Min;
  V = CopiedVoxels + GetIndex(SimRelVoxP, SimSpaceQueryDim);

  
                    v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
                    voxel *AssetV = TryGetVoxel(Data, OriginToCurrentVoxP);
                    if (AssetV && (AssetV->Flags&Voxel_Filled)) { V->Flags = Voxel_Empty; }
                  
}


