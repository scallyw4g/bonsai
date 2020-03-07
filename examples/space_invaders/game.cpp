#include <bonsai_engine.h>

r32
GetLevel(r64 Time)
{
  r32 Level = 1 + ((r32)(Time/SECONDS_PER_LEVEL));
  return Level;
}

void
SpawnEnemies(game_state *GameState, r32 dt)
{
  TIMED_FUNCTION();
  entity **Entities = GameState->EntityTable;

  r32 Level = GetLevel(GameState->Mode.TimeRunning);

  r32 EnemySpawnRate = Level/30.0f;

  r32 SpawnRoll = RandomUnilateral(&GameState->Entropy);

  b32 Spawn = ( SpawnRoll <= EnemySpawnRate);
  if (!Spawn)
    return;

  entity *Enemy = GetFreeEntity(GameState);

  if (Enemy)
    SpawnEnemy(GameState->World, Entities, Enemy, &GameState->Entropy, GameState->Models);

  return;
}

#if 0
void
AllocateParticleSystems(platform *Plat, game_state *GameState)
{
  for (s32 SystemIndex = 0;
      SystemIndex < TOTAL_PARTICLE_SYSTEMS;
      ++SystemIndex)
  {
    GameState->ParticleSystems[SystemIndex] =
      Allocate(particle_system, GameState->World->WorldStorage.Arena, 1);
  }

  return;
}
#endif

void
ReleaseFrameEvent(event_queue *Queue, frame_event *Event)
{
  frame_event *First = Queue->FirstFreeEvent;

  Queue->FirstFreeEvent = Event;
  Queue->FirstFreeEvent->Next = First;

  return;
}

void
SetMode(game_mode *Mode, game_mode_type Active)
{
  Mode->ActiveMode = Active;
  Mode->TimeRunning = 0;

  return;
}

void
StartGame(game_state *GameState)
{
  game_mode *Mode = &GameState->Mode;
  SetMode(Mode, GameMode_Playing);

  for (s32 EntityIndex = 0;
       EntityIndex < TOTAL_ENTITY_COUNT;
       ++EntityIndex)
  {
    entity *Entity = GameState->EntityTable[EntityIndex];

    Entity->Emitter->ActiveParticles = 0;
    Entity->Emitter->EmissionLifespan = 0;

    Unspawn(Entity);
    Deactivate(Entity->Emitter);
  }

  GameState->Player = GetFreeEntity(GameState);
  SpawnPlayer(GameState, GameState->Player );

  return;
}

void
LoseGame()
{}

void
WinGame()
{}

void
ProcessFrameEvent(game_state *GameState, frame_event *Event)
{
  switch(Event->Type)
  {
    case FrameEvent_GameModeLoss:
    {
      SetMode(&GameState->Mode, GameMode_Loss);
    } break;

    case FrameEvent_GameModePlaying:
    {
      GameState->Mode.ActiveMode = GameMode_Playing;
    } break;

    case FrameEvent_GameModeWon:
    {
      GameState->Mode.ActiveMode = GameMode_Won;
    } break;

    case FrameEvent_Explosion:
    {
      entity *Entity = Event->Entity;
      Assert( Destroyed(Entity) );

      Unspawn(Entity);

      r32 PhysicsMultiple = 0.15f;
      Entity->Physics.Velocity = Entity->Physics.Velocity * PhysicsMultiple;
      Entity->Physics.Speed = 0;
      Entity->Physics.Force = V3(0);

      v3 Offset = (Entity->Model.Dim/2.0f)*Entity->Scale;

      SpawnEntity( GameState->Models, Entity, EntityType_ParticleSystem );
      SpawnExplosion(Entity, &GameState->Entropy, Offset);
    } break;

    InvalidDefaultCase;
  }

  ReleaseFrameEvent(&GameState->EventQueue, Event);
  return;
}

void
DoGameplay(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();

  UpdateLogicalFrameCount(&GameState->Frame, Plat->dt);

  world *World = GameState->World;
  entity *Player = GameState->Player;
  camera *Camera = GameState->Camera;

  chunk_dimension WorldChunkDim = World->ChunkDim;

  g_buffer_render_group *gBuffer = GameState->gBuffer;
  ao_render_group *AoGroup = GameState->AoGroup;
  shadow_render_group *SG     = GameState->SG;

  r32 VrHalfDim = (VR_X*CD_X/2.0f);

  v3 WorldCenterRenderP = GetRenderP(World->ChunkDim, World->Center, Camera);

  aabb *Floor = GameState->FolieTable + 0;
  aabb *LeftWall = GameState->FolieTable + 1;
  aabb *RightWall = GameState->FolieTable + 2;

  r32 ZOffset = 25.0f;
  r32 WallThickness = 200.0f;
  r32 WallLength = VR_Y*CD_Y;
  r32 YOffset = WallLength/2.0f;
  {
    v3 Center = WorldCenterRenderP -
                  V3(VrHalfDim, YOffset, ZOffset);

    *Floor = aabb(Center, V3(World->ChunkDim.x+VrHalfDim, WallLength, 0.5f));
  }

  {
    v3 Center = WorldCenterRenderP -
                  V3(VrHalfDim+(2.0f*WallThickness), YOffset, ZOffset);

    *LeftWall = aabb(Center, V3(WallThickness, WallLength, ZOffset));
  }
  {
    v3 Center = WorldCenterRenderP -
                  V3(-VrHalfDim-World->ChunkDim.x, YOffset, 25.0f);

    *RightWall = aabb(Center, V3(WallThickness, WallLength, ZOffset));
  }


#if DEBUG_DRAW_WORLD_AXIES
  DEBUG_DrawLine(World, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
  DEBUG_DrawLine(World, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
  DEBUG_DrawLine(World, V3(0,0,0), V3(0, 0, 10000), TEAL, 0.5f );
#endif

  accumulatedTime += Plat->dt;
  numFrames ++;

  gBuffer->ViewProjection =
    GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    GetViewMatrix(WorldChunkDim, Camera);

  TIMED_BLOCK("Sim");

  SpawnEnemies(GameState, Plat->dt);
  SimulateEntities(GameState, Player, Hotkeys, Plat->dt);


  event_queue *Queue = &GameState->EventQueue;
  u32 LogicalFrameDiff = GameState->Frame.FrameDiff;
  while (LogicalFrameDiff-- > 0)
  {
    Queue->CurrentFrameIndex = (Queue->CurrentFrameIndex+1) % TOTAL_FRAME_EVENT_COUNT;

    frame_event *Event = Queue->Queue[Queue->CurrentFrameIndex];
    while (Event)
    {
      frame_event *NextEvent = Event->Next;
      ProcessFrameEvent(GameState, Event);
      Event = NextEvent;
    }

    GameState->EventQueue.Queue[GameState->EventQueue.CurrentFrameIndex] = 0;
  }

  Assert(Queue->CurrentFrameIndex == (GameState->Frame.LogicalFrame % TOTAL_FRAME_EVENT_COUNT));

  END_BLOCK("Sim");

  canonical_position NewTarget = Canonicalize(WorldChunkDim, Player->P.Offset, Player->P.WorldP) + (Player->Model.Dim/2.0f);
  UpdateCameraP(Plat, World, NewTarget, Camera);

  //
  // Draw World

  TIMED_BLOCK("Render");

  // FIXME(Jesse): This is extremely slow on my laptop ..?!
  /* ClearFramebuffers(gBuffer, SG); */

  world_position VisibleRadius = World_Position(VR_X, VR_Y, VR_Z)/2;

#if 1
  world_position Min = World->Center - VisibleRadius;
  world_position Max = World->Center + VisibleRadius + 1;
#else
  world_position Min = World_Position(0,0,0);;
  world_position Max = World->VisibleRegion;
#endif

#if 0
  DEBUG_DrawAABB( World,
                  GetRenderP(WORLD_CHUNK_DIM, Min, Camera),
                  GetRenderP(WORLD_CHUNK_DIM, Max, Camera),
                  Quaternion(),
                  RED );
#endif

#if 0
  // Draw chunks that aren't initialzied
  {
    for ( s32 z = Min.z; z < Max.z; ++ z )
    {
      for ( s32 y = Min.y; y < Max.y; ++ y )
      {
        for ( s32 x = Min.x; x < Max.x; ++ x )
        {
          world_position TestP = World_Position(x,y,z);
          world_chunk *Chunk = GetWorldChunk(World, TestP);

          if (!Chunk || NotSet(Chunk, Chunk_Initialized))
            DEBUG_DrawChunkAABB( World, TestP, Camera, Quaternion(), RED);

          if (Chunk && IsSet(Chunk, Chunk_Queued))
            DEBUG_DrawChunkAABB( World, TestP, Camera, Quaternion(), BLUE);
        }
      }
    }
  }
#endif

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

  TIMED_BLOCK("Render - Entities");
  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex)
  {
    entity *Enemy = GameState->EntityTable[EntityIndex];
    BufferEntity( &World->Mesh, Enemy, Camera, gBuffer, SG, World->ChunkDim);
  }
  END_BLOCK("Entities");

  for ( s32 FolieIndex = 0;
      FolieIndex < TOTAL_FOLIE_COUNT;
      ++FolieIndex)
  {
    aabb *AABB = GameState->FolieTable + FolieIndex;
    DrawFolie(&World->Mesh, gBuffer, SG, Camera, AABB);
  }

  RenderGBuffer(&World->Mesh, gBuffer, SG, Camera);
  AssertNoGlErrors;

  RenderAoTexture(AoGroup);
  AssertNoGlErrors;

  DrawGBufferToFullscreenQuad( Plat, gBuffer, SG, Camera, World->ChunkDim);
  AssertNoGlErrors;


#if DEBUG_DRAW_SHADOW_MAP_TEXTURE
  /* DrawTexturedQuad(&SG->DebugTextureShader); */
  /* DrawTexturedQuad(&gBuffer->DebugPositionTextureShader); */
  /* DrawTexturedQuad(&gBuffer->DebugNormalTextureShader); */
  /* DrawTexturedQuad(&gBuffer->DebugColorTextureShader); */
  /* DrawTexturedQuad(&AoGroup->DebugSsaoShader); */
  /* SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight)); */
#endif

  AssertNoGlErrors;

  END_BLOCK("Render");

  /* SwapBuffers(); */

  /* Info("%d Triangles drawn", tris ); */
  /* tris=0; */

  /* Info("%d Voxels Indexed", VoxelsIndexed ); */
  /* VoxelsIndexed=0; */

  /* Info("%d Boundary Voxels Indexed", BoundaryVoxelsIndexed ); */
  /* BoundaryVoxelsIndexed=0; */

  return;
}

void
DrawTitleScreen(game_state *GameState)
{
  debug_state *DebugState = GetDebugState();
  debug_text_render_group *RG = DebugState->TextRenderGroup;

  s32 FontSize = TITLE_FONT_SIZE;

  TextOutAt( GameState->Plat, RG, &RG->TextGeo, "Press `Space` to Start", V2(0, 0), FontSize);
}

BONSAI_API_GAME_THREAD_CALLBACK()
{
  switch (Entry->Flags)
  {
    case WorkEntry_InitWorldChunk:
    {
      world_chunk* DestChunk = (world_chunk*)Entry->Input;
      s32 Amplititude = 10;
      s32 StartingZDepth = -120;

      untextured_3d_geometry_buffer* DestMeshBuffer = ThreadSafeUnlinkFirst(Thread->FirstFreeMesh);
      if (!DestMeshBuffer) { DestMeshBuffer = AllocateMesh(Thread->PermMemory, CD_X*CD_Y*CD_Z*VERTS_PER_FACE*6); }

      InitializeWorldChunkPerlinPlane(Thread, &Entry->GameState->Noise,
                                      DestChunk, DestMeshBuffer,
                                      Amplititude, StartingZDepth);
    } break;

    InvalidDefaultCase;
  }

  return;
}

BONSAI_API_GAME_INIT_CALLBACK()
{
  Info("Initializing Game");

  InitGlobals(Plat);

  srand(DEBUG_NOISE_SEED);
  PerlinNoise Noise(rand());
  GlobalNoise = Noise;

  game_state *GameState = Allocate(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;

  memory_arena *GraphicsMemory = Allocate(memory_arena, GameMemory, 1);
  SubArena(GameMemory, GraphicsMemory, Megabytes(8));

  shadow_render_group *SG = Allocate(shadow_render_group, GameState->Memory, 1);
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

  texture *SsaoNoiseTexture = AllocateAndInitSsaoNoise(AoGroup, GraphicsMemory);

  camera *Camera = Allocate(camera, GameState->Memory, 1);
  InitCamera(Camera, V3(0,1,0), 5000.0f);

  gBuffer->LightingShader =
    MakeLightingShader(GraphicsMemory, gBuffer->Textures, SG->ShadowMap, AoGroup->Texture,
        &gBuffer->ViewProjection, &gBuffer->ShadowMVP, &SG->GameLights, Camera);

  gBuffer->gBufferShader =
    CreateGbufferShader(GraphicsMemory, &gBuffer->ViewProjection, Camera);

  AoGroup->Shader = MakeSsaoShader(GraphicsMemory, gBuffer->Textures, SsaoNoiseTexture,
      &AoGroup->NoiseTile, &gBuffer->ViewProjection);

  AoGroup->SsaoKernelUniform = GetShaderUniform(&AoGroup->Shader, "SsaoKernel");

  { // To keep these here or not to keep these here..
    gBuffer->DebugColorTextureShader = MakeSimpleTextureShader(gBuffer->Textures->Color, GraphicsMemory);
    gBuffer->DebugNormalTextureShader = MakeSimpleTextureShader(gBuffer->Textures->Normal, GraphicsMemory);
    gBuffer->DebugPositionTextureShader = MakeSimpleTextureShader(gBuffer->Textures->Position, GraphicsMemory);
    AoGroup->DebugSsaoShader = MakeSimpleTextureShader(AoGroup->Texture, GraphicsMemory);
  }



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

  GameState->Models =
    Allocate(model, GameState->Memory, ModelIndex_Count);
  AllocateGameModels(GameState, GameState->Memory);

  AllocateEntityTable(Plat, GameState);

  GameState->EventQueue.Queue =
    Allocate(frame_event*, GameState->Memory, TOTAL_FRAME_EVENT_COUNT);

  frame_event *TempFrameEvents =
    Allocate(frame_event, GameState->Memory, TOTAL_FRAME_EVENT_COUNT);

  frame_event **Next = &GameState->EventQueue.FirstFreeEvent;

  for ( s32 FrameEventIndex = 0;
        FrameEventIndex < TOTAL_FRAME_EVENT_COUNT;
        ++FrameEventIndex)
  {
    frame_event *FreeEvent = &TempFrameEvents[FrameEventIndex];
    *Next = FreeEvent;
    Next = &FreeEvent->Next;
  }

  GameState->FolieTable =
    Allocate(aabb, GameState->Memory, TOTAL_FOLIE_COUNT);

  /* AllocateParticleSystems(Plat, GameState); */

  GameState->Player =
    GetFreeEntity(GameState);

  SpawnPlayer(GameState, GameState->Player );

  return GameState;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  game_mode *Mode = &GameState->Mode;
  Mode->TimeRunning += Plat->dt;

  ClearFramebuffers(Plat->Graphics);

  switch (Mode->ActiveMode)
  {
    case GameMode_Playing:
    {
      DoGameplay(Plat, GameState, Hotkeys);
    } break;

    case GameMode_Won:
    {
      /* DoWinning(); */
    } break;

    case GameMode_Title:
    {
      if (Hotkeys->Player_Spawn)
        StartGame(GameState);

      Plat->dt *= 0.05f;
      *Hotkeys = NullHotkeys;
      DoGameplay(Plat, GameState, Hotkeys);

      DrawTitleScreen(GameState);
    } break;

    case GameMode_Loss:
    {
      Plat->dt *= 0.05f;
      *Hotkeys = NullHotkeys;
      DoGameplay(Plat, GameState, Hotkeys);

      if (Mode->TimeRunning >= 2.0f)
        SetMode(Mode, GameMode_Title);

    } break;

    InvalidDefaultCase;
  }

  return;
}
