// src/engine/work_queue.h:251:0

link_internal void
RectilinearWorldUpdate_WorldEdit_Mode_Attach_WorldEdit_Modifier_None_type_world_update_op_shape_params_rect
(          world_edit_mode   Mode,
  world_edit_mode_modifier   Modifier,
  world_update_op_shape  *Shape,
  world_chunk **ChunkBuffer,
  u32   ChunkCount,
  rect3i  *SimSpaceQueryAABB,
  voxel  *CopiedVoxels )
{

  Assert(Mode == WorldEdit_Mode_Attach);
  Assert(Modifier == WorldEdit_Modifier_None);
  Assert(Shape->Type == type_world_update_op_shape_params_rect);

  /* v3i SimSpaceQueryMinP = V3i(SimSpaceQueryAABB->Min); */
  v3i SimSpaceQueryDim = GetDim(*SimSpaceQueryAABB);
  Assert(SimSpaceQueryDim.x % Global_StandingSpotDim.x == 0);
  Assert(SimSpaceQueryDim.y % Global_StandingSpotDim.y == 0);

  DimIterator(x, y, z, SimSpaceQueryDim)
  {
    v3i SimRelVoxP = V3i(x,y,z);
    /* v3i SimVoxP = SimRelVoxP + SimSpaceQueryAABB.Min; */
    voxel *V = CopiedVoxels + GetIndex(SimRelVoxP, SimSpaceQueryDim);

    
    world_update_op_shape_params_rect *Rect = SafeCast(world_update_op_shape_params_rect, Shape);
    rect3i SSRect = {V3i(Rect->P0), V3i(Rect->P1)};

    /* voxel NewVoxelValue = { Voxel_Filled, NewTransparency, NewColor}; */
    /* if (Contains(SSRect, SimVoxP)) { *V = NewVoxelValue; } */
  
  }
}

// Unfortunately, the C compiler is too stupid to do this.  I'm leaving
// this here such that when adding a new callback you can manually paste
// this line into Initialize_Global_UpdateWorldCallbackTable()
//
static const char* lolwut_StringHack_WorldEdit_Mode_Attach_WorldEdit_Modifier_None_type_world_update_op_shape_params_rect = "Global_WorldUpdateCallbackTable[WorldEdit_Mode_Attach][WorldEdit_Modifier_None][type_world_update_op_shape_params_rect] = RectilinearWorldUpdate_WorldEdit_Mode_Attach_WorldEdit_Modifier_None_type_world_update_op_shape_params_rect;";

