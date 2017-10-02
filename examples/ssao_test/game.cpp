

#include <bonsai.h>

//
// TODO(Jesse): Axe these!
static gl_extensions *GL_Global;

GLOBAL_VARIABLE physics NullPhysics = {};
GLOBAL_VARIABLE hotkeys NullHotkeys = {};

#include <game.h>

#include <bonsai.cpp>


void
SpawnEntity( model *GameModels, entity *Entity, entity_type Type)
{
  // These are mutually exclusive, so checking both is redundant, but that
  // could change in the future
  Assert(Unspawned(Entity));
  Assert(!Destroyed(Entity));

  Entity->State = EntityState_Spawned;
  Entity->Type = Type;
  Entity->FireCooldown = Entity->RateOfFire;

  switch (Type)
  {
    case EntityType_Player:
    {
      Entity->Model = GameModels[EntityType_Player];
    } break;

    case EntityType_Enemy:
    {
      Entity->Model = GameModels[EntityType_Enemy];
    } break;

    case EntityType_ParticleSystem:
    {
      Entity->Model = GameModels[EntityType_None];
    } break;

    InvalidDefaultCase;
  }

  return;
}

void
SpawnEntity(
    entity *Entity,
    model *Model,
    entity_type Type,

    physics *Physics,

    canonical_position *InitialP,
    v3 CollisionVolumeRadius,

    r32 Scale = 1.0f,
    r32 RateOfFire = 1.0f,
    u32 Health = 1
  )
{
  TIMED_FUNCTION();

  if (Model)
    Entity->Model = *Model;

  Entity->Type = Type;

  Entity->Physics = *Physics;

  if (InitialP)
    Entity->P = *InitialP;

  Entity->CollisionVolumeRadius = CollisionVolumeRadius;

  Entity->Rotation = Quaternion(0,0,0,1);

  Entity->Scale = Scale;
  Entity->RateOfFire = RateOfFire;
  Entity->Health = Health;

  Entity->State = EntityState_Spawned;

  return;
}

entity *
AllocateEntity(platform *Plat, memory_arena *Memory, chunk_dimension ModelDim)
{
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Memory, 1);
  Entity->Model.Chunk = AllocateChunk(Memory, ModelDim);
  Entity->Emitter = PUSH_STRUCT_CHECKED(particle_system, Memory, 1);

  FillChunk(Entity->Model.Chunk, ModelDim, BLACK);
  Entity->Model.Dim = ModelDim;

  Entity->Scale = 1.0f;

  return Entity;
}

void
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  GameState->Models[ModelIndex_Enemy] = LoadModel(Memory, ENEMY_MODEL);
  GameState->Models[ModelIndex_Player] = LoadModel(Memory, PLAYER_MODEL);
  GameState->Models[ModelIndex_Loot] = LoadModel(Memory, LOOT_MODEL);

  chunk_dimension ProjectileDim = Chunk_Dimension(1,30,1);
  GameState->Models[ModelIndex_Projectile].Chunk = AllocateChunk(Memory, ProjectileDim);
  GameState->Models[ModelIndex_Projectile].Dim = ProjectileDim;
  FillChunk(GameState->Models[ModelIndex_Projectile].Chunk, ProjectileDim, GREEN);

  GameState->Models[ModelIndex_Proton] = LoadModel(Memory, PROJECTILE_MODEL);

  return;
}

entity *
AllocateEntity(platform *Plat, memory_arena *Memory)
{
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Memory, 1);
  Entity->Emitter = PUSH_STRUCT_CHECKED(particle_system, Memory, 1);

  Entity->Scale = 1.0f;

  return Entity;
}

void
InitCamera(camera* Camera, canonical_position P, float FocalLength)
{
  Camera->Frust.farClip = FocalLength;
  Camera->Frust.nearClip = 0.1f;
  Camera->Frust.width = 30.0f;
  Camera->Frust.FOV = 45.0f;
  Camera->P = P;
  Camera->Up = WORLD_Y;
  Camera->Right = WORLD_X;
  Camera->Front = WORLD_Z;

  return;
}

entity *
GetFreeEntity(game_state *GameState)
{
  entity *Result = 0;

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *TestEntity = GameState->EntityTable[EntityIndex];
    if (Unspawned(TestEntity) && !Destroyed(TestEntity))
    {
      Result = TestEntity;
      break;
    }
  }

  Assert(Unspawned(Result));
  Assert(!Destroyed(Result));

  return Result;
}

b32
SpawnParticle(particle_system *System)
{
  b32 Spawn = ((RandomUnilateral(&System->Entropy)) * System->EmissionChance) >= 0.5f;

  if (Spawn)
  {
    particle *Particle = &System->Particles[System->ActiveParticles++];
    Assert(System->ActiveParticles < PARTICLES_PER_SYSTEM);

    r32 X = RandomBilateral(&System->Entropy);
    r32 Y = RandomBilateral(&System->Entropy);
    r32 Z = RandomBilateral(&System->Entropy);

    v3 Random = V3(X,Y,Z);

    Particle->Offset = (Random*System->SpawnRegion.Radius) + System->SpawnRegion.Center;

    Particle->Physics = System->ParticlePhysics;
    Particle->Physics.Force = Normalize(Random);

    Particle->RemainingLifespan = System->ParticleLifespan;
  }

  return Spawn;
}

void
SpawnParticleSystem(particle_system *System, particle_system_init_params *Params)
{
  Assert(Inactive(System));

  System->SpawnRegion = Params->SpawnRegion;
  System->ParticlePhysics = Params->Physics;

  System->EmissionLifespan = Params->EmissionLifespan;
  System->EmissionChance = Params->EmissionChance;

  System->ParticleLifespan = Params->ParticleLifespan;

  System->Entropy = Params->Entropy;

  MemCopy( (u8*)&Params->Colors, (u8*)&System->Colors, sizeof(System->Colors) );

  return;
}

void
SpawnExplosion(entity *Entity, random_series *Entropy, v3 Offset)
{
  particle_system_init_params Params = {};

  Params.Entropy.Seed = RandomU32(Entropy);

  Params.Colors[0] = BLACK;
  Params.Colors[1] = DARK_DARK_RED;
  Params.Colors[2] = DARK_RED;
  Params.Colors[3] = ORANGE;
  Params.Colors[4] = YELLOW;
  Params.Colors[5] = WHITE;

  Params.SpawnRegion = aabb(Offset, V3(1.2f));

  Params.EmissionLifespan = 0.10f;
  Params.ParticleLifespan = 0.55f;
  Params.EmissionChance = 8.0f;

  Params.Physics.Speed = 18;
  Params.Physics.Drag = 2.0f;
  Params.Physics.Mass = 0.3f;

  SpawnParticleSystem(Entity->Emitter, &Params );

  return;
}

inline b32
ShouldEmit(particle_system *System)
{
  b32 Result = (System->EmissionLifespan >= 0);
  return Result;
}

void
SimulateAndRenderParticleSystems(
    game_state *GameState,
    mesh_buffer_target *Mesh,
    entity *SystemEntity,
    r32 dt
  )
{
  world *World = GameState->World;
  chunk_dimension WorldChunkDim = World->ChunkDim;
  camera *Camera = GameState->Camera;
  particle_system *System = SystemEntity->Emitter;
  // noise_3d *Turb = GameState->Turb;

  if (Inactive(System))
    return;

  System->EmissionLifespan -= dt;

  if ( CurrentFrameIsLogicalFrame(&GameState->Frame) && ShouldEmit(System) )
  {
    while (SpawnParticle(System));
  }

  for ( s32 ParticleIndex = 0;
        ParticleIndex < System->ActiveParticles;
        ++ParticleIndex)
  {
    particle *Particle = &System->Particles[ParticleIndex];

    r32 MinDiameter = 0.3f;
    r32 LastDiameter = (Particle->RemainingLifespan / System->ParticleLifespan) + MinDiameter;

    Particle->RemainingLifespan -= dt;

    if ( Particle->RemainingLifespan < 0)
    {
      // Swap out last partcile for the current partcile and decrement
      particle *SwapParticle = &System->Particles[System->ActiveParticles--];
      *Particle = *SwapParticle;
      continue;
    }

    r32 Diameter = (Particle->RemainingLifespan / System->ParticleLifespan) + MinDiameter;
    r32 DiameterDiff = LastDiameter-Diameter;

    Particle->Offset += DiameterDiff;

    u8 ColorIndex = (u8)((Particle->RemainingLifespan / System->ParticleLifespan) * (PARTICLE_SYSTEM_COLOR_COUNT-0.0001f));
    Assert(ColorIndex >= 0 && ColorIndex <= PARTICLE_SYSTEM_COLOR_COUNT);

    DrawParticle(&SystemEntity->P, Mesh, WorldChunkDim, Camera, Particle, Diameter, System->Colors[ColorIndex]);


  }

  return;
}


void
SimulateEntities(game_state *GameState, entity *Player, hotkeys *Hotkeys, r32 dt)
{
  TIMED_FUNCTION();

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *Entity = GameState->EntityTable[EntityIndex];

    if (!Spawned(Entity))
        continue;

    switch (Entity->Type)
    {
      case EntityType_ParticleSystem:
      {
      } break;

      InvalidDefaultCase;
    }

    SimulateAndRenderParticleSystems(GameState, &GameState->World->Mesh, Entity, dt);
  }

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
      PUSH_STRUCT_CHECKED(particle_system, GameState->World->WorldStorage.Arena, 1);
  }

  return;
}
#endif

void
AllocateEntityTable(platform *Plat, game_state *GameState)
{
  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++ EntityIndex)
  {
    GameState->EntityTable[EntityIndex] =
      AllocateEntity(Plat, GameState->Memory);
  }

  return;
}

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
AllocateAndInitNoise3d(game_state *GameState, noise_3d *Noise, chunk_dimension Dim)
{
  Noise->Dim = Dim;
  Noise->Voxels = PUSH_STRUCT_CHECKED(voxel, GameState->Memory, Volume(Dim));
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
  ShadowRenderGroup *SG     = GameState->SG;

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

  UpdateCameraP(Plat, World, Player, Camera);

  GlobalLightTheta += Plat->dt;

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
        BufferWorldChunk(World, chunk, Camera, gBuffer);
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
    BufferEntity( &World->Mesh, Enemy, Camera, gBuffer, World->ChunkDim);
  }
  END_BLOCK("Entities");

  for ( s32 FolieIndex = 0;
      FolieIndex < TOTAL_FOLIE_COUNT;
      ++FolieIndex)
  {
    aabb *AABB = GameState->FolieTable + FolieIndex;
    DrawFolie(&World->Mesh, Camera, AABB);
  }

  RenderGBuffer(&World->Mesh, gBuffer, SG, Camera);
  AssertNoGlErrors;

  RenderAoTexture(AoGroup);
  AssertNoGlErrors;

  DrawGBufferToFullscreenQuad( Plat, gBuffer, SG, Camera, World->ChunkDim);
  AssertNoGlErrors;

  World->Mesh.VertexCount = 0;
  World->Mesh.filled = 0;


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
  ShadowRenderGroup *SG = PUSH_STRUCT_CHECKED(ShadowRenderGroup, GameState->Memory, 1);
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
















  gBuffer->LightingShader =
    MakeLightingShader(GraphicsMemory, gBuffer->Textures, SG->ShadowMap, AoGroup->Texture,
        &gBuffer->ViewProjection, &gBuffer->ShadowMVP);

  gBuffer->gBufferShader =
    CreateGbufferShader(GraphicsMemory, &gBuffer->ViewProjection);

  AoGroup->Shader = MakeSsaoShader(GraphicsMemory, gBuffer->Textures, SsaoNoiseTexture,
      &AoGroup->NoiseTile, &gBuffer->ViewProjection);

  AoGroup->SsaoKernelUniform = GetShaderUniform(&AoGroup->Shader, "SsaoKernel");

  { // To keep these here or not to keep these here..
    gBuffer->DebugColorTextureShader = MakeSimpleTextureShader(gBuffer->Textures->Color, GraphicsMemory);
    gBuffer->DebugNormalTextureShader = MakeSimpleTextureShader(gBuffer->Textures->Normal, GraphicsMemory);
    gBuffer->DebugPositionTextureShader = MakeSimpleTextureShader(gBuffer->Textures->Position, GraphicsMemory);
    AoGroup->DebugSsaoShader = MakeSimpleTextureShader(AoGroup->Texture, GraphicsMemory);
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

  camera *Camera = PUSH_STRUCT_CHECKED(camera, GameState->Memory, 1);
  InitCamera(Camera, CAMERA_INITIAL_P, 5000.0f);

  AssertNoGlErrors;

  GameState->Plat = Plat;
  GameState->Camera = Camera;
  GameState->gBuffer = gBuffer;
  GameState->AoGroup = AoGroup;
  GameState->SG = SG;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;


  canonical_position PlayerInitialP = {};

  AllocateAndInitWorld(GameState, PlayerInitialP.WorldP, VISIBLE_REGION_RADIUS);

  GameState->Models =
    PUSH_STRUCT_CHECKED(model, GameState->Memory, ModelIndex_Count);
  AllocateGameModels(GameState, GameState->Memory);

  AllocateEntityTable(Plat, GameState);

  GameState->EventQueue.Queue =
    PUSH_STRUCT_CHECKED(frame_event*, GameState->Memory, TOTAL_FRAME_EVENT_COUNT);

  frame_event *TempFrameEvents =
    PUSH_STRUCT_CHECKED(frame_event, GameState->Memory, TOTAL_FRAME_EVENT_COUNT);

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
    PUSH_STRUCT_CHECKED(aabb, GameState->Memory, TOTAL_FOLIE_COUNT);

  /* AllocateParticleSystems(Plat, GameState); */

  GameState->Player =
    GetFreeEntity(GameState);

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
