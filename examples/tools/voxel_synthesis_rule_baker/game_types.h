
struct game_state
{
  random_series Entropy;
  voxel_synthesis_result BakeResult;

  voxel_synthesis_gen_info InitInfo;
  voxel_synthesis_gen_info GenInfo;

  entity *BakeEntity; // Entity that has the original mesh attached to it.

};

