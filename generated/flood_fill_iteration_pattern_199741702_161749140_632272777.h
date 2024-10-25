// src/engine/world_update.cpp:570:0

random_series ColorEntropy = {4654376543246};

voxel *V = {};
// TODO(Jesse): Do we want to try and keep the amount of temp memory to a minimum here?
voxel_stack_element_cursor Stack = VoxelStackElementCursor(umm(TotalVoxels*6), Thread->TempMemory);

// Unfortunately, can't #if this out in a poof function.  Should probably
// put it on a #define switch to make sure it gets compiled out.
DEBUG_AssertVoxelFloodStartsInEmptyVoxel(FloodOrigin, &SimSpaceUpdateBounds, CopiedChunk->Voxels);

Push(&Stack, VoxelStackElement(FloodOrigin, VoxelRuleDir_Count));
while (AtElements(&Stack))
{
  b32 OverwriteVoxel = False;

  voxel_stack_element Element = Pop(&Stack);
  v3i SimVoxP = Element.VoxSimP + AllDirections[Element.Dir];
  v3i RelVoxP = SimVoxP - SimSpaceUpdateBounds.Min;

  s32 VoxelIndex = TryGetIndex(RelVoxP, UpdateDim);
  if (VoxelIndex > -1)
  {
    V = CopiedChunk->Voxels+VoxelIndex;

    v3i CenterToVoxP = SimVoxP - FloodOrigin;

    
      NotImplemented;
      /* if (LengthSq(CenterToVoxP) < RadiusSquared && (V->Flags&Voxel_Filled) == 0) */
    
    {
      NotImplemented;
      /* if ( (V->Flags & Voxel_MarkBit) == 0) */
      {
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ));
      }
    }

    
      NotImplemented;
      /* if ( Length(CenterToVoxP) < SquareRoot(RadiusSquared)-1.f && (V->Flags&Voxel_Filled) ) */
         /* { V->Flags = Voxel_Empty; } */
    

    NotImplemented;
    /* V->Flags |= Voxel_MarkBit; */

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
}

Push(&Stack, VoxelStackElement(FloodOrigin, VoxelRuleDir_Count));
while (AtElements(&Stack))
{
  voxel_stack_element Element = Pop(&Stack);
  v3i Dir = AllDirections[Element.Dir];
  {
    v3i SimVoxP = Element.VoxSimP + Dir;
    v3i RelVoxP = SimVoxP - SimSpaceUpdateBounds.Min;

    s32 VoxelIndex = TryGetIndex(RelVoxP, UpdateDim);

    if (VoxelIndex > -1)
    {
      V = CopiedChunk->Voxels+VoxelIndex;

      
      v3i CenterToVoxP = SimVoxP - EditCenterP;
      if (Length(CenterToVoxP) < SquareRoot(RadiusSquared)-1.f)
      {
        NotImplemented;
        /* if (V->Flags & Voxel_Filled) */
        {
          V->Color = MagicaVoxelDefaultPaletteToPackedHSV(SafeTruncateU8(RandomBetween((u32)MCV_GREY_5, &ColorEntropy, (u32)MCV_GREY_8+1)));
        }

      }
      else if (LengthSq(CenterToVoxP) < RadiusSquared)
      {
        V->Color = MagicaVoxelDefaultPaletteToPackedHSV(MCV_GREY_8);
      }
    

      NotImplemented;
      /* if ( (V->Flags&Voxel_MarkBit)) */
      {
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ));
        Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ));
      }

      NotImplemented;
      /* V->Flags &= ~Voxel_MarkBit; */
    }
  }
}

