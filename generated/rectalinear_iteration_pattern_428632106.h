// src/engine/world_chunk.cpp:4514:0

DimIterator(x, y, z, UpdateDim)
{
  b32 OverwriteVoxel = False;

  v3i VoxP = V3i(x,y,z);
  v3i SimVoxP = VoxP + SimSpaceUpdateBounds.Min;
  voxel *V = CopiedChunk->Voxels + GetIndex(VoxP, UpdateDim);

  
        v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
        voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP);
        if (NewVoxelValue && (NewVoxelValue->Flags&Voxel_Filled)) { V->Flags = Voxel_Empty; }
      

  if ( ((OverwriteVoxel == True)  && (Invert == False)) ||
    ((OverwriteVoxel == False) && (Invert == True))  )
  {
    *V = *NewVoxelValue;
  }
}


