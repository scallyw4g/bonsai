#include <bonsai_engine.h>


void
SimulatePlayers(game_state *GameState, entity* LocalPlayer, hotkeys *Hotkeys, r32 dt)
{
  for (u32 PlayerIndex = 0;
      PlayerIndex < MAX_CLIENTS;
      ++PlayerIndex)
  {
    entity *Entity = GameState->Player;

    if (LocalPlayer == Entity)
    {
      SimulatePlayer(GameState, Entity, Hotkeys, dt );
    }
    else
    {
      SimulatePlayer(GameState, Entity, 0, dt );
    }

  }
}

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  model *Result = Allocate(model, GameState->Memory, ModelIndex_Count);

  Result[ModelIndex_Enemy] = LoadVoxModel(Memory, &GameState->Heap, ENEMY_MODEL);
  Result[ModelIndex_Player] = LoadCollada(Memory, &GameState->Heap, "models/two-axis-animated-cube.dae");
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, PLAYER_MODEL); */
  Result[ModelIndex_Loot] = LoadVoxModel(Memory, &GameState->Heap, LOOT_MODEL);

  /* chunk_dimension ProjectileDim = Chunk_Dimension(1,30,1); */
  /* Result[ModelIndex_Projectile].Chunk = AllocateChunk(Memory, &GameState->Heap, ProjectileDim); */
  /* Result[ModelIndex_Projectile].Dim = ProjectileDim; */
  /* FillChunk(Result[ModelIndex_Projectile].Chunk, ProjectileDim, GREEN); */

  Result[ModelIndex_Proton] = LoadVoxModel(Memory, &GameState->Heap, PROJECTILE_MODEL);

  return Result;
}

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  Thread->MeshFreelist = &Entry->GameState->MeshFreelist;
  Thread->Noise = &Entry->GameState->Noise;

  switch (Entry->Type)
  {
    case WorkEntryType_InitWorldChunk:
    {
      world_chunk* Dest = (world_chunk*)Entry->Input;
      if (!ChunkIsGarbage(Dest))
      {
        InitializeWorldChunkPerlinPlane(Thread, Dest, 10, -10);
      }
    } break;

    case WorkEntryType_CopyBuffer:
    {
      untextured_3d_geometry_buffer* Src = Entry->GpuCopyParams.Src;
      untextured_3d_geometry_buffer* Dest = &Entry->GpuCopyParams.Dest;
      Assert(Src->At <= Dest->End);

      v3 Basis = Entry->GpuCopyParams.Basis;
      BufferVertsChecked(Src, Dest, Basis, V3(1.0f));
    } break;

    InvalidDefaultCase;
  }


  return;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();

  world                     * World         = GameState->World;
  graphics                  * Graphics      = GameState->Graphics;
  g_buffer_render_group     * gBuffer       = Graphics->gBuffer;
  ao_render_group           * AoGroup       = Graphics->AoGroup;
  camera                    * Camera        = Graphics->Camera;

  gpu_mapped_element_buffer * GpuMap        = Graphics->GpuBuffers + Graphics->GpuBufferWriteIndex;
  Graphics->GpuBufferWriteIndex             = (Graphics->GpuBufferWriteIndex + 1) % 2;

  chunk_dimension             WorldChunkDim = World->ChunkDim;

  MapGpuElementBuffer(GpuMap);

  entity *Player = GameState->Player;
  ClearFramebuffers(Graphics);

#if DEBUG_DRAW_WORLD_AXIES
  /* DEBUG_DrawLine(&World->Mesh, Graphics, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f ); */
  /* DEBUG_DrawLine(&World->Mesh, Graphics, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f ); */
  /* DEBUG_DrawLine(&World->Mesh, Graphics, V3(0,0,0), V3(0, 0, 10000), BLUE, 0.5f ); */
#endif

  if (Hotkeys->Player_Spawn)
  {
    Unspawn(Player);
    world_position PlayerChunkP = World_Position(0, 0, -2);
    SpawnPlayer(GameState, Player, Canonical_Position(Voxel_Position(0,0,0), PlayerChunkP) );
    World->Center = PlayerChunkP;
  }

  SimulatePlayers(GameState, Player, Hotkeys, Plat->dt);

  UpdateGameCamera(Plat->MouseDP, &Plat->Input, Player->P, Camera);

  SimulateEntities(GameState, Plat->dt);

  SimulateAndRenderParticleSystems(GameState->EntityTable, &GpuMap->Buffer, Graphics, Plat->dt);

  gBuffer->ViewProjection =
    ProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    ViewMatrix(WorldChunkDim, Camera);

  TIMED_BLOCK("BufferMeshes");
    BufferWorld(GameState, &GpuMap->Buffer, World, Graphics, VISIBLE_REGION_RADIUS);
    BufferEntities( GameState->EntityTable, &GpuMap->Buffer, Graphics, World, Plat->dt);
  END_BLOCK("BufferMeshes");

  TIMED_BLOCK("RenderToScreen");
    RenderGBuffer(GpuMap, Graphics);
    RenderAoTexture(AoGroup);
    DrawGBufferToFullscreenQuad(Plat, Graphics);
  END_BLOCK("RenderToScreen");

  Graphics->Lights->Count = 0;

  return;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Info("Initializing Game");

  GL = *GL_in;
  GetDebugState = GetDebugState_in;

  Init_Global_QuadVertexBuffer();

  game_state *GameState = Allocate(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;
  GameState->Noise = perlin_noise(DEBUG_NOISE_SEED);

  GameState->Graphics = GraphicsInit(GameMemory);
  if (!GameState->Graphics) { Error("Initializing Graphics"); return False; }

  StandardCamera(GameState->Graphics->Camera, 1000.0f, 300.0f);

  GameState->Plat = Plat;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  world_position WorldCenter = World_Position(0, 0, 0);

  GameState->World = AllocateAndInitWorld(GameState, WorldCenter, WORLD_CHUNK_DIM, VISIBLE_REGION);

  AllocateEntityTable(GameState);

  GameState->Heap = InitHeap(Gigabytes(1));
  GameState->Models = AllocateGameModels(GameState, GameState->Memory);

  GameState->Player = GetFreeEntity(GameState);
  SpawnPlayer(GameState, GameState->Player, Canonical_Position(Voxel_Position(0), WorldCenter));

  return GameState;
}
