#define PLATFORM_GL_IMPLEMENTATIONS 1

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
  Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_jp.vox");
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

  return Result;
}

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  work_queue_entry_type Type = Entry->Type;
  switch (Type)
  {
    case type_work_queue_entry_noop: { InvalidCodePath(); } break;

    case type_work_queue_entry_init_asset:
    {
      InvalidCodePath();

#if 0
      volatile work_queue_entry_init_asset *Job = SafeAccess(work_queue_entry_init_asset, Entry);
      world_chunk *Chunk = Job->Chunk;

      if (ChunkIsGarbage(Chunk))
      {
      }
      else
      {
        // TODO(Jesse): Improve ergonomics here?
        counted_string AssetPath = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);
        const char* zAssetPath = GetNullTerminated(AssetPath, Thread->TempMemory);
        DeserializeChunk(zAssetPath, Chunk);
      }

      FinalizeChunkInitialization(Chunk);
#endif
    } break;

    case type_work_queue_entry_init_world_chunk:
    {
      volatile work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;

      if (ChunkIsGarbage(Chunk))
      {
      }
      else
      {
        counted_string AssetFilename = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);

        native_file AssetFile = OpenFile(AssetFilename, "r+b");
        if (AssetFile.Handle)
        {
          DeserializeChunk(&AssetFile, Chunk, Thread->MeshFreelist, Thread->PermMemory);
          CloseFile(&AssetFile);
          // fsync?
        }
      }

      FinalizeChunkInitialization(Chunk);

    } break;

    case type_work_queue_entry_copy_buffer:
    {
      volatile work_queue_entry_copy_buffer *CopyJob = SafeAccess(work_queue_entry_copy_buffer, Entry);
      DoCopyJob(CopyJob);
    } break;

    case type_work_queue_entry_copy_buffer_set:
    {
      TIMED_BLOCK("Copy Set");
      volatile work_queue_entry_copy_buffer_set *CopySet = SafeAccess(work_queue_entry_copy_buffer_set, Entry);
      for (u32 CopyIndex = 0; CopyIndex < CopySet->Count; ++CopyIndex)
      {
        volatile work_queue_entry_copy_buffer *CopyJob = CopySet->CopyTargets + CopyIndex;
        DoCopyJob(CopyJob);
      }
      END_BLOCK("Copy Set");
    } break;

  }
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES();

  entity *Player = GameState->Player;
  Player->Physics.Speed = 60.f;
  Player->Physics.Mass = 35.f;

  if ( IsGrounded(World, Player, World->VisibleRegion) && Hotkeys->Player_Jump )
  {
    Player->Physics.Force += V3(0.f, 0.f, 0.5f) * Player->Physics.Speed;
  }

  /* if (IsGrounded(World, Player, World->VisibleRegion))// && Hotkeys->Player_Jump) */
  /* { */
  /*   Player->Physics.Force += V3(0, 0, 1); */
  /* } */

  if (Hotkeys->Player_Spawn)
  {
    Unspawn(Player);
    SpawnPlayer(Plat, World, GameState->Models, Player,  Canonical_Position(V3(0,0,0), World_Position(0,0,2)), &GameState->Entropy);
    World->Center = World_Position(0, 0, 2);
  }


  return;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/asset_picker/assets");

  GameState->Noise = perlin_noise(DEBUG_NOISE_SEED);

  world_position WorldCenter = World_Position(1, 1, 3);
  canonical_position PlayerSpawnP = Canonical_Position(Voxel_Position(0), WorldCenter);

  StandardCamera(Graphics->Camera, 10000.0f, 300.0f, PlayerSpawnP);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateAndInitWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  GameState->Models = AllocateGameModels(GameState, Memory, Heap);

  GameState->Player = GetFreeEntity(EntityTable);
  SpawnPlayer(Plat, World, GameState->Models, GameState->Player, PlayerSpawnP, &GameState->Entropy);

  Resources->CameraTargetP = &GameState->Player->P;

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Thread->MeshFreelist = Thread->MeshFreelist;
  /* Thread->Noise = &GameState->Noise; */
}
