// src/engine/world_chunk.cpp:4592:0

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
        if (NewVoxelValue && (NewVoxelValue->Flags&Voxel_Filled))
        {
          OverwriteVoxel = True;
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
        voxel NullV = {};
        if (Mode == WorldEdit_Mode_Remove) { *V = {}; }
        else { *V = *NewVoxelValue; }
      }

      // Knock out face flags so the 'surface' algorithm doesn't "self-apply"
      // We recompute these, so it's fine there.  It's slower on non-surface
      // paths, but .. when that's the bottleneck, we've won.
      V->Flags = voxel_flag(V->Flags&~VoxelFaceMask);
    }
  }
}


