// src/engine/world_chunk.cpp:4356:0

// TODO(Jesse): Do we want to try and keep the amount of temp memory to a minimum here?
voxel_stack_element_cursor Stack = VoxelStackElementCursor(umm(TotalVoxels*6), Thread->TempMemory);

// Unfortunately, can't #if this out in a poof function.  Should probably
// put it on a #define switch to make sure it gets compiled out.
/* DEBUG_AssertVoxelFloodStartsInEmptyVoxel(FloodOrigin, &SimSpaceQueryAABB, CopiedVoxels); */

Push(&Stack, VoxelStackElement(FloodOrigin, VoxelRuleDir_Count));
while (AtElements(&Stack))
{
  voxel_stack_element Element = Pop(&Stack);
  v3i SimVoxP = Element.VoxSimP + AllDirections[Element.Dir];
  v3i RelVoxP = SimVoxP - SimSpaceQueryAABB.Min;

  s32 VoxelIndex = TryGetIndex(RelVoxP, QueryDim);
  if (VoxelIndex > -1)
  {
    V = CopiedVoxels+VoxelIndex;

    v3i CenterToVoxP = SimVoxP - FloodOrigin;

     if (Contains(SSRect, SimVoxP) && ((V->Flags&Voxel_Filled) == (Voxel_Filled*(Mode==WorldEdit_Mode_Attach)))) 
    {
      if ( (V->Flags & Voxel_MarkBit) == 0)
      {
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ));
      }
    }

    
                    if ( Mode == WorldEdit_Mode_Attach &&
                        (V->Flags&Voxel_Filled) ) { }
                    else { if (Contains(SSRect, SimVoxP)) { *V = NewVoxelValue; } }
                  

    V->Flags |= Voxel_MarkBit;
  }
}

Push(&Stack, VoxelStackElement(FloodOrigin, VoxelRuleDir_Count));
while (AtElements(&Stack))
{
  voxel_stack_element Element = Pop(&Stack);
  v3i Dir = AllDirections[Element.Dir];
  {
    v3i SimVoxP = Element.VoxSimP + Dir;
    v3i RelVoxP = SimVoxP - SimSpaceQueryAABB.Min;

    s32 VoxelIndex = TryGetIndex(RelVoxP, QueryDim);

    if (VoxelIndex > -1)
    {
      V = CopiedVoxels+VoxelIndex;

      

      if ( (V->Flags&Voxel_MarkBit))
      {
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ));
      }

      V->Flags &= ~Voxel_MarkBit;
    }
  }
}

