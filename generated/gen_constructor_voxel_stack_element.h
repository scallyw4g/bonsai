// src/engine/world_chunk.cpp:4026:0

link_internal voxel_stack_element
VoxelStackElement( v3i VoxSimP , voxel_rule_direction Dir  )
{
  voxel_stack_element Reuslt = {
    .VoxSimP = VoxSimP,
    .Dir = Dir
  };
  return Reuslt;
}

