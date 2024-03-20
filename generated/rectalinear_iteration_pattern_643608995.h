// src/engine/world_chunk.cpp:4275:0

DimIterator(x, y, z, UpdateDim)
{
  b32 OverwriteVoxel = False;

  v3i VoxP = V3i(x,y,z);
  v3i SimVoxP = VoxP + SimSpaceUpdateBounds.Min;
  voxel *V = CopiedChunk->Voxels + GetIndex(VoxP, UpdateDim);

  

        if ((V->Flags&Voxel_Filled)==False && Contains(SSRect, SimVoxP))
        {
          b32 IsUnfilledBorder = False;
          poof_check_for_unfilled_border()
          Assert(NewVoxelValue->Flags & Voxel_Filled);
          if (IsUnfilledBorder)
          {
            *V = *NewVoxelValue;
          }
        }
      

  if ( ((OverwriteVoxel == True)  && (Invert == False)) ||
    ((OverwriteVoxel == False) && (Invert == True))  )
  {
    *V = *NewVoxelValue;
  }
}


