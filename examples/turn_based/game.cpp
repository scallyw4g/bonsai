#define DEBUG_SYSTEM_API 1
#define DO_EDITOR_UI_FOR_ENTITY_TYPE 1

#include <bonsai_types.h>

#include "game_constants.h"
#include "game_types.h"

global_variable v3
Global_EntityFireballOffset = V3(0,0,16);
/* Global_EntityFireballOffset = V3(7.0f, -1.75f, 4.5f); */

link_internal model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  // TODO(Jesse)(leak, memory): Pass temp memory to LoadVoxModels

  model *Result                                       = Allocate(model, Memory, ModelIndex_Count);

  Result[ModelIndex_Enemy_Skeleton_Axe]              = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_AXE.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_Sword]            = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_SWORD.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_Lasher]           = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_LASHER.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_Archer]           = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_ARCHER.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_Spear]            = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_SPEAR.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_AxeArmor]         = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_AXE_ARMOR.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_Hounds]           = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_HOUNDS.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_Horserider]       = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_HORSERIDER.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_Horsebanner]      = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_HORSE_BANNER.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_Shaman]           = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_SHAMAN.vox", Memory).Value.Start[0];
  /* Result[ModelIndex_Enemy_Skeleton_Champion]         = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_CHAMPION.vox", Memory).Value.Start[0]; */
  Result[ModelIndex_Enemy_Skeleton_ChampionChampion] = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_CHAMPION_CATAPHRACT.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_Concubiner]       = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_CONCUBINER.vox", Memory).Value.Start[0];
  Result[ModelIndex_Enemy_Skeleton_King]             = LoadVoxModels(Memory, Heap, "models/skeletons/SKELLINGTON_KING.vox", Memory).Value.Start[0];

  Result[ModelIndex_Player_jp]      = LoadVoxModels(Memory, Heap, "models/players/chr_jp.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_bow]     = LoadVoxModels(Memory, Heap, "models/players/chr_bow.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_cat]     = LoadVoxModels(Memory, Heap, "models/players/chr_cat.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_fox]     = LoadVoxModels(Memory, Heap, "models/players/chr_fox.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_gumi]    = LoadVoxModels(Memory, Heap, "models/players/chr_gumi.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_knight]  = LoadVoxModels(Memory, Heap, "models/players/chr_knight.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_man]     = LoadVoxModels(Memory, Heap, "models/players/chr_man.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_mom]     = LoadVoxModels(Memory, Heap, "models/players/chr_mom.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_old]     = LoadVoxModels(Memory, Heap, "models/players/chr_old.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_poem]    = LoadVoxModels(Memory, Heap, "models/players/chr_poem.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_rain]    = LoadVoxModels(Memory, Heap, "models/players/chr_rain.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_sasami]  = LoadVoxModels(Memory, Heap, "models/players/chr_sasami.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_sol]     = LoadVoxModels(Memory, Heap, "models/players/chr_sol.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_sword]   = LoadVoxModels(Memory, Heap, "models/players/chr_sword.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_tale]    = LoadVoxModels(Memory, Heap, "models/players/chr_tale.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_tama]    = LoadVoxModels(Memory, Heap, "models/players/chr_tama.vox", Memory).Value.Start[0];
  Result[ModelIndex_Player_tsurugi] = LoadVoxModels(Memory, Heap, "models/players/chr_tsurugi.vox", Memory).Value.Start[0];

  Result[ModelIndex_Bitty0] = LoadVoxModels(Memory, Heap, "models/splotion_bitty_0.vox", Memory).Value.Start[0];
  Result[ModelIndex_Bitty1] = LoadVoxModels(Memory, Heap, "models/splotion_bitty_1.vox", Memory).Value.Start[0];

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


link_internal void
EnemyUpdate(engine_resources *Engine, entity *Enemy)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  /* if (GameState->PlayerActed) */
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

        canonical_position EnemyBaseP = GetEntityBaseP(World, Enemy);

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

        if (GameState->PlayerActed && ClosestTileIndex < Spots.Count)
        {
          standing_spot *Spot = Spots.Start + ClosestTileIndex;

          v3 EnemyBaseSimP = GetSimSpaceP(World, EnemyBaseP);
          v3 SpotSimP = GetSimSpaceP(World, Spot->P);
          v3 SpotTopSimP = SpotSimP + V3(Global_StandingSpotHalfDim.xy, Global_StandingSpotDim.z);
          v3 UpdateV = SpotTopSimP - EnemyBaseSimP + V3(0,0,3);
          UpdateEntityP(World, Enemy, UpdateV);

          // Disallow enemies moving onto other entities
          collision_event EntityCollision = DoEntityCollisions(World, EntityTable, Enemy).Collision;
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


link_internal void
FireballUpdate(engine_resources *Engine, entity *Entity)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  fireball_state *State = (fireball_state*)Entity->UserData;

  if (Engine->GameState->TransitionDuration > 0.35f)
  {
    v3 EntityP = GetSimSpaceP(World, Entity);
    v3 TargetP = GetSimSpaceP(World, State->TargetP);

    Entity->Physics.Velocity = Normalize(TargetP-EntityP)*800.f;
    Entity->Behavior = entity_behavior_flags(Entity->Behavior & ~EntityBehaviorFlags_Gravity);
  }

  if (Entity->CollisionLastFrame.Count)
  {
    r32 Radius = 2.f + r32(State->ChargeLevel)*2.f;
    DoSplotion(Engine, Entity->P, Radius, &Global_GameEntropy, GetTranArena());

    v3 SimP = GetSimSpaceP(World, Entity->P);

    /* r32 Radius = 2.f + r32(GameState->PlayerChargeLevel)*2.f; */
    sphere S = {SimP, Radius};

    u32_buffer EntityIndices = GatherEntitiesIntersecting(World, EntityTable, &S, GetTranArena());

    IterateOver(&EntityIndices, EIndex, EIndexIndex)
    {
      entity *E = EntityTable[*EIndex];
      if (E->UserType == EntityType_Enemy)
      {
        Unspawn(E);

        cs AssetNames[] =
        {
          CSz("skele_bitty_0.vox"),
          CSz("skele_bitty_1.vox"),
          CSz("skele_bitty_2.vox"),
        };

        s32 MaxBitties = ArrayCount(AssetNames);
        RangeIterator(BittyIndex, MaxBitties)
        {
          // TODO(Jesse)(leak): This leaks the asset name when the asset is freed
          file_traversal_node AssetName = {FileTraversalType_File, CSz("models"), AssetNames[BittyIndex]};
          asset_id AID = AssetId(&AssetName);

          entity *BittyEntity = GetFreeEntity(EntityTable);

          BittyEntity->AssetId = AID;

          SpawnEntity(BittyEntity, EntityBehaviorFlags_Default, {}, {});
          BittyEntity->Physics.Speed = 1.f;

          BittyEntity->EulerAngles.z = RandomUnilateral(&Global_GameEntropy)*PI32*2.f;
          BittyEntity->Scale = 1.0f;
          BittyEntity->CollisionVolumeRadius = V3(.1f);

          v3 Rnd = RandomV3Bilateral(&Global_GameEntropy);
          BittyEntity->Physics.Mass = 25.f;
          BittyEntity->Physics.Force += Rnd*150.f*Radius;
          BittyEntity->Physics.Force.z = Abs(BittyEntity->Physics.Force.z) * 0.25f;
          BittyEntity->P = Entity->P + (Rnd*Radius) + V3(0.f, 0.f, 2.0f);
          BittyEntity->P.Offset.z = Entity->P.Offset.z + 2.f;

          if (GetCollision(World, BittyEntity).Count) { Unspawn(BittyEntity); continue; }
        }
      }
    }


    HeapDeallocate(&GameState->Heap, (u8*)State);
    Unspawn(Entity);

  }
}

link_weak b32
GameEntityUpdate(engine_resources *Engine, entity *Entity )
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Assert(Spawned(Entity));

  entity_type Type = Cast(entity_type, Entity->UserType);

  switch (Type)
  {
    case EntityType_Player:   {} break;
    case EntityType_Default:  {} break;
    case EntityType_Enemy:    { EnemyUpdate(Engine, Entity); } break;

    case EntityType_Fireball: { ++GameState->FireballsSimulated; FireballUpdate(Engine, Entity); } break;
  }

  return False;
}


BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  b32 Result = True;
  switch (Entry->Type)
  {
    default: { Result = False; } break;

    // NOTE(Jesse): Here we're going to provide an implementation for
    // initializing a world chunk.
    case type_work_queue_entry_init_world_chunk:
    {
      volatile work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;

      if (ChunkIsGarbage(Chunk))
      {
        // NOTE(Jesse): This is an optimization; the engine marks chunks that
        // have moved outside of the visible region as garbage.
        Chunk->Flags = Chunk_Uninitialized;
      }
      else
      {
        // Custom FBM noise example generating slightly-more-complex game-world-like terrain
        s32 Frequency = 0; // Ignored
        s32 Amplititude = 0; // Ignored
        s32 StartingZDepth = -85;
        u32 OctaveCount = 1;

        octave_buffer OctaveBuf = { OctaveCount, {} };
        OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

        OctaveBuf.Octaves[0] = {V3(400, 400, 180), 150, V3(1)};
        /* OctaveBuf.Octaves[1] = {V3(35, 35, 50),      6, V3(2.f)}; */
        /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
        /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
        /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


        /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
        /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
        chunk_init_flags InitFlags = ChunkInitFlag_Noop;
        InitializeChunkWithNoise( TerracedTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
      }
    }
  }

  return Result;
}

link_internal physics
FireballPhysics()
{
  physics Result = {};

  Result.Mass = 5.f;
  Result.Speed = 6.f;

  return Result;
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


  if (Resources->MousedOverVoxel.Tag)
  {
    picked_voxel Pick = Resources->MousedOverVoxel.Value;
    v3 VoxelP = GetAbsoluteP(&Pick);

    world_chunk *ClosestChunk = Pick.Chunks[PickedVoxel_LastEmpty].Chunk;
    canonical_position PickCP = Pick.Picks[PickedVoxel_LastEmpty];

    if (Input->Z.Clicked)
    {
      GameState->ProposedAction = PlayerAction_Move;
    }

    if (Input->X.Clicked)
    {
      GameState->ProposedAction = PlayerAction_Charge;
    }

    if (Input->C.Clicked)
    {
      GameState->ProposedAction = PlayerAction_Fire;
    }

    if (Input->V.Clicked)
    {
      GameState->ProposedAction = PlayerAction_Jump;
    }

    switch (GameState->ProposedAction)
    {
      InvalidCase(PlayerAction_Count);

      case PlayerAction_None: { } break;

      case PlayerAction_Move:
      {
        cp PlayerBaseP = GetEntityBaseP(World, Player);

        f32 PlayerMoveSpeed = 13.f;
        standing_spot_buffer PlayerSpots = GetStandingSpotsWithinRadius(World, PlayerBaseP, PlayerMoveSpeed, GetTranArena());

        v3 CursorSimP = GetSimSpaceP(World, PickCP);

        r32 LowestDistance = r32_MAX;
        umm LowestIndex = 0;

        RangeIterator_t(umm, SpotIndex, PlayerSpots.Count)
        {
          standing_spot *Spot = PlayerSpots.Start + SpotIndex;

          v3 SpotSimP = GetSimSpaceP(World, GetSpotMidpoint(World, Spot));

          r32 Len = LengthSq(SpotSimP-CursorSimP);
          if (Len < LowestDistance)
          {
            LowestDistance = Len;
            LowestIndex = SpotIndex;
          }

          {
            v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera);
            DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), GREEN, DEFAULT_STANDING_SPOT_THICKNESS*3.f);
          }
        }

        if (PlayerSpots.Count)
        {
          standing_spot *Spot = PlayerSpots.Start + LowestIndex;
          v3 SpotSimP = GetSimSpaceP(World, Spot->P);
          v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera);
          DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), TEAL, DEFAULT_STANDING_SPOT_THICKNESS*4.f);

          if (Input->LMB.Clicked)
          {
            v3 PlayerBaseSimP = GetSimSpaceP(World, PlayerBaseP);
            v3 SpotTopSimP = SpotSimP + V3(Global_StandingSpotHalfDim.xy, Global_StandingSpotDim.z);
            v3 UpdateV = SpotTopSimP - PlayerBaseSimP + V3(0,0,2);
            UpdateEntityP(World, Player, UpdateV);
          }
        }


      } break;

      case PlayerAction_Charge:
      {
        if (Input->LMB.Clicked)
        {
          GameState->PlayerChargeLevel += 2;
          SpawnFire( Player,
                    &GameState->Entropy,
                     Global_EntityFireballOffset + Player->CollisionVolumeRadius.xy,
                     GameState->PlayerChargeLevel);
        }
      } break;

      case PlayerAction_Fire:
      {
        if (GameState->PlayerChargeLevel)
        {
          if (Input->LMB.Clicked)
          {
            entity *E = GetFreeEntity(EntityTable);
            SpawnEntity(E);

            *E->Emitter = *Player->Emitter;
            E->P = Player->P;
            E->P += E->Emitter->SpawnRegion.Min;

            E->Emitter->SpawnRegion.Max -= E->Emitter->SpawnRegion.Min;
            E->Emitter->SpawnRegion.Min = {};

            E->Physics = FireballPhysics();

            E->Behavior = EntityBehaviorFlags_Default;
            E->UserType = EntityType_Fireball;

            E->CollisionVolumeRadius = V3(1); // TODO(Jesse): Should be based on charge level?

            /* fireball_state *FireballState = Allocate(fireball_state, &GameState->Heap, 1); */
            fireball_state *FireballState = (fireball_state*)HeapAllocate(&GameState->Heap, sizeof(fireball_state));

            FireballState->ChargeLevel = GameState->PlayerChargeLevel;
            FireballState->TargetP = PickCP;

            v3 EntityP = GetSimSpaceP(World, E);
            v3 TargetP = GetSimSpaceP(World, PickCP);

            E->Physics.Velocity = (Normalize(TargetP-EntityP) + V3(0,0,5)) * 10.f;

            GameState->PlayerChargeLevel = 0;

            E->UserData = u64(FireballState);

            Deactivate(Player->Emitter);

          }
        }
      } break;

      case PlayerAction_Jump:
      {
      } break;
    }

    if ( GameState->TurnMode == TurnMode_Transition &&
         GameState->FireballsSimulated  == 0 )
    {
      GameState->TurnMode = TurnMode_Default;
    }
    GameState->FireballsSimulated = 0;


    GameState->PlayerActed = GameState->ProposedAction && Input->LMB.Clicked;
    if (GameState->PlayerActed)
    {
      GameState->TurnMode = TurnMode_Transition;
      GameState->TransitionDuration = 0.f;
    }

    if (GameState->TurnMode == TurnMode_Transition)
    {
      /* if (GameState->TransitionDuration > 1.2f) { GameState->TurnMode = TurnMode_Default; } */
      GameState->TransitionDuration += Plat->dt;

      switch (GameState->ProposedAction)
      {
        InvalidCase(PlayerAction_Count);

        case PlayerAction_None: {} break;

        case PlayerAction_Move:
        {
        } break;

        case PlayerAction_Charge:
        {
        } break;

        case PlayerAction_Fire:
        {
        } break;

        case PlayerAction_Jump:
        {
        } break;
      }

      GameState->ProposedAction = PlayerAction_None;
    }

  }

  local_persist window_layout ActionsWindow = WindowLayout("ActionsWindow");
  PushBorderlessWindowStart(Ui, &ActionsWindow);

  v2 WindowDim = GetDim(&ActionsWindow);
  v2 WindowOffset = V2(Plat->WindowWidth/2.f-(WindowDim.y/2.f), Plat->WindowHeight-WindowDim.y);

  ActionsWindow.Basis = WindowOffset;

    ui_element_reference ActionTable = PushTableStart(Ui);
      for (u32 ActionIndex = PlayerAction_Move; ActionIndex < PlayerAction_Count; ++ActionIndex)
      {
        PushTableStart(Ui);
          ui_style *Style = ActionIndex == GameState->ProposedAction ? &DefaultSelectedStyle : &DefaultStyle;
          PushColumn(Ui, ToString((player_action)ActionIndex), Style);
        PushTableEnd(Ui);
      }
    PushTableEnd(Ui);

    PushTableStart(Ui, Position_RightOf, ActionTable);
      PushColumn(Ui, ToString(GameState->TurnMode));
    PushTableEnd(Ui);

  PushWindowEnd(Ui, &ActionsWindow);

}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  GameState->Heap = InitHeap(Megabytes(4));

  Global_AssetPrefixPath = CSz("examples/turn_based/assets");

  world_position WorldCenter = World_Position(5, -4, 0);
  canonical_position PlayerSpawnP = Canonical_Position(Voxel_Position(0), WorldCenter + World_Position(0,0,1));

  StandardCamera(Graphics->Camera, 10000.0f, 500.0f, PlayerSpawnP);
  /* Graphics->Camera->CurrentP.WorldP = WorldCenter; */
  /* Graphics->Camera->CurrentP.Offset = V3(1, -1, 1); */

  GameState->Entropy = {DEBUG_NOISE_SEED};

  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  GameState->Models = AllocateGameModels(GameState, Resources->Memory, Heap);

#if 1
  /* u32 PlayerModelIndex = RandomBetween( u32(ModelIndex_FirstPlayerModel), &GameState->Entropy, u32(ModelIndex_LastPlayerModel+1)); */
  /* u32 PlayerModelIndex = ModelIndex_FirstPlayerModel; */
  u32 PlayerModelIndex = ModelIndex_Player_old;
  GameState->Player = GetFreeEntity(EntityTable);
  GameState->Player->UserType = Cast(u32, EntityType_Player);
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

  Resources->CameraGhost = GetFreeEntity(EntityTable);;
  SpawnEntity( Resources->CameraGhost );

  return GameState;
}

BONSAI_API_ON_LIBRARY_RELOAD()
{
  Info("Game ON_LIBRARY_LOAD");

  UNPACK_ENGINE_RESOURCES(Resources);

  Resources->CameraGhost = GetFreeEntity(EntityTable);;
  SpawnEntity( Resources->CameraGhost );


  GameState->Player = 0;
  RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
  {
    entity *E = EntityTable[EntityIndex];
    if (E->UserType == EntityType_Player)
    {
      if (GameState->Player) { Warn("Multiple Player entities detected!"); }
      GameState->Player = E;
    }
  }
}
