#define DEBUG_SYSTEM_API 1
#define DO_EDITOR_UI_FOR_ENTITY_TYPE 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

global_variable v3
Global_EntityFireballOffset = V3(7.0f, -1.75f, 4.5f);

link_internal model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  // TODO(Jesse)(leak, memory): Pass temp memory to LoadVoxModel

  model *Result                                       = Allocate(model, Memory, ModelIndex_Count);

  Result[ModelIndex_Enemy_Skeleton_Axe]              = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_AXE.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_Sword]            = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_SWORD.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_Lasher]           = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_LASHER.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_Archer]           = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_ARCHER.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_Spear]            = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_SPEAR.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_AxeArmor]         = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_AXE_ARMOR.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_Hounds]           = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_HOUNDS.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_Horserider]       = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_HORSERIDER.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_Horsebanner]      = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_HORSE_BANNER.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_Shaman]           = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_SHAMAN.vox", Memory).Model;
  /* Result[ModelIndex_Enemy_Skeleton_Champion]         = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_CHAMPION.vox", Memory).Model; */
  Result[ModelIndex_Enemy_Skeleton_ChampionChampion] = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_CHAMPION_CATAPHRACT.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_Concubiner]       = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_CONCUBINER.vox", Memory).Model;
  Result[ModelIndex_Enemy_Skeleton_King]             = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_KING.vox", Memory).Model;

  Result[ModelIndex_Player_jp]      = LoadVoxModel(Memory, Heap, "models/players/chr_jp.vox", Memory).Model;
  Result[ModelIndex_Player_bow]     = LoadVoxModel(Memory, Heap, "models/players/chr_bow.vox", Memory).Model;
  Result[ModelIndex_Player_cat]     = LoadVoxModel(Memory, Heap, "models/players/chr_cat.vox", Memory).Model;
  Result[ModelIndex_Player_fox]     = LoadVoxModel(Memory, Heap, "models/players/chr_fox.vox", Memory).Model;
  Result[ModelIndex_Player_gumi]    = LoadVoxModel(Memory, Heap, "models/players/chr_gumi.vox", Memory).Model;
  Result[ModelIndex_Player_knight]  = LoadVoxModel(Memory, Heap, "models/players/chr_knight.vox", Memory).Model;
  Result[ModelIndex_Player_man]     = LoadVoxModel(Memory, Heap, "models/players/chr_man.vox", Memory).Model;
  Result[ModelIndex_Player_mom]     = LoadVoxModel(Memory, Heap, "models/players/chr_mom.vox", Memory).Model;
  Result[ModelIndex_Player_old]     = LoadVoxModel(Memory, Heap, "models/players/chr_old.vox", Memory).Model;
  Result[ModelIndex_Player_poem]    = LoadVoxModel(Memory, Heap, "models/players/chr_poem.vox", Memory).Model;
  Result[ModelIndex_Player_rain]    = LoadVoxModel(Memory, Heap, "models/players/chr_rain.vox", Memory).Model;
  Result[ModelIndex_Player_sasami]  = LoadVoxModel(Memory, Heap, "models/players/chr_sasami.vox", Memory).Model;
  Result[ModelIndex_Player_sol]     = LoadVoxModel(Memory, Heap, "models/players/chr_sol.vox", Memory).Model;
  Result[ModelIndex_Player_sword]   = LoadVoxModel(Memory, Heap, "models/players/chr_sword.vox", Memory).Model;
  Result[ModelIndex_Player_tale]    = LoadVoxModel(Memory, Heap, "models/players/chr_tale.vox", Memory).Model;
  Result[ModelIndex_Player_tama]    = LoadVoxModel(Memory, Heap, "models/players/chr_tama.vox", Memory).Model;
  Result[ModelIndex_Player_tsurugi] = LoadVoxModel(Memory, Heap, "models/players/chr_tsurugi.vox", Memory).Model;

  Result[ModelIndex_Bitty0] = LoadVoxModel(Memory, Heap, "models/splotion_bitty_0.vox", Memory).Model;
  Result[ModelIndex_Bitty1] = LoadVoxModel(Memory, Heap, "models/splotion_bitty_1.vox", Memory).Model;

  /* Result[ModelIndex_FirstPlayerModel] = LoadObj(Memory, Heap, "models/test/nature_pack.obj"); */

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

        /* if (GetCollision()) */
        /* { */
        /* } */

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
          DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), RED, DEFAULT_STANDING_SPOT_THICKNESS*3.f);

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


enum entity_type
#if !POOF_PREPROCESSOR
 : u32
#endif
{
  EntityType_Default,

  EntityType_Enemy,
  EntityType_Player,
};

poof(generate_string_table(entity_type))
#include <generated/generate_string_table_entity_type.h>

poof(do_editor_ui_for_enum(entity_type))
#include <generated/do_editor_ui_for_enum_entity_type.h>

link_weak b32
GameEntityUpdate(engine_resources *Engine, entity *Entity )
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Assert(Spawned(Entity));

  entity_type Type = Entity->Type;

  switch (Type)
  {
    case EntityType_Enemy: { EnemyUpdate(Engine, Entity); } break;
  }

  return False;
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
  /*   DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), RED, DEFAULT_STANDING_SPOT_THICKNESS*3.f); */
  /* } */


  local_persist player_action SelectedAction = {};
  local_persist u32 PlayerChargeLevel = {};

  if (Resources->MousedOverVoxel.Tag)
  {
    picked_voxel Pick = Resources->MousedOverVoxel.Value;
    v3 VoxelP = GetAbsoluteP(&Pick);

    world_chunk *ClosestChunk = Pick.PickedChunk.Chunk;
    canonical_position PickCP = Canonical_Position(Pick.Picks[PickedVoxel_LastEmpty], ClosestChunk->WorldP);

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
      DoSplotion(Resources, PickCP, 5.f, &Global_GameEntropy, GetTranArena());
      /* DoIceBlock(Resources, &Pick, PickCP, 4.f, GetTranArena()); */
    }

    if (Input->T.Clicked)
    {
      DoSplotion(Resources, PickCP, 7.f, &Global_GameEntropy, GetTranArena());
      /* DoIceBlock(Resources, &Pick, PickCP, 6.f, GetTranArena()); */
    }

    if (Input->Y.Clicked)
    {
      DoSplotion(Resources, PickCP, 9.f,  &Global_GameEntropy,GetTranArena());
      /* DoIceBlock(Resources, &Pick, PickCP, 8.f, GetTranArena()); */
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
            DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), TEAL, DEFAULT_STANDING_SPOT_THICKNESS*3.f);

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
            DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), GREEN, DEFAULT_STANDING_SPOT_THICKNESS*3.f);
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
            DoSplotion(Resources, PickCP, 2.f + r32(PlayerChargeLevel)*2.f, &Global_GameEntropy, GetTranArena());
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

    GameState->CameraGhost->P.Offset += CameraDelta;
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
  /* u32 PlayerModelIndex = RandomBetween( u32(ModelIndex_FirstPlayerModel), &GameState->Entropy, u32(ModelIndex_LastPlayerModel+1)); */
  /* u32 PlayerModelIndex = ModelIndex_FirstPlayerModel; */
  u32 PlayerModelIndex = ModelIndex_Player_old;
  GameState->Player = GetFreeEntity(EntityTable);
  GameState->Player->Type = EntityType_Player;
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

    u32 EnemyModelIndex = RandomBetween( u32(ModelIndex_Enemy_Skeleton_Axe), &GameState->Entropy, u32(ModelIndex_Enemy_Skeleton_King+1));
    /* Assert(EnemyModelIndex >= ModelIndex_FirstEnemyModel); */
    /* Assert(EnemyModelIndex <= ModelIndex_LastEnemyModel); */

    auto EnemySpawnP = Canonical_Position(V3(0), WorldCenter + WP );
    auto Enemy = GetFreeEntity(EntityTable);
    /* Enemy->UserData = (void*)GameEntityBehaviorFlags_Enemy; */
    SpawnPlayerLikeEntity(Plat, World, GameState->Models + EnemyModelIndex, Enemy, EnemySpawnP, &GameState->Entropy, 0.35f);
  }
#endif

  WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

  GameState->CameraGhost = GetFreeEntity(EntityTable);
  SpawnEntity( GameState->CameraGhost );

  GameState->CameraGhost->P = Canonical_Position(Voxel_Position(0), {{2,2,0}});

  Resources->CameraGhost = GameState->CameraGhost;

  return GameState;
}
