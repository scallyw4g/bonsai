// src/engine/world_chunk.cpp:4614:0

DimIterator(x, y, z, UpdateDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceUpdateBounds.Min;
  V = CopiedChunk->Voxels + GetIndex(SimRelVoxP, UpdateDim);

  
                if (Contains(SSRect, SimVoxP)) { if (V->Flags & Voxel_Filled) { V->Color = NewColor; } }
              
}


