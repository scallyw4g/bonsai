// src/engine/voxel_synthesis.h:321:0

link_internal voxel_synthesis_change_propagation_info
VoxelSynthesisChangePropagationInfo( tile_rule  PrevTileOptions , v3i  PrevTileP , v3i  DirOfTravel  )
{
  voxel_synthesis_change_propagation_info Reuslt = {
    .PrevTileOptions = PrevTileOptions,
    .PrevTileP = PrevTileP,
    .DirOfTravel = DirOfTravel
  };
  return Reuslt;
}

