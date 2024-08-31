// src/engine/voxel_synthesis.h:21:0

link_internal b32
operator==( tile_rule_id E1, tile_rule_id E2 )
{
  b32 Reuslt = E1.PageIndex == E2.PageIndex && E1.Bit == E2.Bit ;
  return Reuslt;
}

link_internal b32
operator!=( tile_rule_id E1, tile_rule_id E2 )
{
  b32 Reuslt = !(E1 == E2);
  return Reuslt;
}

