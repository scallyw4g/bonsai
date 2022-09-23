#define PLATFORM_GL_IMPLEMENTATIONS 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

/* global_variable chunk_dimension */
/* WORLD_CHUNK_DIM = Chunk_Dimension(32, 32, 32); */

link_internal world_chunk
DeserializeChunk(const char *zAssetPath, memory_arena *Memory)
{
  native_file AssetFile = OpenFile(zAssetPath, "r");
  world_chunk_file_header Header = ReadWorldChunkFileHeader(&AssetFile);

  world_chunk Result = {};
  AllocateWorldChunk(&Result, Memory, {}, WORLD_CHUNK_DIM);
  Result.Mesh = AllocateMesh(Memory, (u32)Header.MeshElementCount);

  u32 Tag = Read_u32(&AssetFile);
  Assert( Tag ==  WorldChunkFileTag_VOXD );
  umm VoxByteCount = Header.VoxelElementCount * Header.VoxelElementSize;
  ReadBytesIntoBuffer(&AssetFile, VoxByteCount, (u8*)Result.Data->Voxels);

  BuildEntityMesh(Result.Data, Result.Mesh, DefaultPalette, WORLD_CHUNK_DIM);

  CloseFile(&AssetFile);
  return Result;
}

link_internal model
LoadWorldChunk(memory_arena *Memory, heap_allocator *Heap, const char* zAssetPath )
{
  world_chunk Chunk = DeserializeChunk(zAssetPath, Memory);

  model Result = {};

  Result.Mesh = *Chunk.Mesh;
  Result.Mesh.At = Result.Mesh.End;

  Result.Dim = Chunk_Dimension(Chunk.DimX, Chunk.DimY, Chunk.DimZ);

  return Result;
}

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  model *Result = Allocate(model, GameState->Memory, ModelIndex_Count);
  Result[ModelIndex_Player] = LoadVoxModel(Memory, &GameState->Heap, "models/chr_old.vox");
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, &GameState->Heap, "../voxel-model/vox/monument/monu10.vox"); */
  /* Result[ModelIndex_Player] = LoadWorldChunk(Memory, &GameState->Heap, "assets/world_chunk_1_0_0"); */

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
#if 1
      volatile work_queue_entry_init_asset *Job = SafeAccess(work_queue_entry_init_asset, Entry);
      world_chunk *Chunk = Job->Chunk;

      if (ChunkIsGarbage(Chunk))
      {
        MarkChunkInitialized(Chunk);
      }
      else
      {
        // TODO(Jesse): Improve ergonomics here?
        counted_string AssetPath = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);
        const char* zAssetPath = GetNullTerminated(AssetPath, Thread->TempMemory);
        DeserializeChunk(zAssetPath, Chunk);
        MarkChunkInitialized(Chunk);
      }

#endif
    } break;

    case type_work_queue_entry_init_world_chunk:
    {
      // TODO(Jesse): Initialize from noise here?
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

#if BONSAI_DEBUG_SYSTEM_API
  if (!GetDebugState) { GetDebugState = (get_debug_state_proc)Plat->GetDebugStateProc; }
  GetDebugState()->Plat = GameState->Plat;
  GetDebugState()->GameState = GameState;
#endif

  world                 *World         = GameState->World;
  graphics              *Graphics      = Plat->Graphics;
  g_buffer_render_group *gBuffer       = Graphics->gBuffer;
  camera                *Camera        = Graphics->Camera;

  Graphics->Lights->Count = 0;

  gpu_mapped_element_buffer* GpuMap = GetCurrentGpuMap(Graphics);

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
    SpawnPlayer(GameState->Models, Player,  Canonical_Position(V3(0,0,0), World_Position(0,0,2)), &GameState->Entropy);
    World->Center = World_Position(0, 0, 2);
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

#if 0
    TIMED_BLOCK("DrainQueue");
      DrainQueue(&Plat->HighPriority, Thread, BONSAI_API_WORKER_THREAD_CALLBACK_NAME);
    END_BLOCK("DrainQueue");

    WaitForWorkerThreads(&Plat->WorkerThreadsWaiting);

    /* Renderer_FrameEnd(Plat); */
#endif

  return;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Info("Initializing Game");


  game_state *GameState = Allocate(game_state, GameMemory, 1);

  GL = *GL_in;
  GameState->Plat = Plat;
  GameState->Memory = GameMemory;



  Init_Global_QuadVertexBuffer();

  GameState->Noise = perlin_noise(DEBUG_NOISE_SEED);

  GameState->Plat->Graphics = GraphicsInit(GameMemory);

  /* GameState->Graphics = GraphicsInit(GameMemory); */
  /* if (!GameState->Graphics) { Error("Initializing Graphics"); return False; } */

  StandardCamera(Plat->Graphics->Camera, 10000.0f, 300.0f);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  world_position WorldCenter = World_Position(0, 0, 2);

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
