#define PLATFORM_GL_IMPLEMENTATIONS 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  model *Result = Allocate(model, GameState->Memory, ModelIndex_Count);
  Result[ModelIndex_Player] = LoadVoxModel(Memory, &GameState->Heap, "models/chr_old.vox");
  Result[ModelIndex_Proton] = LoadVoxModel(Memory, &GameState->Heap, PROJECTILE_MODEL);

  return Result;
}

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  switch (Entry->Type)
  {
    case type_work_queue_entry_noop: { InvalidCodePath(); } break;

    case type_work_queue_entry_init_asset:
    {
      volatile work_queue_entry_init_asset *Job = SafeAccess(work_queue_entry_init_asset, Entry);
      world_chunk *Chunk = Job->Chunk;
      Chunk->Data->Flags = Chunk_MeshComplete;
#if 0
      s32 Amplititude = 7;
      s32 StartingZDepth = 0;
      InitializeWorldChunkPerlinPlane( Thread,
                                       Chunk,
                                       WORLD_CHUNK_DIM,
                                       Amplititude,
                                       StartingZDepth );
      volatile work_queue_entry_init_asset *Job = SafeAccess(work_queue_entry_init_asset, Entry);
      world_chunk *Chunk = Job->Chunk;
      /* const char* zAssetPath = Chunk->zAssetPath; */

      // TODO(Jesse): Improve ergonomics here?
      counted_string AssetPath = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);
      const char* zAssetPath = GetNullTerminated(AssetPath, Thread->TempMemory);

      DeserializeChunk(zAssetPath, Chunk);

      MarkChunkInitialized(Chunk);
#endif
    } break;

    case type_work_queue_entry_init_world_chunk:
    {
      volatile work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;
      s32 Amplititude = 7;
      s32 StartingZDepth = 0;
      InitializeWorldChunkPerlinPlane( Thread,
                                       Chunk,
                                       WORLD_CHUNK_DIM,
                                       Amplititude,
                                       StartingZDepth );
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
        volatile work_queue_entry_copy_buffer *CopyJob = &CopySet->CopyTargets[CopyIndex];
        DoCopyJob(CopyJob);
      }
      END_BLOCK("Copy Set");
    } break;

  }

  return;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();

#if BONSAI_DEBUG_SYSTEM_API
  if (!GetDebugState) { GetDebugState = (get_debug_state_proc)Plat->GetDebugStateProc; }
  GetDebugState()->Plat = GameState->Plat;
  GetDebugState()->GameState = GameState;
#endif

  world                 *World         = GameState->World;
  graphics              *Graphics      = GameState->Plat->Graphics;
  g_buffer_render_group *gBuffer       = Graphics->gBuffer;
  ao_render_group       *AoGroup       = Graphics->AoGroup;
  camera                *Camera        = Graphics->Camera;

  Graphics->GpuBufferWriteIndex = (Graphics->GpuBufferWriteIndex + 1) % 2;
  gpu_mapped_element_buffer* GpuMap = Graphics->GpuBuffers + Graphics->GpuBufferWriteIndex;

  MapGpuElementBuffer(GpuMap);

  entity *Player = GameState->Player;
  ClearFramebuffers(Graphics);

#if DEBUG_DRAW_WORLD_AXIES
  {
    untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_LINE*3);
    DEBUG_DrawLine(&CopyDest, V3(0,0,0), V3(10000, 0, 0), RED, 0.2f );
    DEBUG_DrawLine(&CopyDest, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.2f );
    DEBUG_DrawLine(&CopyDest, V3(0,0,0), V3(0, 0, 10000), BLUE, 0.2f );
  }
#endif

  if (Hotkeys->Player_Spawn)
  {
    Unspawn(Player);
    SpawnPlayer(GameState->Models, Player,  Canonical_Position(V3(0,0,0), World_Position(0,0,0)), &GameState->Entropy);
    World->Center = World_Position(0, 0, 0);
  }

  TIMED_BLOCK("Game Simulation");

  CollectUnusedChunks(World, &GameState->MeshFreelist, GameState->Memory, g_VisibleRegion);

  v2 MouseDelta = GetMouseDelta(Plat);
  input* GameInput = &Plat->Input;

#if 0 // BONSAI_DEBUG_SYSTEM_API
  if (GetDebugState()->UiGroup.PressedInteractionId != StringHash("GameViewport"))
  {
    GameInput = 0;
  }
#endif

  UpdateGameCamera(MouseDelta, GameInput, Player->P, Camera, World->ChunkDim);
  SimulateEntities(World, GameState->EntityTable, Plat->dt, g_VisibleRegion, Camera, Hotkeys);
  SimulateAndRenderParticleSystems(GameState->EntityTable, World->ChunkDim, &GpuMap->Buffer, Graphics, Plat->dt);

  END_BLOCK("Game Simulation");

  gBuffer->ViewProjection =
    ProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    ViewMatrix(World->ChunkDim, Camera);

  DEBUG_COMPUTE_PICK_RAY(Plat, &gBuffer->ViewProjection);

  TIMED_BLOCK("BufferMeshes");
    BufferWorld(Plat, &GpuMap->Buffer, World, Graphics, g_VisibleRegion, &GameState->Heap);
    BufferEntities( GameState->EntityTable, &GpuMap->Buffer, Graphics, World, Plat->dt);
  END_BLOCK("BufferMeshes");

#if BONSAI_DEBUG_SYSTEM_API
  for (u32 ChunkIndex = 0;
      ChunkIndex < GetDebugState()->PickedChunkCount;
      ++ChunkIndex)
  {
    world_chunk *Chunk = GetDebugState()->PickedChunks[ChunkIndex];
    untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
    u8 Color = GREEN;

    if (Chunk == GetDebugState()->HotChunk)
    {
      Color = PINK;
    }

    DEBUG_DrawChunkAABB(&CopyDest, Graphics, Chunk, World->ChunkDim, Color, 0.35f);
  }
#endif

  Graphics->Lights->Count = 0;

  return;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Info("Initializing Game");

  game_state *GameState = Allocate(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;
  GameState->Plat = Plat;

  Init_Global_QuadVertexBuffer();

  GameState->Noise = perlin_noise(DEBUG_NOISE_SEED);

  GameState->Plat->Graphics = GraphicsInit(GameMemory);
  if (!GameState->Plat->Graphics) { Error("Initializing Graphics"); return False; }

  StandardCamera(GameState->Plat->Graphics->Camera, 10000.0f, 300.0f);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  world_position WorldCenter = World_Position(0, 0, 0);

  GameState->Heap = InitHeap(Gigabytes(4));
  GameState->World = AllocateAndInitWorld(WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  GameState->EntityTable = AllocateEntityTable(GameMemory, TOTAL_ENTITY_COUNT);

  GameState->Models = AllocateGameModels(GameState, GameMemory);

  GameState->Player = GetFreeEntity(GameState->EntityTable);
  SpawnPlayer(GameState->Models, GameState->Player, Canonical_Position(Voxel_Position(0), WorldCenter), &GameState->Entropy);

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Thread->MeshFreelist = &GameState->MeshFreelist;
  Thread->Noise = &GameState->Noise;
}
