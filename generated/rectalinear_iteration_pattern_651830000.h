// src/engine/world_chunk.cpp:4713:0

DimIterator(x, y, z, UpdateDim)
{
  b32 OverwriteVoxel = False;

  v3i VoxP = V3i(x,y,z);
  v3i SimVoxP = VoxP + SimSpaceUpdateBounds.Min;
  voxel *V = CopiedChunk->Voxels + GetIndex(VoxP, UpdateDim);

  
                v3i CenterToVoxP = SimVoxP - EditCenterP;
                if (LengthSq(CenterToVoxP) < RadiusSquared) { if (V->Flags & Voxel_Filled) { V->Color = NewColor; } }
              

  if ( ((OverwriteVoxel == True)  && (Invert == False)) ||
    ((OverwriteVoxel == False) && (Invert == True))  )
  {
    if (Mode == WorldEdit_Mode_Paint)
    {
      V->Color = NewVoxelValue->Color;
    }
    else
    {
      *V = *NewVoxelValue;
    }
  }
}


