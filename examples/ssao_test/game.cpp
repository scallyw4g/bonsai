
#include <bonsai_types.h>
#include <bonsai.h>

debug_global os *Global_Os = 0;
debug_global platform *Global_Plat = 0;

#include <globals.h>

global_variable r32 GlobalLightTheta = 0;

#include <game.h>
#include <game_constants.h>

#include <bonsai.cpp>

#include <physics.cpp>
#include <entity.cpp>

void
OrbitCameraAroundTarget(camera *Camera)
{
  Camera->P.Offset.x += 0.5f*Sin(GlobalCameraTheta);
  Camera->P.Offset.y += 0.5f*Cos(GlobalCameraTheta);

  return;
}

void
DoGameplay(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();

  world *World = GameState->World;

  chunk_dimension WorldChunkDim = World->ChunkDim;

  g_buffer_render_group *gBuffer = Plat->Graphics->gBuffer;
  ao_render_group *AoGroup       = Plat->Graphics->AoGroup;
  shadow_render_group *SG        = Plat->Graphics->SG;
  camera *Camera                 = Plat->Graphics->Camera;


#if DEBUG_DRAW_WORLD_AXIES
  DEBUG_DrawLine(&World->Mesh, gBuffer, SG, Camera, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
  DEBUG_DrawLine(&World->Mesh, gBuffer, SG, Camera, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
  DEBUG_DrawLine(&World->Mesh, gBuffer, SG, Camera, V3(0,0,0), V3(0, 0, 10000), TEAL, 0.5f );
#endif



  SimulatePlayer(GameState, GameState->Player, Hotkeys, Plat->dt);
  //SimulateEntities(GameState, GameState->Player, Hotkeys, Plat->dt);

  UpdateCameraP(Plat, World, GameState->Player->P, Camera);
  GlobalCameraTheta += Plat->dt*0.5;

  //
  // Draw World
  //

  //
  // Update gBuffer camera transform
  //

  gBuffer->ViewProjection =
    GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    GetViewMatrix(WorldChunkDim, Camera);

  TIMED_BLOCK("BufferMeshes");
    BufferWorld(World, Plat->Graphics, Camera);
    BufferEntities( GameState->EntityTable, &World->Mesh, Plat->Graphics, Camera, World);
  END_BLOCK("BufferMeshes");

  TIMED_BLOCK("RenderToScreen");
    { // Debug Lighting
      GlobalLightTheta += (Plat->dt * TWOPI) / 20;
      SG->GameLights.Lights[0].Position += 0.1*V3( Sin(GlobalLightTheta), Cos(GlobalLightTheta), 0.0f);
      SG->GameLights.Lights[1].Position += 0.2*V3( Sin(GlobalLightTheta), Cos(GlobalLightTheta), 0.0f);
      DrawVoxel( &World->Mesh, gBuffer, SG, Camera, SG->GameLights.Lights[0].Position, BLUE, V3(1.0f));
      DrawVoxel( &World->Mesh, gBuffer, SG, Camera, SG->GameLights.Lights[1].Position, RED, V3(1.0f));
    }

    RenderGBuffer(&World->Mesh, gBuffer, SG, Camera);

    RenderAoTexture(AoGroup);

    DrawGBufferToFullscreenQuad( Plat, gBuffer, SG, Camera, World->ChunkDim);

  #if DEBUG_DRAW_SHADOW_MAP_TEXTURE
    // DrawTexturedQuad(&GetDebugState()->TextRenderGroup->DebugTextureShader);
    /* DrawTexturedQuad(&GetDebugState()->TextRenderGroup->DebugFontTextureShader); */
    /* DrawTexturedQuad(&SG->DebugTextureShader); */
    /* DrawTexturedQuad(&gBuffer->DebugPositionTextureShader); */
    /* DrawTexturedQuad(&gBuffer->DebugNormalTextureShader); */
    /* DrawTexturedQuad(&gBuffer->DebugColorTextureShader); */
    /* DrawTexturedQuad(&AoGroup->DebugSsaoShader); */
    SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight));
  #endif

  END_BLOCK("RenderToScreen");

  return;
}

void
InitializeVoxels(world_chunk *Chunk)
{
  chunk_dimension Dim = Global_WorldChunkDim;
  ZeroChunk(Chunk->Data, Volume(Global_WorldChunkDim));

  if ( Chunk->WorldP.z == 0 )
  {
    InitChunkPerlin(Chunk, V3(Dim), GRASS_GREEN);

    for ( int z = 0; z < Dim.z; ++ z)
    {
      for ( int y = 0; y < Dim.y; ++ y)
      {
        for ( int x = 0; x < Dim.x; ++ x)
        {
          if (z==0)
          {
            s32 i = GetIndex(Voxel_Position(x,y,z), Chunk->Data, Dim);
            voxel *Vox = &Chunk->Data->Voxels[i];
            SetFlag(Vox, Voxel_Filled);
            Vox->Color = GRASS_GREEN;
          }
        }
      }
    }

  }

  UnSetFlag(Chunk, Chunk_Queued);
  SetFlag(Chunk, Chunk_Initialized);

  return;
}

EXPORT void
GameThreadCallback(work_queue_entry *Entry)
{
  switch (Entry->Flags)
  {
    case WorkEntry_InitWorldChunk:
    {
      InitializeVoxels( (world_chunk*)Entry->Input);
    } break;

    InvalidDefaultCase;
  }

  return;
}

EXPORT void
InitGlobals(platform *Plat, os *Os)
{
  GL_Global = &Plat->GL;
  Global_WorldChunkDim = WORLD_CHUNK_DIM;
#if BONSAI_INTERNAL
  GlobalDebugState = &Plat->DebugState;
#endif

  Global_Os = Os;
  Global_Plat = Plat;
}

EXPORT void*
GameInit( platform *Plat, memory_arena *GameMemory, os *Os)
{
  Info("Initializing Game");

  InitGlobals(Plat, Os);

  Init_Global_QuadVertexBuffer();

  srand(DEBUG_NOISE_SEED);
  PerlinNoise Noise(rand());
  GlobalNoise = Noise;

  game_state *GameState = PUSH_STRUCT_CHECKED(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;

  GameState->Turb = PUSH_STRUCT_CHECKED(noise_3d, GameState->Memory, 1);
  AllocateAndInitNoise3d(GameState, GameState->Turb, Chunk_Dimension(8,8,8) );

  GameState->Plat = Plat;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  GameState->World = AllocateAndInitWorld(GameState, World_Position(0), VISIBLE_REGION_RADIUS, WORLD_CHUNK_DIM, VISIBLE_REGION);

  AllocateEntityTable(Plat, GameState);

  GameState->Models = AllocateGameModels(GameState, GameState->Memory);
  GameState->Player = GetFreeEntity(GameState);

  SpawnPlayer(GameState, GameState->Player, Canonical_Position( V3(0,8,2), World_Position(0,0,0) ));

  return GameState;
}

EXPORT void
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();
  Assert(GlobalDebugState);
  game_mode *Mode = &GameState->Mode;
  Mode->TimeRunning += Plat->dt;

  ClearFramebuffers(Plat->Graphics);
  DoGameplay(Plat, GameState, Hotkeys);
}
