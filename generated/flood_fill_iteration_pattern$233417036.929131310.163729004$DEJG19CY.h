// callsite
// src/engine/world_update.cpp:485:0

// def (flood_fill_iteration_pattern)
// src/engine/world_update.cpp:208:0
random_series ColorEntropy = {4654376543246};

NotImplemented;

voxel *V = {};
/* // TODO(Jesse): Do we want to try and keep the amount of temp memory to a minimum here? */
/* voxel_stack_element_cursor Stack = VoxelStackElementCursor(umm(TotalVoxels*6), Thread->TempMemory); */

/* // Unfortunately, can't #if this out in a poof function.  Should probably */
/* // put it on a #define switch to make sure it gets compiled out. */
/* DEBUG_AssertVoxelFloodStartsInEmptyVoxel(FloodOrigin, &SimSpaceUpdateBounds, CopiedChunk->Voxels); */

/* Push(&Stack, VoxelStackElement(FloodOrigin, VoxelRuleDir_Count)); */
/* while (AtElements(&Stack)) */
/* { */
/*   b32 OverwriteVoxel = False; */

/*   voxel_stack_element Element = Pop(&Stack); */
/*   v3i SimVoxP = Element.VoxSimP + AllDirections[Element.Dir]; */
/*   v3i RelVoxP = SimVoxP - SimSpaceUpdateBounds.Min; */

/*   s32 VoxelIndex = TryGetIndex(RelVoxP, UpdateDim); */
/*   if (VoxelIndex > -1) */
/*   { */
/*     V = CopiedChunk->Voxels+VoxelIndex; */

/*     v3i CenterToVoxP = SimVoxP - FloodOrigin; */

/*     (FloodPredicate) */
/*     { */
/*       NotImplemented; */
/*       /1* if ( (V->Flags & Voxel_MarkBit) == 0) *1/ */
/*       { */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ)); */
/*       } */
/*     } */

/*     (UserCode) */

/*       NotImplemented; */
/*     /1* V->Flags |= Voxel_MarkBit; *1/ */

/*     if ( ((OverwriteVoxel == True)  && (Invert == False)) || */
/*          ((OverwriteVoxel == False) && (Invert == True))  ) */
/*     { */
/*       /1* if (Mode == WorldEdit_Mode_Paint) *1/ */
/*       /1* { *1/ */
/*       /1*   V->Color = NewVoxelValue->Color; *1/ */
/*       /1* } *1/ */
/*       /1* else *1/ */
/*       { */
/*         *V = *NewVoxelValue; */
/*       } */
/*     } */
/*   } */
/* } */

/* Push(&Stack, VoxelStackElement(FloodOrigin, VoxelRuleDir_Count)); */
/* while (AtElements(&Stack)) */
/* { */
/*   voxel_stack_element Element = Pop(&Stack); */
/*   v3i Dir = AllDirections[Element.Dir]; */
/*   { */
/*     v3i SimVoxP = Element.VoxSimP + Dir; */
/*     v3i RelVoxP = SimVoxP - SimSpaceUpdateBounds.Min; */

/*     s32 VoxelIndex = TryGetIndex(RelVoxP, UpdateDim); */

/*     if (VoxelIndex > -1) */
/*     { */
/*       V = CopiedChunk->Voxels+VoxelIndex; */

/*       (UserCode2) */

/*       NotImplemented; */
/*       /1* if ( (V->Flags&Voxel_MarkBit)) *1/ */
/*       { */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ)); */
/*         Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ)); */
/*       } */

/*       NotImplemented; */
/*       /1* V->Flags &= ~Voxel_MarkBit; *1/ */
/*     } */
/*   } */
/* } */

