// src/engine/world_chunk.cpp:4315:0

DimIterator(x, y, z, UpdateDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceUpdateBounds.Min;
  V = CopiedChunk->Voxels + GetIndex(SimRelVoxP, UpdateDim);

  

        v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
        voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP);

        if ((V->Flags&Voxel_Filled)==False && Contains(SSRect, SimVoxP))
        {
          b32 IsUnfilledBorder = False;
          poof_check_for_unfilled_border()
          if (IsUnfilledBorder)
          {
            *V = *NewVoxelValue;
            V->Flags = voxel_flag(V->Flags&~VoxelFaceMask); // Knock out face flags so the algorithm doesn't "self-apply"
          }
        }
      
}


