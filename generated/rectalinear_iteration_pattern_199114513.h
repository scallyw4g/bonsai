// src/engine/world_chunk.cpp:4635:0

DimIterator(x, y, z, UpdateDim)
{
  b32 OverwriteVoxel = False;

  v3i VoxP = V3i(x,y,z);
  v3i SimVoxP = VoxP + SimSpaceUpdateBounds.Min;
  voxel *V = CopiedChunk->Voxels + GetIndex(VoxP, UpdateDim);

  
            v3i CenterToVoxP = SimVoxP - EditCenterP;
            if (LengthSq(CenterToVoxP) < RadiusSquared)
            {
              OverwriteVoxel = True;
            }
          

  if ( ((OverwriteVoxel == True)  && (Invert == False)) ||
    ((OverwriteVoxel == False) && (Invert == True))  )
  {
    *V = *NewVoxelValue;
  }
}


