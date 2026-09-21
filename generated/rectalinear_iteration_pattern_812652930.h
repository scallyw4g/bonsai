// callsite
// src/engine/world_update.cpp:521:0

// def (rectalinear_iteration_pattern)
// src/engine/world_update.cpp:310:0
NotImplemented;
/* DimIterator(x, y, z, UpdateDim) */
/* { */
/*   b32 OverwriteVoxel = False; */

/*   v3i VoxP = V3i(x,y,z); */
/*   v3i SimVoxP = VoxP + SimSpaceUpdateBounds.Min; */
/*   voxel *V = CopiedChunk->Voxels + GetIndex(VoxP, UpdateDim); */

/*   if (Contains(SSRect, SimVoxP)) */
/*   { */
/*     UserCode */

/*     if ( ((OverwriteVoxel == True ) && (Invert == False)) || */
/*          ((OverwriteVoxel == False) && (Invert == True ))  ) */
/*     { */
/*       /1* if (Mode == WorldEdit_Mode_Paint) *1/ */
/*       /1* { *1/ */
/*       /1*   V->Color = NewVoxelValue->Color; *1/ */
/*       /1* } *1/ */
/*       /1* else *1/ */
/*       { */
/*         if (Mode == WorldEdit_Mode_Subtractive) { *V = {}; } */
/*         else { *V = *NewVoxelValue; } */
/*       } */

/*       // Knock out face flags so the 'surface' algorithm doesn't "self-apply" */
/*       // We recompute these, so it's fine there.  It's slower on non-surface */
/*       // paths, but .. when that's the bottleneck, we've won. */
/*       NotImplemented; */
/*       /1* V->Flags = voxel_flag(V->Flags&~VoxelFaceMask); *1/ */
/*     } */
/*   } */
/* } */


