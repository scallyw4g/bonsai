// src/engine/world_chunk.cpp:4751:0

DimIterator(x, y, z, UpdateDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceUpdateBounds.Min;
  V = CopiedChunk->Voxels + GetIndex(SimRelVoxP, UpdateDim);

  
              v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
              voxel *AssetV = TryGetVoxel(Data, OriginToCurrentVoxP);
              if (AssetV && (AssetV->Flags&Voxel_Filled)) { V->Color = AssetV->Color; }
            
}


