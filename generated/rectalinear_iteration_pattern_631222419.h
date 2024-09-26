// src/engine/world_update.cpp:751:0

DimIterator(x, y, z, UpdateDim)
{
  b32 OverwriteVoxel = False;

  v3i VoxP = V3i(x,y,z);
  v3i SimVoxP = VoxP + SimSpaceUpdateBounds.Min;
  voxel *V = CopiedChunk->Voxels + GetIndex(VoxP, UpdateDim);

  if (Contains(SSRect, SimVoxP))
  {
    

        v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
        voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP);

        NotImplemented;
        /* if ((V->Flags&Voxel_Filled)==False) */
        {
          b32 IsUnfilledBorder = False;
          poof_check_for_unfilled_border();
          if (IsUnfilledBorder)
          {
            OverwriteVoxel = True;
          }
        }
      

    if ( ((OverwriteVoxel == True ) && (Invert == False)) ||
      ((OverwriteVoxel == False) && (Invert == True ))  )
    {
      if (Mode == WorldEdit_Mode_Paint)
      {
        V->Color = NewVoxelValue->Color;
      }
      else
      {
        if (Mode == WorldEdit_Mode_Remove) { *V = {}; }
        else { *V = *NewVoxelValue; }
      }

      // Knock out face flags so the 'surface' algorithm doesn't "self-apply"
      // We recompute these, so it's fine there.  It's slower on non-surface
      // paths, but .. when that's the bottleneck, we've won.
      NotImplemented;
      /* V->Flags = voxel_flag(V->Flags&~VoxelFaceMask); */
    }
  }
}


