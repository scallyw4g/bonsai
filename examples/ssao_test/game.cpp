
#include <bonsai_types.h>
#include <bonsai.h>

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


  /* OrbitCameraAroundTarget(Camera); */
  UpdateCameraP(Plat, World, Canonical_Position(0), Camera);
  GlobalCameraTheta += Plat->dt*0.5;

  gBuffer->ViewProjection =
    GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    GetViewMatrix(WorldChunkDim, Camera);


#if DEBUG
  debug_state *DebugState = GetDebugState();
  debug_text_render_group *DebugRG = DebugState->TextRenderGroup;
  TextOutAt(Plat, DebugRG, &DebugRG->TextGeo, "HI", V2(100,100), 12);
#endif

  //
  // Draw World

  TIMED_BLOCK("Render");

  RenderWorld(World, Plat->Graphics, Camera);
  RenderEntities( GameState->EntityTable, &World->Mesh, Plat->Graphics, Camera, World);

  { // Debug Lighting
    GlobalLightTheta += (Plat->dt * TWOPI) / 20;
    SG->GameLights.Lights[0].Position += 0.1*V3( Sin(GlobalLightTheta), Cos(GlobalLightTheta), 0.0f);
    SG->GameLights.Lights[1].Position += 0.2*V3( Sin(GlobalLightTheta), Cos(GlobalLightTheta), 0.0f);
    DEBUG_DrawPointMarker( &World->Mesh, gBuffer, SG, Camera, SG->GameLights.Lights[0].Position, BLUE, V3(1.0f));
    DEBUG_DrawPointMarker( &World->Mesh, gBuffer, SG, Camera, SG->GameLights.Lights[1].Position, RED, V3(1.0f));
  }

  RenderGBuffer(&World->Mesh, gBuffer, SG, Camera);

  RenderAoTexture(AoGroup);

  DrawGBufferToFullscreenQuad( Plat, gBuffer, SG, Camera, World->ChunkDim);

#if DEBUG_DRAW_SHADOW_MAP_TEXTURE
  // DrawTexturedQuad(&GetDebugState()->TextRenderGroup->DebugTextureShader);
  /* DrawTexturedQuad(&SG->DebugTextureShader); */
  /* DrawTexturedQuad(&gBuffer->DebugPositionTextureShader); */
  /* DrawTexturedQuad(&gBuffer->DebugNormalTextureShader); */
  /* DrawTexturedQuad(&gBuffer->DebugColorTextureShader); */
  /* DrawTexturedQuad(&AoGroup->DebugSsaoShader); */
  SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight));
#endif

  END_BLOCK("Render");

  return;
}

void
InitializeVoxels( game_state *GameState, world_chunk *Chunk )
{
  ZeroChunk(Chunk->Data, Volume(GameState->World->ChunkDim));

  if ( Chunk->WorldP.z == 0 )
  {
    chunk_dimension Dim = GameState->World->ChunkDim;
    InitChunkPerlin(GameState, Chunk, V3(Dim), GRASS_GREEN);

    for ( int z = 0; z < Dim.z; ++ z)
    {
      for ( int y = 0; y < Dim.y; ++ y)
      {
        for ( int x = 0; x < Dim.x; ++ x)
        {
          if (z==0 || (x==0 && y==0))
          {
            s32 i = GetIndex(Voxel_Position(x,y,z), Chunk->Data, Dim);
            voxel *Vox = &Chunk->Data->Voxels[i];
            SetFlag(Vox, Voxel_Filled);
            if (z==0)
            {
              Vox->Color = GRASS_GREEN;
            }
            else
            {
              Vox->Color = WHITE;
            }
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
      InitializeVoxels(Entry->GameState, (world_chunk*)Entry->Input);
    } break;

    InvalidDefaultCase;
  }

  return;
}

EXPORT void
InitGlobals(platform *Plat)
{
  GL_Global = &Plat->GL;
  INIT_DEUBG_STATE(Plat);
  Global_WorldChunkDim = WORLD_CHUNK_DIM;
}

EXPORT void*
GameInit( platform *Plat, memory_arena *GameMemory)
{
  Info("Initializing Game");

  InitGlobals(Plat);

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

  canonical_position PlayerInitialP = {};
  AllocateAndInitWorld(GameState, PlayerInitialP.WorldP, VISIBLE_REGION_RADIUS, WORLD_CHUNK_DIM, VISIBLE_REGION);

  GameState->Models = 0;
  GameState->EventQueue.Queue = 0;
  GameState->FolieTable = 0;

  AllocateEntityTable(Plat, GameState);

  GameState->Models =
  AllocateGameModels(GameState, GameState->Memory);

  GameState->Player =
    GetFreeEntity(GameState);

  SpawnPlayer(GameState, GameState->Player );

  return GameState;
}

EXPORT void
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  game_mode *Mode = &GameState->Mode;
  Mode->TimeRunning += Plat->dt;

  ClearFramebuffers(Plat->Graphics);
  DoGameplay(Plat, GameState, Hotkeys);

  return;
}
