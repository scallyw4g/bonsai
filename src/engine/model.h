
enum model_index
{
  ModelIndex_None,

  ModelIndex_Enemy_Skeleton_Axe,
  ModelIndex_Enemy_Skeleton_Sword,
  ModelIndex_Enemy_Skeleton_Lasher,
  ModelIndex_Enemy_Skeleton_Archer,
  ModelIndex_Enemy_Skeleton_Spear,
  ModelIndex_Enemy_Skeleton_AxeArmor,
  ModelIndex_Enemy_Skeleton_Hounds,
  ModelIndex_Enemy_Skeleton_Horserider,
  ModelIndex_Enemy_Skeleton_Horsebanner,
  ModelIndex_Enemy_Skeleton_Shaman,
  ModelIndex_Enemy_Skeleton_Champion,
  ModelIndex_Enemy_Skeleton_ChampionChampion,
  ModelIndex_Enemy_Skeleton_Concubiner,
  ModelIndex_Enemy_Skeleton_King,

  /* ModelIndex_FirstEnemyModel = ModelIndex_Enemy_Skeleton_Axe, */
  /* ModelIndex_LastEnemyModel = ModelIndex_Enemy_Skeleton_King, */

  ModelIndex_Player_jp,
  ModelIndex_Player_bow,
  ModelIndex_Player_cat,
  ModelIndex_Player_fox,
  ModelIndex_Player_gumi,
  ModelIndex_Player_knight,
  ModelIndex_Player_man,
  ModelIndex_Player_mom,
  ModelIndex_Player_old,
  ModelIndex_Player_poem,
  ModelIndex_Player_rain,
  ModelIndex_Player_sasami,
  ModelIndex_Player_sol,
  ModelIndex_Player_sword,
  ModelIndex_Player_tale,
  ModelIndex_Player_tama,
  ModelIndex_Player_tsurugi,

  /* ModelIndex_FirstPlayerModel = ModelIndex_Player_jp, */
  /* ModelIndex_LastPlayerModel = ModelIndex_Player_tsurugi, */

  ModelIndex_Loot,
  ModelIndex_Projectile,
  ModelIndex_Proton,
  ModelIndex_Bitty0,
  ModelIndex_Bitty1,

  ModelIndex_Level,

  ModelIndex_Count,
};
poof(string_and_value_tables(model_index))
#include <generated/string_and_value_tables_model_index.h>


// TODO(Jesse): Remove
// Used when loading .vox files
/* struct vox_data */
/* { */
/*   u32 FilledCount; */
/*   chunk_data *ChunkData; */
/*   voxel *Voxels; */
/* }; */

struct model
{
  // TODO(Jesse): Do we actually not do this ..?  Do we ever even actually serialize these?
  gen_chunk *Gen; poof(@no_serialize)

  untextured_3d_geometry_buffer TransparentMesh;

  animation Animation;
  v3i Dim;
};

link_internal b32
Deserialize(u8_stream *Bytes, gpu_mapped_element_buffer *Element, memory_arena *Memory)
{
  NotImplemented;
  return 0;
}

/* poof(block_array(vox_data, {8})) */
/* #include <generated/block_array_vox_data_688856411.h> */

poof(buffer_t(model, u64))
#include <generated/buffer_model.h>

poof(block_array(model, {8}))
#include <generated/block_array_model_688856411.h>

poof(maybe(model_buffer))
#include <generated/maybe_model_buffer.h>


#if 0
link_internal voxel *
TryGetVoxel(chunk_data *ChunkData, v3i P)
{
  voxel *Result = 0;
  s32 Index = TryGetIndex(P, ChunkData->Dim);
  if (Index > -1) { Result = ChunkData->Voxels + Index; }
  return Result;
}
#endif

struct maybe_model
{
  maybe_tag Tag;
  model Model;
};
