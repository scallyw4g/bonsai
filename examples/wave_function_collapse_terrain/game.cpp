#define PLATFORM_GL_IMPLEMENTATIONS 1
#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

global_variable v3
Global_EntityFireballOffset = V3(7.0f, -.75f, 4.5f);

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  // TODO(Jesse)(leak, memory): Pass temp memory to LoadVoxModel

  model *Result                                       = Allocate(model, Memory, ModelIndex_Count);
  /* Result[ModelIndex_Enemy]                         = LoadVoxModel(Memory, Heap, "models/chr_tama.vox", Memory); */

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

  /* Result[ModelIndex_Player_jp]      = LoadVoxModel(Memory, Heap, "models/players/chr_jp.vox", Memory); */
  /* Result[ModelIndex_Player_bow]     = LoadVoxModel(Memory, Heap, "models/players/chr_bow.vox", Memory); */
  /* Result[ModelIndex_Player_cat]     = LoadVoxModel(Memory, Heap, "models/players/chr_cat.vox", Memory); */
  /* Result[ModelIndex_Player_fox]     = LoadVoxModel(Memory, Heap, "models/players/chr_fox.vox", Memory); */
  /* Result[ModelIndex_Player_gumi]    = LoadVoxModel(Memory, Heap, "models/players/chr_gumi.vox", Memory); */
  /* Result[ModelIndex_Player_knight]  = LoadVoxModel(Memory, Heap, "models/players/chr_knight.vox", Memory); */
  /* Result[ModelIndex_Player_man]     = LoadVoxModel(Memory, Heap, "models/players/chr_man.vox", Memory); */
  /* Result[ModelIndex_Player_mom]     = LoadVoxModel(Memory, Heap, "models/players/chr_mom.vox", Memory); */
  /* Result[ModelIndex_Player_old]     = LoadVoxModel(Memory, Heap, "models/players/chr_old.vox", Memory); */
  /* Result[ModelIndex_Player_poem]    = LoadVoxModel(Memory, Heap, "models/players/chr_poem.vox", Memory); */
  /* Result[ModelIndex_Player_rain]    = LoadVoxModel(Memory, Heap, "models/players/chr_rain.vox", Memory); */
  /* Result[ModelIndex_Player_sasami]  = LoadVoxModel(Memory, Heap, "models/players/chr_sasami.vox", Memory); */
  /* Result[ModelIndex_Player_sol]     = LoadVoxModel(Memory, Heap, "models/players/chr_sol.vox", Memory); */
  /* Result[ModelIndex_Player_sword]   = LoadVoxModel(Memory, Heap, "models/players/chr_sword.vox", Memory); */
  /* Result[ModelIndex_Player_tale]    = LoadVoxModel(Memory, Heap, "models/players/chr_tale.vox", Memory); */
  /* Result[ModelIndex_Player_tama]    = LoadVoxModel(Memory, Heap, "models/players/chr_tama.vox", Memory); */
  /* Result[ModelIndex_Player_tsurugi] = LoadVoxModel(Memory, Heap, "models/players/chr_tsurugi.vox", Memory); */

  Result[ModelIndex_Bitty0] = LoadVoxModel(Memory, Heap, "models/splotion_bitty_0.vox", Memory);
  Result[ModelIndex_Bitty1] = LoadVoxModel(Memory, Heap, "models/splotion_bitty_1.vox", Memory);

  return Result;
}

u8
GetColorForTile(u32 TileType)
{
  switch (TileType)
  {
    case TileOption_None:
      return RED;

#if 0
    case TileOption_HouseBase_North:
      return GREEN;

    case TileOption_HouseBase_South:
      return BLUE;

    case TileOption_HouseBase_East:
    case TileOption_HouseBase_West:
      return GREY_6;

    case TileOption_HouseBase_Interior:
      return YELLOW;
#endif

    case TileOption_Stone:
      return GREY_2;

    case TileOption_Dirt:
      return GRASS_GREEN;
  }

  return BLACK;
}

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  world *World = Thread->EngineResources->World;

  work_queue_entry_type Type = Entry->Type;
  switch (Type)
  {
    InvalidCase(type_work_queue_entry_noop);

    case type_work_queue_entry_init_asset:
    {
      InvalidCodePath();
    } break;

    case type_work_queue_entry_update_world_region:
    {
      work_queue_entry_update_world_region *Job = SafeAccess(work_queue_entry_update_world_region, Entry);

      picked_voxel Location = Job->Location;
      r32 Radius = Job->Radius;

      DoWorldUpdate(&Thread->EngineResources->Plat->LowPriority, World, Job->ChunkBuffer, Job->ChunkCount, &Location, Job->MinP, Job->MaxP, Job->Op, Job->ColorIndex, Radius, Thread);
    } break;

    case type_work_queue_entry_sim_particle_system:
    {
      work_queue_entry_sim_particle_system *Job = SafeAccess(work_queue_entry_sim_particle_system, Entry);
      SimulateParticleSystem(Job);
    } break;

    case type_work_queue_entry_rebuild_mesh:
    {
      work_queue_entry_rebuild_mesh *Job = SafeAccess(work_queue_entry_rebuild_mesh, Entry);
      world_chunk *Chunk = Job->Chunk;
      Assert( IsSet(Chunk->Flags, Chunk_VoxelsInitialized) );

      untextured_3d_geometry_buffer *NewMesh = 0;

      /* if (Chunk->FilledCount > 0) */
      {
        untextured_3d_geometry_buffer *GeneratedMesh = GetMeshForChunk(&Thread->EngineResources->MeshFreelist, Thread->PermMemory);
        /* MarkBoundaryVoxels( CopiedVoxels, QueryDim, {{1,1,1}}, QueryDim-2); */
        /* MarkBoundaryVoxels( Chunk->Voxels, Chunk->Dim, {}, Chunk->Dim, GeneratedMesh ); */
        BuildWorldChunkMeshFromMarkedVoxels_Greedy( Chunk->Voxels, Chunk->Dim, {}, Chunk->Dim, GeneratedMesh, Thread->TempMemory);
        /* BuildWorldChunkMesh(Chunk->Voxels, Chunk->Dim, {}, Chunk->Dim, GeneratedMesh); */

        /* DrawDebugVoxels( Chunk->Voxels, Chunk->Dim, {}, Chunk->Dim, GeneratedMesh ); */

        if (GeneratedMesh->At)
        {
          NewMesh = GeneratedMesh;
        }
        else
        {
          DeallocateMesh(&GeneratedMesh, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
        }
      }

      umm Timestamp = NewMesh ? NewMesh->Timestamp : __rdtsc();
      /* TakeOwnershipSync(&Chunk->Meshes, MeshBit_Main); */
      auto Replaced = AtomicReplaceMesh(&Chunk->Meshes, MeshBit_Main, NewMesh, Timestamp);
      if (Replaced) { DeallocateMesh(&Replaced, &Thread->EngineResources->MeshFreelist, Thread->PermMemory); }
      /* ReleaseOwnership(&Chunk->Meshes, MeshBit_Main, NewMesh); */

#if 0
      {
        if (Chunk->FilledCount > 0)
        {
          world *World = Thread->EngineResources->World;
          untextured_3d_geometry_buffer *LodMesh = GetMeshForChunk(&Thread->EngineResources->MeshFreelist, Thread->PermMemory);
          ComputeLodMesh( Thread, Chunk, World->ChunkDim, Chunk, World->ChunkDim, LodMesh, False);

          if (LodMesh->At)
          {
            if ( Chunk->SelectedMeshes & MeshBit_Lod )
            {
              untextured_3d_geometry_buffer *OldMesh = (untextured_3d_geometry_buffer*)TakeOwnershipSync((volatile void**)&Chunk->LodMesh);
              DeallocateMesh(&OldMesh, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
            }

            Assert(Chunk->LodMesh == 0);
            Replace((volatile void**)&Chunk->LodMesh, (void*)LodMesh);
            Chunk->SelectedMeshes |= MeshBit_Lod;
          }
        }
        else
        {
          Chunk->SelectedMeshes &= (~(u32)MeshBit_Lod);
        }
      }
#endif

      FinalizeChunkInitialization(Chunk);
    } break;

    case type_work_queue_entry_init_world_chunk:
    {
      work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;


      if (ChunkIsGarbage(Chunk))
      {
      }
      else
      {
        if ( NotSet(Chunk->Flags, Chunk_VoxelsInitialized) )
        {
          // TODO(Jesse): Deduplicate this
          v3i TileDim = V3i(8);

          v3i AbsTileMaxDim = World->VisibleRegion*World->ChunkDim / TileDim;
          Assert((World->VisibleRegion*World->ChunkDim%TileDim) == V3i(0));

          v3i TilesPerWorldChunk = World->ChunkDim / TileDim;

          v3i TileMin = (Chunk->WorldP) *TilesPerWorldChunk;
          v3i TileMax = TileMin + TilesPerWorldChunk;

          for (s32 zTile = TileMin.z; zTile < TileMax.z; ++zTile)
          {
            for (s32 yTile = TileMin.y; yTile < TileMax.y; ++yTile)
            {
              for (s32 xTile = TileMin.x; xTile < TileMax.x; ++xTile)
              {
                s32 TileIndex = GetIndex(xTile, yTile, zTile, AbsTileMaxDim);
                Assert(TileIndex < TileSuperpositionCount);
                u32 TileOptions = TileSuperpositions[TileIndex];

                switch (TileOptions)
                {
                  /* case TileOption_None: */
                  case TileOption_Air:
                  {
                  } break;

                  case TileOption_None:
#if 0
                  case TileOption_HouseBase_North:
                  case TileOption_HouseBase_South:
                  case TileOption_HouseBase_East:
                  case TileOption_HouseBase_West:
                  case TileOption_HouseBase_Interior:
#endif
                  case TileOption_Dirt:
                  case TileOption_Stone:
                  {
                    u8 Color = GetColorForTile(TileOptions);
                    v3i VoxMin = (V3i(xTile, yTile, zTile)*TileDim) % World->ChunkDim;
                    v3i VoxMax = VoxMin + TileDim;
                    for (s32 zVox = VoxMin.z; zVox < VoxMax.z; ++zVox)
                    {
                      for (s32 yVox = VoxMin.y; yVox < VoxMax.y; ++yVox)
                      {
                        for (s32 xVox = VoxMin.x; xVox < VoxMax.x; ++xVox)
                        {
                          s32 VoxIndex = GetIndex(xVox, yVox, zVox, World->ChunkDim);
                          Chunk->Voxels[VoxIndex].Flags = Voxel_Filled;
                          Chunk->Voxels[VoxIndex].Color = Color;
                          ++Chunk->FilledCount;
                        }
                      }
                    }
                  } break;

                }

              }
            }
          }
          MarkBoundaryVoxels_MakeExteriorFaces(Chunk->Voxels, World->ChunkDim, {}, World->ChunkDim);

          ComputeStandingSpots( World->ChunkDim, Chunk, {}, {}, Global_StandingSpotDim,
                                World->ChunkDim, 0, &Chunk->StandingSpots, Thread->TempMemory);


          if ( Chunk->FilledCount > 0)
          {
            auto PrimaryMesh = GetMeshForChunk(&Thread->EngineResources->MeshFreelist, Thread->PermMemory);
            BuildWorldChunkMeshFromMarkedVoxels_Greedy(Chunk->Voxels, World->ChunkDim, {}, World->ChunkDim, PrimaryMesh, Thread->TempMemory);
            if (PrimaryMesh->At)
            { Ensure( AtomicReplaceMesh(&Chunk->Meshes, MeshBit_Main, PrimaryMesh, PrimaryMesh->Timestamp) == 0); }
            else
            { DeallocateMesh(&PrimaryMesh, &Thread->EngineResources->MeshFreelist, Thread->PermMemory); }
          }

        }
      }

      FinalizeChunkInitialization(Chunk);

    } break;

    case type_work_queue_entry_copy_buffer_ref:
    {
      work_queue_entry_copy_buffer_ref *CopyJob = SafeAccess(work_queue_entry_copy_buffer_ref, Entry);
      DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
    } break;

    case type_work_queue_entry_copy_buffer:
    {
      volatile work_queue_entry_copy_buffer *CopyJob = SafeAccess(work_queue_entry_copy_buffer, Entry);
      DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
    } break;

    case type_work_queue_entry_copy_buffer_set:
    {
      TIMED_BLOCK("Copy Set");
      volatile work_queue_entry_copy_buffer_set *CopySet = SafeAccess(work_queue_entry_copy_buffer_set, Entry);
      for (u32 CopyIndex = 0; CopyIndex < CopySet->Count; ++CopyIndex)
      {
        volatile work_queue_entry_copy_buffer *CopyJob = CopySet->CopyTargets + CopyIndex;
        DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
      }
      END_BLOCK("Copy Set");
    } break;

  }
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

void
DoSplotion( engine_resources *Resources, picked_voxel *Pick, canonical_position PickCP, f32 Radius, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Resources);

  QueueWorldUpdateForRegion(Plat, World, Pick, WorldUpdateOperation_Subtractive, DARK_GREY, Radius, Resources->Memory);

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


    v3 MaxPower = V3(15.f, 15.f, 3.f) * Radius;
    v3 Power = Lerp(t, MaxPower, V3(0) );

    /* DebugLine("t(%f) Power(%f,%f,%f)", t, Power.x, Power.y, Power.z); */

    HitEntity->Physics.Force += Normalize(SplosionToEntityCenter) * Power;
  }
#endif

#if 1
  {
    entity *E = GetFreeEntity(EntityTable);
    SpawnEntity( 0, E, EntityType_ParticleSystem, ModelIndex_None);
    E->P = PickCP + V3(0.5f);
    SpawnExplosion(E, &Global_GameEntropy, {}, Radius);
  }
  {
    entity *E = GetFreeEntity(EntityTable);
    SpawnEntity( 0, E, EntityType_ParticleSystem, ModelIndex_None);
    E->P = PickCP + V3(0.5f);
    SpawnSmoke(E, &Global_GameEntropy, {}, Radius);
  }
#endif

#if 1
  u32 MaxBitties = 4*u32(Radius);
  for (u32 BittyIndex = 0; BittyIndex < MaxBitties; ++BittyIndex)
  {
    entity *E = GetFreeEntity(EntityTable);
    SpawnEntity( GameState->Models , E, EntityType_ParticleSystem, (model_index)(ModelIndex_Bitty0 + (BittyIndex % 2)) );
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
        DebugLine("move");
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

  /* entity *Enemy = GameState->Enemy; */

  /* standing_spot_buffer EnemySpots = GetStandingSpotsWithinRadius(World, Enemy->P, EnemyMoveSpeed, GetTranArena()); */
  /* for (u32 SpotIndex = 0; SpotIndex < EnemySpots.Count; ++SpotIndex) */
  /* { */
  /*   standing_spot *Spot = EnemySpots.Start + SpotIndex; */
  /*   v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera); */
  /*   DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), RED, DEFAULT_LINE_THICKNESS*3.f); */
  /* } */



  picked_voxel Pick = PickVoxel(Resources);

  local_persist player_action SelectedAction = {};
  local_persist u32 PlayerChargeLevel = {};

  if (Pick.PickedChunk.tChunk != f32_MAX)
  {
    world_chunk *ClosestChunk = Pick.PickedChunk.Chunk;
    v3 MinP =  V3(ClosestChunk->WorldP * World->ChunkDim);
    v3 VoxelP = MinP + Truncate(Pick.VoxelRelP);

    canonical_position PickCP = Canonical_Position(Pick.VoxelRelP, ClosestChunk->WorldP);

    v3 CursorSimP = GetSimSpaceP(World, PickCP);

    untextured_3d_geometry_buffer OutlineAABB = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
    v3 Offset = V3(0.001f);
    DEBUG_DrawAABB( &OutlineAABB,
                    GetRenderP(World->ChunkDim, VoxelP-Offset, Camera),
                    GetRenderP(World->ChunkDim, VoxelP+V3(1.f)+Offset, Camera),
                    WHITE, 0.05f);


    /* if (Input->F8.Clicked) */
    /* { */
    /*   DoSplotion(Resources, &Pick, PickCP, 20.f); */
    /* } */


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
          OutlineAABB.At = 0;
          DEBUG_DrawAABB( &OutlineAABB,
                          GetRenderP(World->ChunkDim, VoxelP-Offset, Camera),
                          GetRenderP(World->ChunkDim, VoxelP+V3(1.f)+Offset, Camera),
                          RED, 0.15f);
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

  PushForceAdvance(GameUi, V2(0, 128));
  for (u32 ActionIndex = 0; ActionIndex < PlayerAction_Count; ++ActionIndex)
  {
    ui_style *Style = ActionIndex == SelectedAction ? &DefaultSelectedStyle : &DefaultStyle;
    PushTableStart(GameUi);
      PushColumn(GameUi, ToString((player_action)ActionIndex), Style);
      PushNewRow(GameUi);
    PushTableEnd(GameUi);
  }


  if (Hotkeys)
  {
    r32 CameraSpeed = 125.f;
    v3 CameraDelta = (GetCameraRelativeInput(Hotkeys, Camera));
    CameraDelta.z = 0.f;
    CameraDelta = Normalize(CameraDelta) * CameraSpeed * Plat->dt;

    GameState->CameraTarget->P.Offset += CameraDelta;
    Canonicalize(World->ChunkDim, GameState->CameraTarget->P);
  }

  /* if ( IsGrounded(World, Player, World->VisibleRegion) && Hotkeys->Player_Jump ) */
  /* { */
  /*   Player->Physics.Force += V3(0.f, 0.f, 0.5f) * Player->Physics.Speed; */
  /* } */

  /* if (IsGrounded(World, Player, World->VisibleRegion))// && Hotkeys->Player_Jump) */
  /* { */
  /*   Player->Physics.Force += V3(0, 0, 1); */
  /* } */

  /* if (Hotkeys->Player_Spawn) */
  /* { */
  /*   Unspawn(Player); */
  /*   SpawnPlayerLikeEntity(Plat, World, GameState->Models, Player,  Canonical_Position(V3(0,0,0), World_Position(0,0,2)), &GameState->Entropy); */
  /*   World->Center = World_Position(0, 0, 2); */
  /* } */


  return;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/turn_based/assets");

  world_position WorldCenter = World_Position(2, 2, 2);
  /* world_position WorldCenter = {}; */
  canonical_position PlayerSpawnP = Canonical_Position(Voxel_Position(0), WorldCenter + World_Position(0,0,3));

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f, PlayerSpawnP);
  /* Graphics->Camera->CurrentP.WorldP = World_Position(1, -1, 1); */
  /* Graphics->Camera->CurrentP.Offset = V3(1, -1, 1); */

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  random_series WorldEntropy = {54930695483};
  InitializeWorld_WFC(Resources->World, g_VisibleRegion, V3i(8), Memory, &WorldEntropy);

  GameState->Models = AllocateGameModels(GameState, Memory, Heap);

#if 0
  u32 PlayerModelIndex = RandomBetween(ModelIndex_FirstPlayerModel, &GameState->Entropy, ModelIndex_LastPlayerModel+1);
  GameState->Player = GetFreeEntity(EntityTable);
  SpawnPlayerLikeEntity(Plat, World, GameState->Models + PlayerModelIndex, GameState->Player, PlayerSpawnP, &GameState->Entropy);

  u32 EnemyCount = 3;
  v3i HalfVisibleRegion = g_VisibleRegion / 2;
  HalfVisibleRegion.z = 0;
  for (u32 EnemyIndex = 0; EnemyIndex < EnemyCount; ++EnemyIndex)
  {
    world_position WP = World_Position(
        (s32)RandomBetween(0, &GameState->Entropy, (u32)g_VisibleRegion.x),
        (s32)RandomBetween(0, &GameState->Entropy, (u32)g_VisibleRegion.y),
        1);

    u32 EnemyModelIndex = RandomBetween(ModelIndex_FirstEnemyModel, &GameState->Entropy, ModelIndex_Enemy_Skeleton_AxeArmor+1);
    Assert(EnemyModelIndex >= ModelIndex_FirstEnemyModel);
    Assert(EnemyModelIndex <= ModelIndex_LastEnemyModel);

    auto EnemySpawnP = Canonical_Position(V3(0), WorldCenter + WP - HalfVisibleRegion );
    auto Enemy = GetFreeEntity(EntityTable);
    Enemy->Update = EnemyUpdate;
    SpawnPlayerLikeEntity(Plat, World, GameState->Models + EnemyModelIndex, Enemy, EnemySpawnP, &GameState->Entropy, 0.35f);
  }
#endif

  WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

  GameState->CameraTarget = GetFreeEntity(EntityTable);
  SpawnEntity( 0, GameState->CameraTarget, EntityType_Default, ModelIndex_None);

  GameState->CameraTarget->P = Canonical_Position(Voxel_Position(0), {{2,2,0}});

  Resources->CameraTargetP = &GameState->CameraTarget->P;

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}
