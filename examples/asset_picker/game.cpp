#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

/* global_variable debug_state *Global_DebugStatePointer; */

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  model *Result = Allocate(model, Memory, ModelIndex_Count);
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_bow.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_cat.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_fox.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_gumi.vox"); */
  /* Result[ModelIndex_Player_jp] = LoadVoxModel(Memory, Heap, "models/chr_jp.vox", GetTranArena()); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_knight.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_man.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_mom.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_old.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_poem.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_rain.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_sasami.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_sol.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_sword.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_tale.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_tama.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_tsurugi.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "../voxel-model/vox/monument/monu10.vox"); */
  /* Result[ModelIndex_Player] = LoadWorldChunk(Memory, Heap, "assets/world_chunk_1_0_0"); */

  Result[ModelIndex_Level] = LoadVoxModel(Memory, Heap, "models/trees/lanterns.vox", GetTranArena());

  return Result;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/asset_picker/assets");

  /* world_position WorldCenter = World_Position(1, 1, 3); */
  world_position WorldCenter = {};
  canonical_position PlayerSpawnP = Canonical_Position(Voxel_Position(0), WorldCenter);

  StandardCamera(Graphics->Camera, 10000.0f, 300.0f, PlayerSpawnP);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  GameState->Models = AllocateGameModels(GameState, Resources->Memory, Heap);

  GameState->Player = GetFreeEntity(EntityTable);
  SpawnStaticEntity(Plat, World, GameState->Models + ModelIndex_Level, GameState->Player, PlayerSpawnP, &GameState->Entropy);

  /* Resources->CameraTargetP = &GameState->Player->P; */

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}
