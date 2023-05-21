link_internal voxel_synthesis_change_propagation_info
VoxelSynthesisChangePropagationInfo( u64 PrevTileOptions , v3i PrevTileP , v3i DirOfTravel  )
{
  voxel_synthesis_change_propagation_info Reuslt = {
    .PrevTileOptions = PrevTileOptions,
    .PrevTileP = PrevTileP,
    .DirOfTravel = DirOfTravel
  };
  return Reuslt;
}

