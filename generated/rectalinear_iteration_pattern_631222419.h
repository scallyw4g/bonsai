// src/engine/world_chunk.cpp:4201:0

DimIterator(x, y, z, UpdateDim)
{
  v3i SimRelVoxP = V3i(x,y,z);
  v3i SimVoxP = SimRelVoxP + SimSpaceUpdateBounds.Min;
  V = CopiedChunk->Voxels + GetIndex(SimRelVoxP, UpdateDim);

  

        v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
        voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP);
        /* v3i OriginToCurrentVoxP = SimVoxP - SimOrigin; */
        /* voxel *AssetV = TryGetVoxel(Data, OriginToCurrentVoxP); */

        if ((V->Flags&Voxel_Filled)==False && Contains(SSRect, SimVoxP))
        {
          b32 IsUnfilledBorder = False;
          if (voxel *Vn = TryGetVoxel(CopiedChunk, SimRelVoxP + V3(1,0,0)))
          {
            if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
          }
          if (voxel *Vn = TryGetVoxel(CopiedChunk, SimRelVoxP + V3(-1,0,0)))
          {
            if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
          }

          if (voxel *Vn = TryGetVoxel(CopiedChunk, SimRelVoxP + V3(0,1,0)))
          {
            if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
          }
          if (voxel *Vn = TryGetVoxel(CopiedChunk, SimRelVoxP + V3(0,-1,0)))
          {
            if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
          }

          if (voxel *Vn = TryGetVoxel(CopiedChunk, SimRelVoxP + V3(0,0,1)))
          {
            if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
          }
          if (voxel *Vn = TryGetVoxel(CopiedChunk, SimRelVoxP + V3(0,0,-1)))
          {
            if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
          }

          /* if (AssetV && (AssetV->Flags&Voxel_Filled)) { *V = *AssetV; } */
          /* if ( NewVoxelValue && (NewVoxelValue->Flags&Voxel_Filled) && IsUnfilledBorder) { *V = *NewVoxelValue; } */
          if (IsUnfilledBorder)
          {
            *V = *NewVoxelValue;
            V->Flags = voxel_flag(V->Flags&~VoxelFaceMask); // Knock out face flags so the algorithm doesn't "self-apply"
          }
        }
      
}


