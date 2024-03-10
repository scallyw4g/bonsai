// src/engine/world_chunk.cpp:4231:0

DimIterator(x, y, z, UpdateDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceUpdateBounds.Min;
  V = CopiedChunk->Voxels + GetIndex(SimRelVoxP, UpdateDim);

  

        if ((V->Flags&Voxel_Filled)==False && Contains(SSRect, SimVoxP))
        {
          b32 IsUnfilledBorder = False;
          poof_check_for_unfilled_border()
          Assert(NewVoxelValue->Flags & Voxel_Filled);
          if (IsUnfilledBorder) { *V = *NewVoxelValue; }
        }
      
}


