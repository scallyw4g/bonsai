#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

global_variable v3
Global_EntityFireballOffset = V3(7.0f, -.75f, 4.5f);

enum game_entity_type
{
  GameEntityType_Unknown,

  GameEntityType_Enemy,
  GameEntityType_Splosion,
  GameEntityType_Bitty,
};

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  // TODO(Jesse)(leak, memory): Pass temp memory to LoadVoxModel

  model *Result                                       = Allocate(model, Memory, ModelIndex_Count);

  Result[ModelIndex_Enemy_Skeleton_Axe]              = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_AXE.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Sword]            = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_SWORD.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Lasher]           = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_LASHER.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Archer]           = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_ARCHER.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Spear]            = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_SPEAR.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_AxeArmor]         = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_AXE_ARMOR.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Hounds]           = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_HOUNDS.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Horserider]       = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_HORSERIDER.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Horsebanner]      = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_HORSE_BANNER.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Shaman]           = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_SHAMAN.vox", Memory);
  /* Result[ModelIndex_Enemy_Skeleton_Champion]         = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_CHAMPION.vox", Memory); */
  Result[ModelIndex_Enemy_Skeleton_ChampionChampion] = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_CHAMPION_CATAPHRACT.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Concubiner]       = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_CONCUBINER.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_King]             = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_KING.vox", Memory);

  Result[ModelIndex_Player_jp]      = LoadVoxModel(Memory, Heap, "models/players/chr_jp.vox", Memory);
  Result[ModelIndex_Player_bow]     = LoadVoxModel(Memory, Heap, "models/players/chr_bow.vox", Memory);
  Result[ModelIndex_Player_cat]     = LoadVoxModel(Memory, Heap, "models/players/chr_cat.vox", Memory);
  Result[ModelIndex_Player_fox]     = LoadVoxModel(Memory, Heap, "models/players/chr_fox.vox", Memory);
  Result[ModelIndex_Player_gumi]    = LoadVoxModel(Memory, Heap, "models/players/chr_gumi.vox", Memory);
  Result[ModelIndex_Player_knight]  = LoadVoxModel(Memory, Heap, "models/players/chr_knight.vox", Memory);
  Result[ModelIndex_Player_man]     = LoadVoxModel(Memory, Heap, "models/players/chr_man.vox", Memory);
  Result[ModelIndex_Player_mom]     = LoadVoxModel(Memory, Heap, "models/players/chr_mom.vox", Memory);
  Result[ModelIndex_Player_old]     = LoadVoxModel(Memory, Heap, "models/players/chr_old.vox", Memory);
  Result[ModelIndex_Player_poem]    = LoadVoxModel(Memory, Heap, "models/players/chr_poem.vox", Memory);
  Result[ModelIndex_Player_rain]    = LoadVoxModel(Memory, Heap, "models/players/chr_rain.vox", Memory);
  Result[ModelIndex_Player_sasami]  = LoadVoxModel(Memory, Heap, "models/players/chr_sasami.vox", Memory);
  Result[ModelIndex_Player_sol]     = LoadVoxModel(Memory, Heap, "models/players/chr_sol.vox", Memory);
  Result[ModelIndex_Player_sword]   = LoadVoxModel(Memory, Heap, "models/players/chr_sword.vox", Memory);
  Result[ModelIndex_Player_tale]    = LoadVoxModel(Memory, Heap, "models/players/chr_tale.vox", Memory);
  Result[ModelIndex_Player_tama]    = LoadVoxModel(Memory, Heap, "models/players/chr_tama.vox", Memory);
  Result[ModelIndex_Player_tsurugi] = LoadVoxModel(Memory, Heap, "models/players/chr_tsurugi.vox", Memory);

  Result[ModelIndex_Bitty0] = LoadVoxModel(Memory, Heap, "models/splotion_bitty_0.vox", Memory);
  Result[ModelIndex_Bitty1] = LoadVoxModel(Memory, Heap, "models/splotion_bitty_1.vox", Memory);

  return Result;
}

global_variable random_series Global_GameEntropy = {543232654};

link_internal canonical_position
MoveToStandingSpot(world *World, canonical_position P)
{
  canonical_position Result = Canonicalize(World->ChunkDim, Canonical_Position(P.Offset + V3(0,0,5), P.WorldP));
  return Result;
}

enum player_action
{
  PlayerAction_None,

  PlayerAction_Move,
  PlayerAction_Charge,
  PlayerAction_Fire,
  PlayerAction_Jump,

  PlayerAction_Count,
};
poof(generate_string_table(player_action))
#include <generated/generate_string_table_player_action.h>

link_internal void
DoSplotion( engine_resources *Resources, picked_voxel *Pick, canonical_position PickCP, f32 Radius, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Resources);

  world_update_op_shape Shape = {
    .Type = type_world_update_op_shape_params_sphere,
    .world_update_op_shape_params_sphere.Radius = Radius,
    .world_update_op_shape_params_sphere.Location = *Pick,
  };
  QueueWorldUpdateForRegion(Resources, WorldUpdateOperationMode_Subtractive, &Shape, DARK_GREY, Resources->Memory);
  /* QueueWorldUpdateForRegion(Resources, Pick, MinPCoarse, MaxPCoarse, WorldUpdateOperation_Additive, WorldUpdateOperationShape_Sphere, DARK_GREY, Radius, Resources->Memory); */

#if 1
  v3 SplosionSimP = GetSimSpaceP(World, PickCP);

  sphere Explosion = Sphere(SplosionSimP, Radius);
  u32_buffer Hits = GatherEntitiesIntersecting(World, EntityTable, &Explosion, TempMemory);

  for (u32 HitIndex = 0; HitIndex < Hits.Count; ++HitIndex)
  {
    u32 EntityIndex = Hits.Start[HitIndex];
    entity *HitEntity = EntityTable[EntityIndex];

    v3 ESimP = GetSimSpaceP(World, HitEntity->P);
    v3 EntityCenterP = GetSimSpaceCenterP(HitEntity, ESimP);

    aabb EntityAABB = GetSimSpaceAABB(World, HitEntity);
    v3 ClosestEntityPToSplosion = ClipPToAABB(&EntityAABB, Explosion.P);

    v3 SplosionToClosestEntityP = ClosestEntityPToSplosion - SplosionSimP;
    v3 SplosionToEntityCenter = EntityCenterP - SplosionSimP;

    r32 t = SafeDivide0(Length(SplosionToClosestEntityP), Radius);
    t = Clamp01(t);


    v3 MaxPower = V3(25.f, 25.f, 4.f) * Radius;
    v3 Power = Lerp(t, MaxPower, V3(0) );

    /* DebugLine("t(%f) Power(%f,%f,%f)", t, Power.x, Power.y, Power.z); */

    HitEntity->Physics.Force += Normalize(SplosionToEntityCenter) * Power;
  }
#endif

#if 1
  {
    entity *E = GetFreeEntity(EntityTable);
    SpawnEntity( E, EntityType_ParticleSystem, 0, ModelIndex_None);
    E->P = PickCP + V3(0.5f);
    E->UserData = (void*)GameEntityType_Splosion;
    SpawnExplosion(E, &Global_GameEntropy, {}, Radius);
  }
  {
    entity *E = GetFreeEntity(EntityTable);
    SpawnEntity( E, EntityType_ParticleSystem, 0, ModelIndex_None);
    E->P = PickCP + V3(0.5f);
    SpawnSmoke(E, &Global_GameEntropy, {}, Radius);
  }
#endif

#if 1
  u32 MaxBitties = 4*u32(Radius);
  for (u32 BittyIndex = 0; BittyIndex < MaxBitties; ++BittyIndex)
  {
    entity *E = GetFreeEntity(EntityTable);
    SpawnEntity( E, EntityType_ParticleSystem, GameState->Models, (model_index)(ModelIndex_Bitty0 + (BittyIndex % 2)) );
    E->Physics.Speed = 1.f;

    E->Rotation = RandomRotation(&Global_GameEntropy);
    E->Scale = 0.3f;
    E->CollisionVolumeRadius = V3(.1f);

    v3 Rnd = RandomV3Bilateral(&Global_GameEntropy);
    E->Physics.Mass = 25.f;
    E->Physics.Force += Rnd*150.f*Radius;
    E->Physics.Force.z = Abs(E->Physics.Force.z) * 0.25f;
    E->P = PickCP + (Rnd*Radius) + V3(0.f, 0.f, 2.0f);
    E->P.Offset.z = PickCP.Offset.z + 2.f;

    E->UserData = (void*)GameEntityType_Bitty;
    /* E->Update = DoBittyLight; */

    SpawnSplotionBitty(E, &Global_GameEntropy, {}, .1f);
  }
#endif
}

void
EnemyUpdate(engine_resources *Engine, entity *Enemy)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  /* if (GameState->DidPlayerAction) */
  {
    /* u32 EnemyChoice = RandomU32(&Global_GameEntropy) % 4; */
    u32 EnemyChoice = 0;

    switch(EnemyChoice)
    {
      case 0:
      case 1:
      case 2:
      {
        /* DebugLine("move"); */
        v3 PlayerBaseP = GetSimSpaceBaseP(World, GameState->Player);
        f32 ShortestDistanceToPlayerSq = f32_MAX;
        u32 ClosestTileIndex = u32_MAX;

        canonical_position EnemyOriginalP = Enemy->P;

        canonical_position EnemyBaseP = Enemy->P;
        EnemyBaseP.Offset += Enemy->CollisionVolumeRadius.xy;
        Canonicalize(World, &EnemyBaseP);

        f32 EnemyMoveSpeed = 8.f;
        standing_spot_buffer Spots = GetStandingSpotsWithinRadius(World, EnemyBaseP, EnemyMoveSpeed, GetTranArena());
        for (u32 SpotIndex = 0; SpotIndex < Spots.Count; ++SpotIndex)
        {
          standing_spot *Spot = Spots.Start + SpotIndex;
          v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera);
          DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), RED, DEFAULT_LINE_THICKNESS*3.f);

          v3 SpotSimP = GetSimSpaceP(World, Spot->P);
          r32 ThisDist = DistanceSq(SpotSimP, PlayerBaseP);
          if (ThisDist < ShortestDistanceToPlayerSq)
          {
            ShortestDistanceToPlayerSq = ThisDist;
            ClosestTileIndex = SpotIndex;
          }
        }

        if (GameState->DidPlayerAction && ClosestTileIndex < Spots.Count)
        {
          standing_spot *Spot = Spots.Start + ClosestTileIndex;

          v3 EnemyBaseSimP = GetSimSpaceP(World, EnemyBaseP);
          v3 SpotSimP = GetSimSpaceP(World, Spot->P);
          v3 SpotTopSimP = SpotSimP + V3(Global_StandingSpotHalfDim.xy, Global_StandingSpotDim.z);
          v3 UpdateV = SpotTopSimP - EnemyBaseSimP + V3(0,0,3);
          UpdateEntityP(World, Enemy, UpdateV);

          // Disallow enemies moving onto other entities
          collision_event EntityCollision = DoEntityCollisions(World, EntityTable, Enemy);
          if (EntityCollision.Count) { Enemy->P = EnemyOriginalP; }
        }

      } break;

      case 3:
      {
      } break;


      InvalidDefaultCase;
    }
  }
}

void
GameEntityUpdate(engine_resources *Engine, entity *Entity )
{
  UNPACK_ENGINE_RESOURCES(Engine);

  game_entity_type Type = *((game_entity_type*)&Entity->UserData);

  // Bitty offset, gets overridden by the Splosion case.  Janky af.  @offset-jank-fallthrough
  v3 Offset = V3(0.f, 0.f, 0.2f);
  switch (Type)
  {
    case GameEntityType_Unknown: {} break;;
    case GameEntityType_Enemy: { EnemyUpdate(Engine, Entity); } break;

    case GameEntityType_Splosion: { Offset = V3(0.f, 0.f, 2.5f); } _FALLTHROUGH; // @offset-jank-fallthrough
    case GameEntityType_Bitty:
    {
      v3 Color = {};
      switch (Type)
      {
        case GameEntityType_Splosion:
          Color = V3(0.97f, 0.32f, 0.03f)*70.f; break;
        case GameEntityType_Bitty:
          Color = V3(0.97f, 0.42f, 0.03f)*0.2f; break;

        InvalidDefaultCase;
      }

      v3 P = GetRenderP(World->ChunkDim, Entity, Camera) + Offset;
      r32 Intensity = Max(0.f, Entity->Emitter->RemainingLifespan / Entity->Emitter->EmissionLifespan);
      DoLight(&Lighting->Lights, P, Color*Intensity );
    } break;
  }
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  entity *Player = GameState->Player;

  /* v3 RotP = {}; */
  /* RotP.x = Sin(r32(Plat->GameTime)); */
  /* RotP.y = Cos(r32(Plat->GameTime)); */
  /* Player->Rotation = RotatePoint(V3(0.f, -1.f, 0.f), RotP); */

  /* standing_spot_buffer EnemySpots = GetStandingSpotsWithinRadius(World, Enemy->P, EnemyMoveSpeed, GetTranArena()); */
  /* for (u32 SpotIndex = 0; SpotIndex < EnemySpots.Count; ++SpotIndex) */
  /* { */
  /*   standing_spot *Spot = EnemySpots.Start + SpotIndex; */
  /*   v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera); */
  /*   DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), RED, DEFAULT_LINE_THICKNESS*3.f); */
  /* } */

  picked_voxel Pick = Resources->MousedOverVoxel;

  local_persist player_action SelectedAction = {};
  local_persist u32 PlayerChargeLevel = {};

  if (Pick.PickedChunk.tChunk != f32_MAX)
  {
    v3 VoxelP = GetAbsoluteP(&Pick);

    world_chunk *ClosestChunk = Pick.PickedChunk.Chunk;
    canonical_position PickCP = Canonical_Position(Pick.Picks[PickedVoxel_FirstFilled], ClosestChunk->WorldP);

    if (Input->Z.Clicked)
    {
      SelectedAction = PlayerAction_Move;
    }

    if (Input->X.Clicked)
    {
      SelectedAction = PlayerAction_Charge;
    }

    if (Input->C.Clicked)
    {
      SelectedAction = PlayerAction_Fire;
    }

    if (Input->V.Clicked)
    {
      SelectedAction = PlayerAction_Jump;
    }

    if (Input->R.Clicked)
    {
      DoSplotion(Resources, &Pick, PickCP, 4.f, GetTranArena());
    }

    if (Input->T.Clicked)
    {
      DoSplotion(Resources, &Pick, PickCP, 6.f, GetTranArena());
    }

    if (Input->Y.Clicked)
    {
      DoSplotion(Resources, &Pick, PickCP, 8.f, GetTranArena());
    }


    GameState->DidPlayerAction = False;
    switch (SelectedAction)
    {
      case PlayerAction_Count:
      case PlayerAction_None:
      {
      } break;

      case PlayerAction_Move:
      {
        canonical_position PlayerBaseP = Player->P;
        PlayerBaseP.Offset += Player->CollisionVolumeRadius.xy;
        PlayerBaseP = Canonicalize(World, PlayerBaseP);

        f32 PlayerMoveSpeed = 13.f;
        standing_spot_buffer PlayerSpots = GetStandingSpotsWithinRadius(World, PlayerBaseP, PlayerMoveSpeed, GetTranArena());
        v3 CursorSimP = GetSimSpaceP(World, PickCP);

        for (u32 SpotIndex = 0; SpotIndex < PlayerSpots.Count; ++SpotIndex)
        {
          standing_spot *Spot = PlayerSpots.Start + SpotIndex;
          v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera);

          v3 SpotSimP = GetSimSpaceP(World, Spot->P);
          aabb SpotSimAABB = AABBMinDim(SpotSimP, Global_StandingSpotDim);
          if (IsInside(SpotSimAABB, CursorSimP))
          {
            DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), TEAL, DEFAULT_LINE_THICKNESS*3.f);

            if (Input->LMB.Clicked)
            {
              GameState->DidPlayerAction = True;

              v3 PlayerBaseSimP = GetSimSpaceP(World, PlayerBaseP);
              v3 SpotTopSimP = SpotSimP + V3(Global_StandingSpotHalfDim.xy, Global_StandingSpotDim.z);
              v3 UpdateV = SpotTopSimP - PlayerBaseSimP + V3(0,0,2);
              UpdateEntityP(World, Player, UpdateV);
            }
          }
          else
          {
            DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), GREEN, DEFAULT_LINE_THICKNESS*3.f);
          }
        }

      } break;

      case PlayerAction_Charge:
      {
        if (Input->LMB.Clicked)
        {
          ++PlayerChargeLevel;
          SpawnFire(Player, &GameState->Entropy, Global_EntityFireballOffset, PlayerChargeLevel);
          GameState->DidPlayerAction = True;
        }
      } break;

      case PlayerAction_Fire:
      {
        if (PlayerChargeLevel)
        {
          if (Input->LMB.Clicked)
          {
            DoSplotion(Resources, &Pick, PickCP, 2.f + r32(PlayerChargeLevel)*2.f, GetTranArena());
            PlayerChargeLevel = 0.f;
            Deactivate(Player->Emitter);
            GameState->DidPlayerAction = True;
          }
        }
      } break;

      case PlayerAction_Jump:
      {
        if (Input->LMB.Clicked)
        {
          GameState->DidPlayerAction = True;
        }
      } break;
    }

    if (GameState->DidPlayerAction)
    {
      SelectedAction = PlayerAction_None;
    }

  }

  local_persist window_layout ActionsWindow = {};
  PushBorderlessWindowStart(Ui, &ActionsWindow, V2(0, 128));
  for (u32 ActionIndex = 0; ActionIndex < PlayerAction_Count; ++ActionIndex)
  {
    ui_style *Style = ActionIndex == SelectedAction ? &DefaultSelectedStyle : &DefaultStyle;
    PushTableStart(Ui);
      PushColumn(Ui, ToString((player_action)ActionIndex), Style);
    PushTableEnd(Ui);
  }
  PushWindowEnd(Ui, &ActionsWindow);

  if (Hotkeys)
  {
    r32 CameraSpeed = 125.f;
    v3 CameraDelta = (GetCameraRelativeInput(Hotkeys, Camera));

    CameraDelta.z = 0.f;
    CameraDelta = Normalize(CameraDelta) * CameraSpeed * Plat->dt;

    GameState->CameraTarget->P.Offset += CameraDelta;
  }
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/turn_based/assets");

  world_position WorldCenter = World_Position(0);
  canonical_position PlayerSpawnP = Canonical_Position(Voxel_Position(0), WorldCenter + World_Position(0,0,1));

  StandardCamera(Graphics->Camera, 10000.0f, 500.0f, PlayerSpawnP);
  /* Graphics->Camera->CurrentP.WorldP = World_Position(1, -1, 1); */
  /* Graphics->Camera->CurrentP.Offset = V3(1, -1, 1); */

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  random_series WorldEntropy = {54930695483};

  GameState->Models = AllocateGameModels(GameState, Resources->Memory, Heap);

#if 1
  u32 PlayerModelIndex = RandomBetween( u32(ModelIndex_FirstPlayerModel), &GameState->Entropy, u32(ModelIndex_LastPlayerModel+1));
  GameState->Player = GetFreeEntity(EntityTable);
  SpawnPlayerLikeEntity(Plat, World, GameState->Models + PlayerModelIndex, GameState->Player, PlayerSpawnP, &GameState->Entropy);
#endif

#if 1
  u32 EnemyCount = 3;
  v3i HalfVisibleRegion = g_VisibleRegion / 2;
  HalfVisibleRegion.z = 0;
  for (u32 EnemyIndex = 0; EnemyIndex < EnemyCount; ++EnemyIndex)
  {
    world_position WP = World_Position(
        RandomBetween(0, &GameState->Entropy, HalfVisibleRegion.x),
        RandomBetween(0, &GameState->Entropy, HalfVisibleRegion.y),
        1);

    u32 EnemyModelIndex = RandomBetween( u32(ModelIndex_FirstEnemyModel), &GameState->Entropy, u32(ModelIndex_Enemy_Skeleton_AxeArmor+1));
    Assert(EnemyModelIndex >= ModelIndex_FirstEnemyModel);
    Assert(EnemyModelIndex <= ModelIndex_LastEnemyModel);

    auto EnemySpawnP = Canonical_Position(V3(0), WorldCenter + WP );
    auto Enemy = GetFreeEntity(EntityTable);
    Enemy->UserData = (void*)GameEntityType_Enemy;
    SpawnPlayerLikeEntity(Plat, World, GameState->Models + EnemyModelIndex, Enemy, EnemySpawnP, &GameState->Entropy, 0.35f);
  }
#endif

  WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

  GameState->CameraTarget = GetFreeEntity(EntityTable);
  SpawnEntity( GameState->CameraTarget, EntityType_Default, 0, ModelIndex_None);

  GameState->CameraTarget->P = Canonical_Position(Voxel_Position(0), {{2,2,0}});

  Resources->CameraTarget = GameState->CameraTarget;

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
}
