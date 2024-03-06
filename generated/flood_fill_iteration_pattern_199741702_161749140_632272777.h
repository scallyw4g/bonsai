// src/engine/world_chunk.cpp:4246:0

// TODO(Jesse): Do we want to try and keep the amount of temp memory to a minimum here?
voxel_stack_element_cursor Stack = VoxelStackElementCursor(umm(TotalVoxels*6), Thread->TempMemory);

// Unfortunately, can't #if this out in a poof function.  Should probably
// put it on a #define switch to make sure it gets compiled out.
/* DEBUG_AssertVoxelFloodStartsInEmptyVoxel(EditCenterP, &SimSpaceQueryAABB, CopiedVoxels); */

Push(&Stack, VoxelStackElement(EditCenterP, VoxelRuleDir_Count));
while (AtElements(&Stack))
{
  voxel_stack_element Element = Pop(&Stack);
  v3i SimVoxP = Element.VoxSimP + AllDirections[Element.Dir];
  v3i RelVoxP = SimVoxP - SimSpaceQueryAABB.Min;

  s32 VoxelIndex = TryGetIndex(RelVoxP, QueryDim);
  if (VoxelIndex > -1)
  {
    V = CopiedVoxels+VoxelIndex;

    v3i CenterToVoxP = SimVoxP - EditCenterP;

    if ( LengthSq(CenterToVoxP) < RadiusSquared && (V->Flags&Voxel_Filled) == 0 )
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

    
                    if ( LengthSq(CenterToVoxP) < Square(Sphere->Radius-1.f) && V->Flags & Voxel_Filled )
                       { V->Flags = Voxel_Empty; }
                  

    V->Flags |= Voxel_MarkBit;
  }
}

Push(&Stack, VoxelStackElement(EditCenterP, VoxelRuleDir_Count));
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

      
                    v3i CenterToVoxP = SimVoxP - EditCenterP;
                    if (LengthSq(CenterToVoxP) < Square(Sphere->Radius-1.f))
                    {
                      if (V->Flags & Voxel_Filled)
                      {
                        V->Color = SafeTruncateU8(RandomBetween((u32)GREY_5, &ColorEntropy, (u32)GREY_8+1));
                      }

                    }
                    else if (LengthSq(CenterToVoxP) < RadiusSquared)
                    {
                      V->Color = GREY_8;
                    }
                  

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

