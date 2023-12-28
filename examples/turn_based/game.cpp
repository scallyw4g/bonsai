#define DEBUG_SYSTEM_API 1
#define DO_EDITOR_UI_FOR_ENTITY_TYPE 1

#include <bonsai_types.h>

#include "game_constants.h"
#include "game_types.h"

global_variable v3
Global_EntityFireballOffset = V3(0,0,16);
/* Global_EntityFireballOffset = V3(7.0f, -1.75f, 4.5f); */

global_variable f32
Global_MeleeRange = 10.f;

#if 0
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
#endif

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

  /* file_traversal_node AssetName = {FileTraversalType_File, CSz("models"), CSz("skele_base.vox")}; */
  /* Enemy->AssetId = AssetId(&AssetName); */

  entity *Player = GameState->Player;
  cp EnemyBaseP  = GetEntityBaseP(World, Enemy);
  cp PlayerBaseP = GetEntityBaseP(World, Player);

  v3 PlayerBaseSimP = GetSimSpaceP(World, PlayerBaseP);
  v3 EnemyBaseSimP  = GetSimSpaceP(World, EnemyBaseP);

  /* DEBUG_DrawSimSpaceVectorAt(Engine, EnemyBaseSimP, V3(0, 0, 100), RED, 0.25f); */

  // NOTE(Jesse): Entities embedded in the world cannot act
  if (GetCollision(World, Enemy).Count) { return; }

  u16 StandingSpotColor = YELLOW;
  r32 StandingSpotRadius = DEFAULT_STANDING_SPOT_THICKNESS;

  if (Enemy == Engine->HoverEntity.Value)
  {
    StandingSpotColor = ORANGE;
    StandingSpotRadius *= 2.f;
  }

  f32 EnemySightDistance = 60.f;
  standing_spot_buffer SightedSpots = GetStandingSpotsWithinRadius(World, EnemyBaseP, EnemySightDistance, GetTranArena());

  f32 ShortestDistanceToPlayerSq = f32_MAX;
  umm ClosestSpotIndex = umm_MAX;
  IterateOver(&SightedSpots, Spot, SpotIndex)
  {
    DrawStandingSpot(&GpuMap->Buffer, Camera, Spot, StandingSpotColor, StandingSpotRadius);

    v3 SpotSimP = GetSimSpaceCenterP(World, Spot);
    r32 ThisDist = DistanceSq(SpotSimP, PlayerBaseSimP);
    if (ThisDist < ShortestDistanceToPlayerSq)
    {
      ShortestDistanceToPlayerSq = ThisDist;
      ClosestSpotIndex = SpotIndex;
    }
  }

  if (LengthSq(PlayerBaseSimP-EnemyBaseSimP) < Square(EnemySightDistance))
  {
    /* maybe_standing_spot MaybeSpot = AStarPathfind(&Spots, EnemyBaseP, PlayerBaseP, EnemyMoveSpeed); */
    /* maybe_standing_spot MaybeSpot = {Maybe_Yes, Spots.Start[ClosestSpotIndex]}; */
    f32 EnemyMovespeed = 10.f;

    memory_arena *Temp = GetTranArena();
    temp_memory_handle TMH = BeginTemporaryMemory(Temp);

    standing_spot_buffer MoveSpots = GetStandingSpotsWithinRadius(World, &SightedSpots, EnemyBaseP, EnemyMovespeed, Temp, Temp);
    maybe_standing_spot MaybeSpot = GetClosestToP(World, &MoveSpots, PlayerBaseP);
    standing_spot *Spot = &MaybeSpot.Value;

    DrawStandingSpot(&GpuMap->Buffer, Camera, Spot, RED, StandingSpotRadius*2.f);

    if (GameState->PlayerActed)
    {
      v3 SpotSimP = GetSimSpaceBaseP(World, Spot);
      v3 SpotTopSimP = SpotSimP + V3(Global_StandingSpotHalfDim.xy, Global_StandingSpotDim.z);
      v3 UpdateV = SpotTopSimP - EnemyBaseSimP + V3(0,0,3);

      cp EnemyOriginalP = Enemy->P;
      UpdateEntityP(World, Enemy, UpdateV);

      // Disallow enemies moving onto other entities
      collision_event EntityCollision = DoEntityCollisions(World, EntityTable, Enemy).Collision;
      if (EntityCollision.Count) { Enemy->P = EnemyOriginalP; }
    }
  }
}

link_internal void
DestroyLoot(engine_resources *Engine, entity *Entity)
{
  file_traversal_node AssetName = {FileTraversalType_File, CSz("models"), CSz("skull_broken.vox")};

  Entity->AssetId = AssetId(&AssetName);
  Entity->EulerAngles = V4(0.f, 5.32f, RandomBilateral(&Global_GameEntropy), 0.f);

  Entity->UserType = 0;
}

link_internal void
DestroySkeleton(engine_resources *Engine, entity *Entity, r32 Radius)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  cs AssetNames[] =
  {
    CSz("skele_bitty_0.vox"),
    CSz("skele_bitty_1.vox"),
    CSz("skele_bitty_2.vox"),
  };

#if 1
  s32 MaxBitties = ArrayCount(AssetNames);
  RangeIterator(BittyIndex, MaxBitties)
  {
    // TODO(Jesse)(leak): This leaks the asset name when the asset is freed
    file_traversal_node AssetName = {FileTraversalType_File, CSz("models"), AssetNames[BittyIndex]};
    asset_id AID = AssetId(&AssetName);

    entity *BittyEntity = GetFreeEntity(EntityTable);

    BittyEntity->AssetId = AID;

    BittyEntity->Physics.Speed = 1.f;

    BittyEntity->EulerAngles.z = RandomUnilateral(&Global_GameEntropy)*PI32*2.f;
    BittyEntity->Scale = 1.0f;

    /* UpdateCollisionVolumeRadius(World, BittyEntity, V3(.1f), GetTranArena()); */
    BittyEntity->_CollisionVolumeRadius = V3(.1f);

    v3 Rnd = RandomV3Bilateral(&Global_GameEntropy);
    BittyEntity->Physics.Mass = 25.f;
    BittyEntity->Physics.Force += Rnd*150.f*Radius;
    BittyEntity->Physics.Force.z = Abs(BittyEntity->Physics.Force.z) * 0.25f;
    BittyEntity->P = Entity->P + (Rnd*Radius) + V3(0.f, 0.f, 2.0f);
    BittyEntity->P.Offset.z = Entity->P.Offset.z + 2.f;

    SpawnEntity(BittyEntity, EntityBehaviorFlags_Default);

    /* if (GetCollision(World, BittyEntity).Count) { Unspawn(BittyEntity); continue; } */
  }
#endif


  file_traversal_node SkullAssetNames[] =
  {
    {FileTraversalType_File, CSz("models"), CSz("skull_broken.vox")},
    {FileTraversalType_File, CSz("models"), CSz("skull.vox")},
  };

  /* s32 AssetIndex = RandomBetween(0, &Global_GameEntropy, ArrayCount(SkullAssetNames)); */
  s32 AssetIndex = 1;

  Entity->AssetId = AssetId(&SkullAssetNames[AssetIndex]);
  Entity->Physics.Velocity.z += 7.f;
  Entity->P.Offset.z += 10.f;
  Entity->UserType = EntityType_Loot;

  if (AssetIndex == 0)
  {
    Entity->EulerAngles = V4(0.f, 5.32f, RandomBilateral(&Global_GameEntropy), 0.f);
  }
}

link_internal b32
EffectFireballEntity(engine_resources *Engine, entity *Enemy)
{
  b32 Result = False;

  switch (entity_type(Enemy->UserType))
  {
    case EntityType_Default: {} break;
    case EntityType_Player: {} break;
    case EntityType_Fireball: {} break;
    case EntityType_Enemy: {Result = True; DestroySkeleton(Engine, Enemy, 5.f);} break;
    case EntityType_Loot: {Result = True; DestroyLoot(Engine, Enemy);} break;
  }
  return Result;
}

link_internal b32
EffectSmackEntity(engine_resources *Engine, entity *Enemy)
{
  b32 Result = False;

  switch (entity_type(Enemy->UserType))
  {
    case EntityType_Default: {} break;
    case EntityType_Player: {} break;
    case EntityType_Fireball: {} break;
    case EntityType_Enemy: {Result = True; DestroySkeleton(Engine, Enemy, 5.f);} break;
    case EntityType_Loot: {Result = True; DestroyLoot(Engine, Enemy);} break;
  }
  
  return Result;
}

link_internal void
FireballUpdate(engine_resources *Engine, entity *FireballEntity)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  fireball_state *State = (fireball_state*)FireballEntity->UserData;

  v3 EntityP = GetSimSpaceP(World, FireballEntity);

  if (Engine->GameState->TransitionDuration > 0.45f)
  {
    v3 TargetP = GetSimSpaceP(World, State->TargetP);

    FireballEntity->Physics.Velocity = Normalize(TargetP-EntityP)*800.f;
    /* FireballEntity->Physics.Velocity = Normalize(TargetP-EntityP)*10.f; */
    FireballEntity->Behavior = entity_behavior_flags(FireballEntity->Behavior & ~EntityBehaviorFlags_Gravity);

  }

  /* DEBUG_DrawSimSpaceVectorAt(Engine, EntityP, FireballEntity->Physics.Velocity*10000, RED); */
  /* DEBUG_DrawSimSpaceVectorAt(Engine, EntityP, FireballEntity->Physics.Velocity, RED); */

  if (FireballEntity->LastResolvedCollision.Count)
  {
    r32 Radius = 2.f + r32(State->ChargeLevel)*2.f;
    DoSplotion(Engine, FireballEntity->P, Radius, &Global_GameEntropy, GetTranArena());

    v3 SimP = GetSimSpaceP(World, FireballEntity->P);

    sphere S = {SimP, Radius};

    u32_buffer EntityIndices = GatherEntitiesIntersecting(World, EntityTable, &S, GetTranArena());

    IterateOver(&EntityIndices, EIndex, EIndexIndex)
    {
      entity *E = EntityTable[*EIndex];
      EffectFireballEntity(Engine, E);
    }

    HeapDeallocate(&GameState->Heap, (u8*)State);
    Unspawn(FireballEntity);
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
    case EntityType_Loot:     {} break;
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

  /* file_traversal_node AssetName = {FileTraversalType_File, CSz("models/players"), CSz("chr_old.vox")}; */
  /* Player->AssetId = AssetId(&AssetName); */

  GameState->PlayerActed = False;

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
    canonical_position LastEmptyMouseVoxel = Pick.Picks[PickedVoxel_LastEmpty];
    canonical_position FirstFilledMouseVoxel = Pick.Picks[PickedVoxel_FirstFilled];

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
      GameState->ProposedAction = PlayerAction_IceBlock;
    }

    if (Input->B.Clicked)
    {
      GameState->ProposedAction = PlayerAction_Dig;
    }

    if (Input->N.Clicked)
    {
      GameState->ProposedAction = PlayerAction_ShovelSmack;
    }



    if (GameState->TurnMode == TurnMode_Default)
    {
      switch (GameState->ProposedAction)
      {
        InvalidCase(PlayerAction_Count);

        case PlayerAction_None: { } break;

        case PlayerAction_Move:
        {
          cp PlayerBaseP = GetEntityBaseP(World, Player);

          f32 PlayerMoveSpeed = 13.f;
          standing_spot_buffer PlayerSpots = GetStandingSpotsWithinRadius(World, PlayerBaseP, PlayerMoveSpeed, GetTranArena());

          v3 CursorSimP = GetSimSpaceP(World, LastEmptyMouseVoxel);

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
              GameState->PlayerActed = True;

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
            GameState->PlayerActed = True;

            GameState->PlayerChargeLevel += 2;
            SpawnFire( Player,
                      &GameState->Entropy,
                       Global_EntityFireballOffset + Player->_CollisionVolumeRadius.xy,
                       GameState->PlayerChargeLevel);
          }
        } break;

        case PlayerAction_Fire:
        {
          if (GameState->PlayerChargeLevel)
          {
            if (Input->LMB.Clicked)
            {
              GameState->PlayerActed = True;

              entity *E = GetFreeEntity(EntityTable);

              E->UserType = EntityType_Fireball;

              auto CP = Player->P;
              CP.Offset += Player->Emitter->SpawnRegion.Min;
              Canonicalize(World, &CP);

              *E->Emitter = *Player->Emitter;
              E->Emitter->SpawnRegion.Max -= E->Emitter->SpawnRegion.Min;
              E->Emitter->SpawnRegion.Min = {};

              physics Physics = FireballPhysics();
              SpawnEntity(
                E,
                0,
                EntityBehaviorFlags_Default,
                &Physics,
                &CP,
                V3(1)
              );

              /* fireball_state *FireballState = Allocate(fireball_state, &GameState->Heap, 1); */
              fireball_state *FireballState = (fireball_state*)HeapAllocate(&GameState->Heap, sizeof(fireball_state));

              FireballState->ChargeLevel = GameState->PlayerChargeLevel;
              FireballState->TargetP = FirstFilledMouseVoxel;

              v3 EntityP = GetSimSpaceP(World, E);
              v3 TargetP = GetSimSpaceP(World, FireballState->TargetP);

              E->Physics.Velocity = (Normalize(TargetP-EntityP) + V3(0,0,3)) * 20.f;
              /* E->Physics.Velocity = V3(10.f); */

              GameState->PlayerChargeLevel = 0;

              E->UserData = u64(FireballState);

              Deactivate(Player->Emitter);

            }
          }
        } break;

        case PlayerAction_Dig:
        {
          if (Resources->ClosestStandingSpotToCursor.Tag) { DrawStandingSpot(&GpuMap->Buffer, Camera, &Resources->ClosestStandingSpotToCursor.Value, DIRT, 0.5f); }

          if (Input->LMB.Clicked)
          {
            GameState->PlayerActed = True;
            cp StandingSpotCenterP = Canonicalize(World, Resources->ClosestStandingSpotToCursor.Value.P + Global_StandingSpotHalfDim);
            DoDig(Resources, StandingSpotCenterP, 5.f, GetTranArena());
          }
        } break;

        case PlayerAction_IceBlock:
        {
          if (Resources->ClosestStandingSpotToCursor.Tag) { DrawStandingSpot(&GpuMap->Buffer, Camera, &Resources->ClosestStandingSpotToCursor.Value, BLUE, 0.5f); }

          if (Input->LMB.Clicked)
          {
            GameState->PlayerActed = True;
            cp StandingSpotCenterP = Canonicalize(World, Resources->ClosestStandingSpotToCursor.Value.P + Global_StandingSpotHalfDim);
            DoIceBlock(Resources, StandingSpotCenterP, 4.f, GetTranArena());
          }
        } break;

        case PlayerAction_ShovelSmack:
        {
          {
            if (Resources->HoverEntity.Tag)
            {
              entity *Enemy = Resources->HoverEntity.Value;
              {
                if (DistanceSqBetweenBasePoints(World, Enemy, Player) < Square(Global_MeleeRange))
                {
                  HighlightEntity(Resources, Enemy);
                  if (Input->LMB.Clicked)
                  {
                    GameState->PlayerActed = EffectSmackEntity(Resources, Enemy);
                  }
                }
              }
            }
          }
        }
      }
    }

    if (Input->RMB.Clicked) { GameState->ProposedAction = PlayerAction_None; }

    if ( GameState->TurnMode == TurnMode_Transition &&
         GameState->FireballsSimulated == 0 )
    {
      GameState->TurnMode = TurnMode_Default;
    }
    GameState->FireballsSimulated = 0;


    if (GameState->PlayerActed)
    {
      GameState->TurnMode = TurnMode_Transition;
      GameState->TransitionDuration = 0.f;
    }

    if (GameState->TurnMode == TurnMode_Transition)
    {
      /* if (GameState->TransitionDuration > 1.2f) { GameState->TurnMode = TurnMode_Default; } */
      GameState->TransitionDuration += Plat->dt;

#if 0
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

        case PlayerAction_IceBlock:
        {
        } break;

        case PlayerAction_Dig:
        {
        } break;
      }
#endif

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

  /* GameState->Models = AllocateGameModels(GameState, Resources->Memory, Heap); */

#if 1
  /* u32 PlayerModelIndex = RandomBetween( u32(ModelIndex_FirstPlayerModel), &GameState->Entropy, u32(ModelIndex_LastPlayerModel+1)); */
  /* u32 PlayerModelIndex = ModelIndex_FirstPlayerModel; */
  u32 PlayerModelIndex = ModelIndex_Player_old;
  GameState->Player = GetFreeEntity(EntityTable);
  GameState->Player->UserType = Cast(u32, EntityType_Player);

  asset_id PlayerAsset = AssetId({FileTraversalType_File, CSz("models"), CSz("players/chr_rain.vox")});
  SpawnPlayerLikeEntity(Plat, World, &PlayerAsset, GameState->Player, PlayerSpawnP, &GameState->Entropy);
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

    /* u32 EnemyModelIndex = u32(ModelIndex_Enemy_Skeleton_Axe) + EnemyIndex; */
    /* u32 EnemyModelIndex = RandomBetween( u32(ModelIndex_Enemy_Skeleton_Axe), &GameState->Entropy, u32(ModelIndex_Enemy_Skeleton_King+1)); */
    /* Assert(EnemyModelIndex >= ModelIndex_FirstEnemyModel); */
    /* Assert(EnemyModelIndex <= ModelIndex_LastEnemyModel); */
    asset_id EnemyAsset = AssetId({FileTraversalType_File, CSz("models"), CSz("skele_base.vox")});

    auto EnemySpawnP = Canonical_Position(V3(0), WorldCenter + WP );
    auto Enemy = GetFreeEntity(EntityTable);
    /* Enemy->UserData = (void*)GameEntityBehaviorFlags_Enemy; */
    SpawnPlayerLikeEntity(Plat, World, &EnemyAsset, Enemy, EnemySpawnP, &GameState->Entropy, 0.35f);
  }
#endif

  WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

  return GameState;
}

BONSAI_API_ON_LIBRARY_RELOAD()
{
  Info("Game ON_LIBRARY_LOAD");

  UNPACK_ENGINE_RESOURCES(Resources);

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
