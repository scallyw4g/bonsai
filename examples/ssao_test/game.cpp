
#include <bonsai.h>

//
// TODO(Jesse): Axe these!
static gl_extensions *GL_Global;

GLOBAL_VARIABLE physics NullPhysics = {};
GLOBAL_VARIABLE hotkeys NullHotkeys = {};

GLOBAL_VARIABLE r32 GlobalLightTheta = 0;

#include <game.h>
#include <game_constants.h>

#include <bonsai.cpp>

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
  camera *Camera = GameState->Camera;

  chunk_dimension WorldChunkDim = World->ChunkDim;

  g_buffer_render_group *gBuffer = GameState->gBuffer;
  ao_render_group *AoGroup = GameState->AoGroup;
  shadow_render_group *SG     = GameState->SG;


#if DEBUG_DRAW_WORLD_AXIES
  DEBUG_DrawLine(World, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
  DEBUG_DrawLine(World, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
  DEBUG_DrawLine(World, V3(0,0,0), V3(0, 0, 10000), TEAL, 0.5f );
#endif


  OrbitCameraAroundTarget(Camera);
  UpdateCameraP(Plat, World, Canonical_Position(0), Camera);
  GlobalCameraTheta += Plat->dt*0.5;

  gBuffer->ViewProjection =
    GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    GetViewMatrix(WorldChunkDim, Camera);

  //
  // Draw World

  TIMED_BLOCK("Render");

  // FIXME(Jesse): This is extremely slow on my laptop ..?!
  /* ClearFramebuffers(gBuffer, SG); */

  world_position VisibleRadius = World_Position(VR_X, VR_Y, VR_Z)/2;

  world_position Min = World->Center - VisibleRadius;
  world_position Max = World->Center + VisibleRadius + 1;

  TIMED_BLOCK("Render - World");
  for ( s32 ChunkIndex = 0;
        ChunkIndex < WORLD_HASH_SIZE;
        ++ChunkIndex)
  {
    world_chunk *chunk = World->ChunkHash[ChunkIndex];

    while (chunk)
    {
      if ( (chunk->WorldP >= Min && chunk->WorldP < Max) )
      {
        /* DEBUG_DrawChunkAABB( World, chunk, Camera, Quaternion(), BLUE ); */
        BufferWorldChunk(World, chunk, Camera, gBuffer, SG);
        chunk = chunk->Next;
      }
      else
      {
        world_chunk *ChunkToFree = chunk;
        chunk = chunk->Next;
        FreeWorldChunk(World, ChunkToFree);
      }
    }
  }
  END_BLOCK("Render - World");


  GlobalLightTheta += (Plat->dt * TWOPI) / 20;
  SG->GameLights.Lights[0].Position += 0.1*V3( Sin(GlobalLightTheta), Cos(GlobalLightTheta), 0.0f);
  DEBUG_DrawPointMarker( &World->Mesh, gBuffer, SG, Camera, SG->GameLights.Lights[0].Position, BLUE, V3(1.0f));


  /* DoLight(SG->Lights, SG->TestLight.Position); */


  RenderGBuffer(&World->Mesh, gBuffer, SG, Camera);
  AssertNoGlErrors;

  RenderAoTexture(AoGroup);
  AssertNoGlErrors;

  DrawGBufferToFullscreenQuad( Plat, gBuffer, SG, Camera, World->ChunkDim);
  AssertNoGlErrors;

  World->Mesh.VertsFilled = 0;

#if DEBUG_DRAW_SHADOW_MAP_TEXTURE
  /* DrawTexturedQuad(&SG->DebugTextureShader); */
  /* DrawTexturedQuad(&gBuffer->DebugPositionTextureShader); */
  /* DrawTexturedQuad(&gBuffer->DebugNormalTextureShader); */
  /* DrawTexturedQuad(&gBuffer->DebugColorTextureShader); */
  /* DrawTexturedQuad(&AoGroup->DebugSsaoShader); */
  /* SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight)); */

  AssertNoGlErrors;
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
    InitChunkPerlin(GameState, Chunk, V3(Dim));

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
              Vox->Color = LIGHT_GREEN;
            }
            else
            {
              Vox->Color = GREY;
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
  InitDebugState(GetDebugState(), Plat);
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


  memory_arena *GraphicsMemory = PUSH_STRUCT_CHECKED(memory_arena, GameMemory, 1);
  SubArena(GameMemory, GraphicsMemory, Megabytes(8));


  //FIXME(Jesse): Sub-arena for GraphicsMemory
  shadow_render_group *SG = PUSH_STRUCT_CHECKED(shadow_render_group, GameState->Memory, 1);
  if (!InitializeShadowBuffer(SG, GameState->Memory))
  {
    Error("Initializing Shadow Buffer"); return False;
  }

  AssertNoGlErrors;

  //FIXME(Jesse): Sub-arena for GraphicsMemory
  g_buffer_render_group *gBuffer = CreateGbuffer(GameState->Memory);
  if (!InitGbufferRenderGroup(gBuffer, GameState->Memory))
  {
    Error("Initializing g_buffer_render_group"); return False;
  }

  AssertNoGlErrors;
  ao_render_group *AoGroup = CreateAoRenderGroup(GameState->Memory);
  if (!InitAoRenderGroup(AoGroup, GameState->Memory, gBuffer->Textures, &gBuffer->ViewProjection))
  {
    Error("Initializing g_buffer_render_group"); return False;
  }




  texture *SsaoNoiseTexture = 0;
  {
    v2i SsaoNoiseDim = V2i(4,4);
    random_series SsaoEntropy;

    AoGroup->NoiseTile = V3(SCR_WIDTH/SsaoNoiseDim.x, SCR_HEIGHT/SsaoNoiseDim.y, 1);

    InitSsaoKernel(AoGroup->SsaoKernel, ArrayCount(AoGroup->SsaoKernel), &SsaoEntropy);

    // TODO(Jesse): Transient arena for this instead of stack allocation ?
    v3 SsaoNoise[Area(SsaoNoiseDim)] = {};
    InitSsaoNoise(&SsaoNoise[0], ArrayCount(SsaoNoise), &SsaoEntropy);

    SsaoNoiseTexture = MakeTexture_RGB(SsaoNoiseDim, &SsaoNoise, GraphicsMemory);
  }














  camera *Camera = PUSH_STRUCT_CHECKED(camera, GameState->Memory, 1);
  InitCamera(Camera, CAMERA_INITIAL_P, 5000.0f);

  gBuffer->LightingShader =
    MakeLightingShader(GraphicsMemory, gBuffer->Textures, SG->ShadowMap, AoGroup->Texture,
        &gBuffer->ViewProjection, &gBuffer->ShadowMVP, &SG->GameLights.Lights[0], &SG->GameLights, Camera);

  gBuffer->gBufferShader =
    CreateGbufferShader(GraphicsMemory, &gBuffer->ViewProjection);

  AoGroup->Shader = MakeSsaoShader(GraphicsMemory, gBuffer->Textures, SsaoNoiseTexture,
      &AoGroup->NoiseTile, &gBuffer->ViewProjection);

  AoGroup->SsaoKernelUniform = GetShaderUniform(&AoGroup->Shader, "SsaoKernel");

  { // To keep these here or not to keep these here..
    gBuffer->DebugColorTextureShader    = MakeSimpleTextureShader(gBuffer->Textures->Color    , GraphicsMemory);
    gBuffer->DebugNormalTextureShader   = MakeSimpleTextureShader(gBuffer->Textures->Normal   , GraphicsMemory);
    gBuffer->DebugPositionTextureShader = MakeSimpleTextureShader(gBuffer->Textures->Position , GraphicsMemory);
    AoGroup->DebugSsaoShader            = MakeSimpleTextureShader(AoGroup->Texture            , GraphicsMemory);
  }

  GameState->Turb = PUSH_STRUCT_CHECKED(noise_3d, GameState->Memory, 1);
  AllocateAndInitNoise3d(GameState, GameState->Turb, Chunk_Dimension(8,8,8) );

  // This needs to be off for shadow maps to work correctly
  /* glEnable(GL_CULL_FACE); */
  /* glCullFace(GL_BACK); */

  // This is necessary!
  GLuint VertexArrayID;
  Plat->GL.glGenVertexArrays(1, &VertexArrayID);
  Plat->GL.glBindVertexArray(VertexArrayID);

  AssertNoGlErrors;

  AssertNoGlErrors;

  GameState->Plat = Plat;
  GameState->Camera = Camera;
  GameState->gBuffer = gBuffer;
  GameState->AoGroup = AoGroup;
  GameState->SG = SG;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;


  canonical_position PlayerInitialP = {};
  AllocateAndInitWorld(GameState, PlayerInitialP.WorldP, VISIBLE_REGION_RADIUS, WORLD_CHUNK_DIM, VISIBLE_REGION);

  GameState->Models = 0;
  GameState->EventQueue.Queue = 0;
  GameState->FolieTable = 0;
  GameState->Player = 0;

  return GameState;
}

EXPORT void
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  game_mode *Mode = &GameState->Mode;
  Mode->TimeRunning += Plat->dt;

  ClearFramebuffers(GameState->gBuffer, GameState->SG);
  DoGameplay(Plat, GameState, Hotkeys);

  return;
}
