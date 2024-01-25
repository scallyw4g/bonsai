#define BONSAI_DEBUG_SYSTEM_API 1
#define DO_EDITOR_UI_FOR_ENTITY_TYPE 1

#include <bonsai_types.h>

#include "game_constants.h"
#include "game_types.h"

global_variable v3
Global_EntityFireballOffset = V3(0,0,18);

global_variable f32
Global_MeleeRange = 12.f;

global_variable f32
Global_PlayerDigDepth = 3.f;


struct entity_game_data
{
  u32 FireballChargeLevel;
  u32 FireballCharges;

  u32 IceBlockCharges;
  u32 HoldingItem;
};

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

  cp EnemyBaseP  = GetEntityBaseP(World, Enemy);
  v3 EnemyBaseSimP  = GetSimSpaceP(World, EnemyBaseP);
  {
    v3 EnemyCenterSimP = GetSimSpaceP(World, GetEntityCenterP(World, Enemy));
    v3 EnemyBaseToCenter = EnemyCenterSimP - EnemyBaseSimP;

    // NOTE(Jesse): This transform doesn't work for scaled entities.
    Assert(Enemy->Scale == 1.f);
    m4 Transform = GetTransformMatrix({}, V3(Enemy->Scale), FromEuler(Enemy->EulerAngles));

    v3 Light0Offset = (Transform * (V4(1.5f,  1.5f, 4.5f, 1.f))).xyz;
    v3 Light1Offset = (Transform * (V4(-1.5f, 1.5f, 4.5f, 1.f))).xyz;

    v3 LightColor = V3(0.05f, 0.0001f, 0.0001f);
    /* v3 LightColor = V3(0.035f, 0.001f, 0.001f); */
    /* v3 LightColor = V3(0.035f, 0.f, 0.02f); */
    /* v3 LightColor = V3(0.045f, 0.0005f, 0.0025f); */

    v3 Light0Sim = EnemyCenterSimP + Light0Offset;
    v3 Light1Sim = EnemyCenterSimP + Light1Offset;

    /* DEBUG_HighlightVoxel(Engine, Light0Sim, RED); */
    /* DEBUG_HighlightVoxel(Engine, Light1Sim, RED); */

    cp Light0CP = SimSpaceToCanonical(Engine->World, Light0Sim);
    cp Light1CP = SimSpaceToCanonical(Engine->World, Light1Sim);

    DoLight(&Lighting->Lights, GetRenderP(Engine, Light0CP), LightColor);
    DoLight(&Lighting->Lights, GetRenderP(Engine, Light1CP), LightColor);
  }


  entity *Player = GetEntity(EntityTable, GameState->PlayerId);
  if (Player == 0) { return; }

  cp PlayerBaseP = GetEntityBaseP(World, Player);
  v3 PlayerBaseSimP = GetSimSpaceP(World, PlayerBaseP);



  // NOTE(Jesse): Entities embedded in the world cannot act
  if (GetCollision(World, Enemy).Count > Global_EntityCanMoveThroughCollisionThresh) { return; }

  u16 StandingSpotColor = YELLOW;
  r32 StandingSpotRadius = DEFAULT_STANDING_SPOT_THICKNESS;

  if (Enemy == Engine->HoverEntity.Value)
  {
    StandingSpotColor = ORANGE;
    StandingSpotRadius *= 2.f;
  }

  f32 EnemySightDistance = 60.f;
  standing_spot_buffer SightedSpots = GetStandingSpotsWithinRadius_FilteredByStandable(World, EnemyBaseP, EnemySightDistance, Enemy->_CollisionVolumeRadius, GetTranArena());

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

  memory_arena *Temp = GetTranArena();
  temp_memory_handle TMH = BeginTemporaryMemory(Temp);

  f32 EnemyMovespeed = 10.f;
  standing_spot_buffer MoveSpots = GetStandingSpotsWithinRadius(World, &SightedSpots, EnemyBaseP, EnemyMovespeed, Temp, Temp);

  standing_spot *NextSpot =  {};

  if (LengthSq(PlayerBaseSimP-EnemyBaseSimP) < Square(EnemySightDistance))
  {
    maybe_standing_spot MaybeSpot = GetClosestToP(World, &MoveSpots, PlayerBaseP);
    NextSpot = &MaybeSpot.Value;
  }
  else
  {
    random_series Entropy = {9490653468579 + u64(GameState->TurnIndex) + Enemy->Id.Index};
    if (MoveSpots.Count)
    {
      u32 NextSpotIndex = RandomBetween(0u, &Entropy, u32(MoveSpots.Count));
      Assert(NextSpotIndex < MoveSpots.Count);
      NextSpot = MoveSpots.Start + NextSpotIndex;
    }
  }


  if (NextSpot)
  {
    DrawStandingSpot(&GpuMap->Buffer, Camera, NextSpot, RED, StandingSpotRadius*2.f);

    if (GameState->PlayerActed)
    {
      v3 SpotSimP = GetSimSpaceBaseP(World, NextSpot);
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

  Entity->AssetId = GetOrAllocateAssetId(Engine, &AssetName);
  Entity->EulerAngles = V3(0.f, 5.32f, RandomBilateral(&Global_GameEntropy));

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

  u32 MaxBitties = ArrayCount(AssetNames) * 2;
  RangeIterator_t(u32, BittyIndex, MaxBitties)
  {
  // TODO(Jesse)(leak): This leaks the asset name when the asset is freed
    entity *BittyEntity = TryGetFreeEntityPtr(EntityTable);
    file_traversal_node AssetName = {FileTraversalType_File, CSz("models"), AssetNames[BittyIndex%ArrayCount(AssetNames)]};
    BittyEntity->AssetId = GetOrAllocateAssetId(Engine, &AssetName);

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


  file_traversal_node SkullAssetNames[] =
  {
    {FileTraversalType_File, CSz("models"), CSz("skull.vox")},
    {FileTraversalType_File, CSz("models"), CSz("skull_broken.vox")},
  };

  /* s32 AssetIndex = RandomBetween(0, &Global_GameEntropy, ArrayCount(SkullAssetNames)); */
  s32 AssetIndex = 0;

  Entity->AssetId = GetOrAllocateAssetId(Engine, &SkullAssetNames[AssetIndex]);
  Entity->Physics.Velocity.z += 9.5f;
  Entity->P.Offset.z += 10.f;
  Entity->UserType = EntityType_Loot;

  if (AssetIndex == 1)
  {
    Entity->EulerAngles = V3(0.f, 5.32f, RandomBilateral(&Global_GameEntropy));
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
EffectGrabEntity(engine_resources *Engine, entity *Grabber, entity *Grabee)
{
  b32 Result = False;
  switch (entity_type(Grabee->UserType))
  {
    case EntityType_Default:
    case EntityType_Player:
    case EntityType_Fireball:
    {
    } break;

    case EntityType_Enemy:
    {
      Result = True;
      DestroySkeleton(Engine, Grabee, 5.f);
    } break;

    case EntityType_Loot:
    {
      Result = True;
      Grabber->Carrying = Grabee->Id;
      Grabee->Behavior = entity_behavior_flags(Grabee->Behavior & ~EntityBehaviorFlags_Gravity);
    } break;
  }
  return Result;
}

link_internal b32
EffectSmackEntity(engine_resources *Engine, entity *Enemy)
{
  b32 Result = False;

  switch (entity_type(Enemy->UserType))
  {
    case EntityType_Default:
    case EntityType_Player:
    case EntityType_Fireball:
    {
    } break;

    case EntityType_Enemy:
    {
      Result = True;
      DestroySkeleton(Engine, Enemy, 5.f);
    } break;

    case EntityType_Loot:
    {
      Result = True;
      DestroyLoot(Engine, Enemy);
    } break;
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

  if (Entity->Carrying)
  {
    entity *Carrying = GetEntity(EntityTable, Entity->Carrying);
    v3 EntitySimP = GetSimSpaceBaseP(World, Entity);

    r32 AnimOffset = Sin(fmodf(Plat->GameTime*2.f, PI32)) + 1.f;
    v3 NewP = EntitySimP + V3(0.f, 0.f, Entity->_CollisionVolumeRadius.z*2.f + AnimOffset) - V3(Carrying->_CollisionVolumeRadius.xy, 0.f);
    Carrying->P = SimSpaceToCanonical(World,  NewP);
  }

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
        s32 Frequency = 0; // Ignored
        s32 Amplititude = 0; // Ignored
        s32 StartingZDepth = -100;
        u32 OctaveCount = 1;

        octave_buffer OctaveBuf = { OctaveCount, {} };
        OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

        OctaveBuf.Octaves[0] = {V3(400, 400, 200), 150, V3(1)};

        chunk_init_flags InitFlags = chunk_init_flags(ChunkInitFlag_ComputeStandingSpots|ChunkInitFlag_GenLODs);
        InitializeChunkWithNoise( GrassyTerracedTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
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


link_internal b32
HoldingItem(entity *Player)
{
  b32 Result = Player->Carrying;
  return Result;
}

link_internal b32
IceBlockCharges(entity *Player)
{
  entity_game_data *GameData = Cast(entity_game_data*, Player->UserData);
  b32 Result = GameData->IceBlockCharges != 0;
  return Result;
}

link_internal u32
GetColorForAction(player_action Action)
{
  u32 Result = 0;

  switch (Action)
  {
    InvalidCase(PlayerAction_Count);

    case PlayerAction_None: { } break;

    case PlayerAction_Move:
    {
      Result = GREEN;
    } break;

    case PlayerAction_ChargeFireball:
    {
      Result = GREEN;
    } break;

    case PlayerAction_Throw:
    {
      Result = ORANGE;
    } break;

    case PlayerAction_IceBlock:
    {
      Result = ICE_BLUE;
    } break;

    case PlayerAction_Shovel:
    {
      Result = LIGHT_DIRT;
    } break;

    case PlayerAction_Grab:
    {
      Result = GREEN;
    } break;

  }

  return Result;
}
link_internal b32
CanDoAction(engine_resources *Engine, entity *Player, player_action Action, u32_buffer *MeleeEntities)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  b32 Result = False;
  switch (Action)
  {
    InvalidCase(PlayerAction_Count);

    case PlayerAction_None:
    case PlayerAction_Move:
    {
      Result = True;
    } break;

    case PlayerAction_ChargeFireball:
    {
      if (HoldingItem(Player) == False) { Result = True; }
    } break;

    case PlayerAction_Throw:
    {
      if (HoldingItem(Player)) { Result = True; }
    } break;

    case PlayerAction_IceBlock:
    {
      if (IceBlockCharges(Player)) { Result = True; }
    } break;

    case PlayerAction_Shovel:
    {
      Result = True;
    } break;

    case PlayerAction_Grab:
    {
      RangeIterator_t(umm, EntityIndexIndex, MeleeEntities->Count)
      {
        entity *E = EntityTable[MeleeEntities->Start[EntityIndexIndex]];
        if (E->UserType == EntityType_Loot && E->Id != Player->Carrying)
        {
          Result = True;
        }
      }
    } break;

  }

  return Result;
}

link_internal b32
EntityIsWithinMeleeRangeOf(world *World, entity *E0, entity *E1)
{
  b32 Result = False;
  if (E0 && E1)
  {
    aabb E1HitArea = GetSimSpaceAABB(World, E1);
    E1HitArea.Min -= V3(Global_MeleeRange);
    E1HitArea.Max += V3(Global_MeleeRange);

    if (Intersect(World, E0, &E1HitArea))
    {
      Result = True;
    }
  }
  return Result;
}

link_internal b32
IsWithinRangeForAction(world *World, entity *Player, cp ActionLocation, player_action Action)
{
  b32 Result = False;

  switch (Action)
  {
    InvalidCase(PlayerAction_Count);

    case PlayerAction_None:
    case PlayerAction_Move:
    case PlayerAction_ChargeFireball:
    case PlayerAction_Throw:
    case PlayerAction_Grab:
    {
      NotImplemented;
    } break;


    case PlayerAction_IceBlock:
    {
      Result = True;
    } break;

    case PlayerAction_Shovel:
    {
      v3 SimPlayerP = GetSimSpaceBaseP(World, Player);
      v3 SimActionP = GetSimSpaceP(World, ActionLocation);
      if (DistanceSq(SimPlayerP, SimActionP) <= Square(Global_MeleeRange))
      {
        Result = True;
      }
    } break;
  }

  return Result;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  memory_arena *GameMemory = &GameState->Memory;
  entity *Player           = GetEntity(EntityTable, GameState->PlayerId);
  if (Player == 0) { return; }

  // NOTE(Jesse): Crutch for loading savefiles that didn't have this
  if (Player->UserData == 0)
  {
    Player->UserData = Cast(u64, Allocate(entity_game_data, GameMemory, 1));
  }

  if (GameState->PlayerActed)
  {
    GameState->TurnIndex++;
  }

  GameState->PlayerActed = False;

  v3 SimPlayerCenter = GetSimSpaceCenterP(World, Player);
  sphere S = {SimPlayerCenter, Global_MeleeRange};
  u32_buffer MeleeEntities = GatherEntitiesIntersecting(World, EntityTable, &S, GetTranArena());

  if (Input->Z.Clicked)
  {
    if (CanDoAction(Resources, Player, PlayerAction_Move, &MeleeEntities))
    {
      GameState->ProposedAction = PlayerAction_Move;
    }
    else
    {
      GameState->ProposedAction = PlayerAction_None;
    }
  }

  if (Input->X.Clicked)
  {
    if (CanDoAction(Resources, Player, PlayerAction_Shovel, &MeleeEntities))
    {
      GameState->ProposedAction = PlayerAction_Shovel;
    }
    else
    {
      GameState->ProposedAction = PlayerAction_None;
    }
  }

  if (Input->C.Clicked)
  {
    if (CanDoAction(Resources, Player, PlayerAction_Grab, &MeleeEntities))
    {
      GameState->ProposedAction = PlayerAction_Grab;
    }
    else
    {
      GameState->ProposedAction = PlayerAction_None;
    }
  }

  if (Input->V.Clicked)
  {
    if (CanDoAction(Resources, Player, PlayerAction_ChargeFireball, &MeleeEntities))
    {
      GameState->ProposedAction = PlayerAction_ChargeFireball;
    }
    else
    {
      GameState->ProposedAction = PlayerAction_None;
    }
  }

  if (Input->B.Clicked)
  {
    if (CanDoAction(Resources, Player, PlayerAction_IceBlock, &MeleeEntities))
    {
      GameState->ProposedAction = PlayerAction_IceBlock;
    }
    else
    {
      GameState->ProposedAction = PlayerAction_None;
    }
  }

  if (Input->N.Clicked)
  {
    if (CanDoAction(Resources, Player, PlayerAction_Throw, &MeleeEntities))
    {
      GameState->ProposedAction = PlayerAction_Throw;
    }
    else
    {
      GameState->ProposedAction = PlayerAction_None;
    }
  }


  if (Input->RMB.Clicked) { GameState->ProposedAction = PlayerAction_None; }

  if (Resources->MousedOverVoxel.Tag)
  {
    picked_voxel Pick = Resources->MousedOverVoxel.Value;
    v3 VoxelP = GetAbsoluteP(&Pick);

    world_chunk *ClosestChunk = Pick.Chunks[PickedVoxel_LastEmpty].Chunk;
    canonical_position LastEmptyMouseVoxel = Pick.Picks[PickedVoxel_LastEmpty];
    canonical_position FirstFilledMouseVoxel = Pick.Picks[PickedVoxel_FirstFilled];



    b32 WorldEditMode = (Editor->EngineDebugViewModeToggleBits & (1<<EngineDebugViewMode_WorldEdit));
    if ( GameState->TurnMode == TurnMode_Default &&
         WorldEditMode == 0 )
    {
      entity_game_data *PlayerGameData = Cast(entity_game_data*, Player->UserData);
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

        case PlayerAction_ChargeFireball:
        {
          if (Input->LMB.Clicked)
          {
            GameState->PlayerActed = True;

            PlayerGameData->HoldingItem = 1;
            PlayerGameData->FireballChargeLevel += 2;

            entity *E = TryGetFreeEntityPtr(EntityTable); Assert(E);
            /* E->UserType = EntityType_Fireball; */

            Player->Carrying = E->Id;

            SpawnFire( E,
                      &GameState->Entropy,
                       Global_EntityFireballOffset + Player->_CollisionVolumeRadius.xy,
                       PlayerGameData->FireballChargeLevel);


            auto CP = Player->P;
            CP.Offset += Player->Emitter->SpawnRegion.Min;
            Canonicalize(World, &CP);

            E->Emitter->SpawnRegion.Max -= E->Emitter->SpawnRegion.Min;
            E->Emitter->SpawnRegion.Min = {};

            physics Physics = FireballPhysics();
            SpawnEntity(
              E,
              0,
              0,
              EntityBehaviorFlags_None,
              &Physics,
              &CP,
              V3(1)
            );

            /* fireball_state *FireballState = Allocate(fireball_state, &GameState->Heap, 1); */
            fireball_state *FireballState = (fireball_state*)HeapAllocate(&GameState->Heap, sizeof(fireball_state));

            FireballState->ChargeLevel = 2;
            FireballState->TargetP = FirstFilledMouseVoxel;

            v3 EntityP = GetSimSpaceP(World, E);
            v3 TargetP = GetSimSpaceP(World, FireballState->TargetP);

            /* E->Physics.Velocity = (Normalize(TargetP-EntityP) + V3(0,0,3)) * 20.f; */
            /* E->Physics.Velocity = V3(10.f); */

            E->UserData = u64(FireballState);
          }
        } break;

        case PlayerAction_Throw:
        {
          if (PlayerGameData->FireballChargeLevel)
          {
            if (Input->LMB.Clicked)
            {
              GameState->PlayerActed = True;
            }
          }
        } break;

        case PlayerAction_IceBlock:
        case PlayerAction_Shovel:
        {
          if (Resources->MousedOverVoxel.Tag)
          {
            cp ActionMinP        = Canonical_Position(&Resources->MousedOverVoxel.Value);
               ActionMinP.Offset = RoundToMultiple(ActionMinP.Offset, V3i(8, 8, s32(Global_PlayerDigDepth)));
            Canonicalize(World, &ActionMinP);

            cp ActionCenterP = Canonicalize(World, ActionMinP + Global_StandingSpotHalfDim);

            if (IsWithinRangeForAction(World, Player, ActionCenterP, GameState->ProposedAction))
            {
              u32 Color = GetColorForAction(GameState->ProposedAction);
              DrawStandingSpot(&GpuMap->Buffer, Camera, ActionMinP, Color, 0.5f);

              if (Input->LMB.Clicked)
              {
                GameState->PlayerActed = True;
                switch (GameState->ProposedAction)
                {
                  case PlayerAction_IceBlock:
                  {
                    DoIceBlock(Resources, ActionCenterP, 4.f, GetTranArena());
                  } break;

                  case PlayerAction_Shovel:
                  {
                    entity *HoverEntity = Resources->HoverEntity.Value;
                    if (EntityIsWithinMeleeRangeOf(World, HoverEntity, Player))
                    {
                      GameState->PlayerActed = EffectSmackEntity(Resources, HoverEntity);
                    }
                    else
                    {
                      DoDig(Resources, ActionCenterP, 5.f, Global_PlayerDigDepth, GetTranArena());
                    }
                  } break;

                  InvalidDefaultCase;
                }
              }
            }
          }
        } break;

        case PlayerAction_Grab:
        {
          entity *HoverEnemy = Resources->HoverEntity.Value;
          if (EntityIsWithinMeleeRangeOf(World, HoverEnemy, Player))
          {
            HighlightEntity(Resources, HoverEnemy);
            if (Input->LMB.Clicked)
            {
              GameState->PlayerActed = EffectGrabEntity(Resources, Player, HoverEnemy);
            }
          }
        }
      }
    }

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
      GameState->TransitionDuration += Plat->dt;
      GameState->ProposedAction = PlayerAction_None;
    }
  }

  local_persist window_layout ActionsWindow = WindowLayout("ActionsWindow");
  PushBorderlessWindowStart(Ui, &ActionsWindow);

  v2 WindowDim = GetDim(&ActionsWindow);
  v2 WindowOffset = V2(Plat->WindowWidth/2.f-(WindowDim.x/2.f), Plat->WindowHeight-WindowDim.y-25.f);
  /* v2 WindowOffset = V2(0.f, Plat->WindowHeight-WindowDim.y); */
  /* v2 WindowOffset = {}; */

  ActionsWindow.Basis = WindowOffset;

/*   RangeIterator_t(umm, EntityIndexIndex, MeleeEntities.Count) */
/*   { */
/*     entity *E = EntityTable[MeleeEntities.Start[EntityIndexIndex]]; */
/*     HighlightEntity(Resources, E); */
/*   } */

  {
    ui_element_reference ActionTable = PushTableStart(Ui);
      for (u32 ActionIndex = PlayerAction_Move; ActionIndex < PlayerAction_Count; ++ActionIndex)
      {
        player_action Action = player_action(ActionIndex);
        ui_style *Style = ActionIndex == GameState->ProposedAction ? &DefaultSelectedStyle : &DefaultStyle;

        b32 PlayerCanDoThisAction = CanDoAction(Resources, Player, player_action(ActionIndex), &MeleeEntities);
        if (PlayerCanDoThisAction == False)
        {
          Style = &DefaultDisabledStyle;
        }

        u32 Start = StartColumn(Ui);


          ui_id ButtonId = UiId(&ActionsWindow, "player_action", ActionIndex);
          interactable_handle ButtonHandle = {ButtonId};

          v3 Tint =  V3(0.5f);

          if (PlayerCanDoThisAction)
          {
            Tint = V3(1.f);
            if (Hover(Ui, &ButtonHandle))
            {
              Tint = V3(1.1f);
            }

            if (Clicked(Ui, &ButtonHandle))
            {
              Tint = V3(1.25f);
              GameState->ProposedAction = Action;
            }
          }

          v3 BackgroundTint = Tint;

          if (GameState->ProposedAction == ActionIndex)
          {
            BackgroundTint.g *= 1.5f;
          }

          v2 SpriteSize = V2(96);

          PushButtonStart(Ui, ButtonId);
            PushTexturedQuadColumn(Ui, Resources->Ui.SpriteTextureArray, 0, SpriteSize, zDepth_TitleBar, BackgroundTint, QuadRenderParam_NoAdvance);
            PushTexturedQuadColumn(Ui, Resources->Ui.SpriteTextureArray, Global_SpriteIndexFromActionIndex[ActionIndex], SpriteSize, zDepth_Text, Tint);
          PushButtonEnd(Ui);

          PushForceAdvance(Ui, V2(16, 0));


        EndColumn(Ui, Start);
      }
    PushTableEnd(Ui);

    /* PushTableStart(Ui, Position_RightOf, ActionTable); */
    /*   PushColumn(Ui, ToString(GameState->TurnMode)); */
    /* PushTableEnd(Ui); */
  }

  PushWindowEnd(Ui, &ActionsWindow);
}

poof(serdes_struct(entity_game_data))
#include <generated/serdes_struct_entity_game_data.h>

link_weak b32
EntityUserDataSerialize(native_file *File, u64 UserType, u64 UserData)
{
  entity_game_data *EGD = Cast(entity_game_data*, UserData);
  if (EGD)
  {
    u64 T = True;
    Serialize(File, &T);
    Serialize(File, EGD);
  }
  else
  {
    u64 F = False;
    Serialize(File, &F);
  }
  return True;
}

link_weak b32
EntityUserDataDeserialize(u8_cursor *Bytes, u64 *UserType, u64 *UserData, memory_arena *Memory)
{
  Deserialize(Bytes, UserData, Memory);

  if(*UserData == True)
  {
    entity_game_data *GameData = Allocate(entity_game_data, Memory, 1);
    Deserialize(Bytes, GameData, Memory);
    *UserData = (u64)GameData;
  }
  else
  {
    if (*UserData != False)
    {
      SoftError("Corrupt entity, UserData was (%x), when it should be (1) or (0)!", *UserData);
      return False;
    }
  }

  b32 Result = True;
  /* MAYBE_READ_DEBUG_OBJECT_DELIM(); */
  return Result;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  GameState->Heap = InitHeap(Megabytes(4));

  Global_AssetPrefixPath = CSz("examples/turn_based/assets");

  world_position WorldCenter = World_Position(5, -4, 0);
  canonical_position PlayerSpawnP = Canonical_Position(Voxel_Position(0), WorldCenter + World_Position(0,0,1));

  StandardCamera(&Graphics->GameCamera, 10000.0f, 500.0f, PlayerSpawnP);

  GameState->Entropy = {DEBUG_NOISE_SEED};

  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  u32 PlayerModelIndex = ModelIndex_Player_old;
  GameState->PlayerId = GetFreeEntity(EntityTable);

  entity *Player = GetEntity(EntityTable, GameState->PlayerId);
  Player->UserType = Cast(u32, EntityType_Player);

  asset_id PlayerAsset = GetOrAllocateAssetId(Resources, {FileTraversalType_File, CSz("models"), CSz("players/chr_rain.vox")});
  SpawnPlayerLikeEntity(Plat, World, &PlayerAsset, 0, Player, PlayerSpawnP, &GameState->Entropy);

  Player->UserData = (u64)Allocate(entity_game_data, Resources->Memory, 1);
  Graphics->GameCamera.GhostId = Player->Id;

  u32 EnemyCount = 3;
  v3i HalfVisibleRegion = g_VisibleRegion / 2;
  HalfVisibleRegion.z = 0;
  for (u32 EnemyIndex = 0; EnemyIndex < EnemyCount; ++EnemyIndex)
  {
    world_position WP = World_Position(
        RandomBetween(0, &GameState->Entropy, HalfVisibleRegion.x),
        RandomBetween(0, &GameState->Entropy, HalfVisibleRegion.y),
        1);

    asset_id EnemyAsset = GetOrAllocateAssetId(Resources, {FileTraversalType_File, CSz("models"), CSz("skele_base.vox")});

    cp EnemySpawnP = Canonical_Position(V3(0), WorldCenter + WP );
    entity *Enemy = TryGetFreeEntityPtr(EntityTable);
    Assert(Enemy);

    SpawnPlayerLikeEntity(Plat, World, &EnemyAsset, 0, Enemy, EnemySpawnP, &GameState->Entropy, 1.f);
  }

  WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

  return GameState;
}

BONSAI_API_ON_LIBRARY_RELOAD()
{
  Info("Game ON_LIBRARY_LOAD");

  UNPACK_ENGINE_RESOURCES(Resources);

  GameState->PlayerId = {};
  RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
  {
    entity *E = EntityTable[EntityIndex];
    if (E->UserType == EntityType_Player)
    {
      if (GameState->PlayerId.Generation) { Warn("Multiple Player entities detected!"); }
      GameState->PlayerId = E->Id;
    }
  }
}
