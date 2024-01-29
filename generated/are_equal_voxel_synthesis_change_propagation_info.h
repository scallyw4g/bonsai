// src/engine/voxel_synthesis.h:0:324

link_internal b32
AreEqual(voxel_synthesis_change_propagation_info *Thing1, voxel_synthesis_change_propagation_info *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( voxel_synthesis_change_propagation_info ) );

  return Result;
}


