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
  Result[ModelIndex_Enemy_Skeleton_Champion]         = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_CHAMPION.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_ChampionChampion] = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_CHAMPION_CATAPHRACT.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_Concubiner]       = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_CONCUBINER.vox", Memory);
  Result[ModelIndex_Enemy_Skeleton_King]             = LoadVoxModel(Memory, Heap, "models/skeletons/SKELLINGTON_KING.vox", Memory);

  Result[ModelIndex_Player_jp]      = LoadVoxModel(Memory, Heap, "models/chr_jp.vox", Memory);
  Result[ModelIndex_Player_bow]     = LoadVoxModel(Memory, Heap, "models/chr_bow.vox", Memory);
  Result[ModelIndex_Player_cat]     = LoadVoxModel(Memory, Heap, "models/chr_cat.vox", Memory);
  Result[ModelIndex_Player_fox]     = LoadVoxModel(Memory, Heap, "models/chr_fox.vox", Memory);
  Result[ModelIndex_Player_gumi]    = LoadVoxModel(Memory, Heap, "models/chr_gumi.vox", Memory);
  Result[ModelIndex_Player_knight]  = LoadVoxModel(Memory, Heap, "models/chr_knight.vox", Memory);
  Result[ModelIndex_Player_man]     = LoadVoxModel(Memory, Heap, "models/chr_man.vox", Memory);
  Result[ModelIndex_Player_mom]     = LoadVoxModel(Memory, Heap, "models/chr_mom.vox", Memory);
  Result[ModelIndex_Player_old]     = LoadVoxModel(Memory, Heap, "models/chr_old.vox", Memory);
  Result[ModelIndex_Player_poem]    = LoadVoxModel(Memory, Heap, "models/chr_poem.vox", Memory);
  Result[ModelIndex_Player_rain]    = LoadVoxModel(Memory, Heap, "models/chr_rain.vox", Memory);
  Result[ModelIndex_Player_sasami]  = LoadVoxModel(Memory, Heap, "models/chr_sasami.vox", Memory);
  Result[ModelIndex_Player_sol]     = LoadVoxModel(Memory, Heap, "models/chr_sol.vox", Memory);
  Result[ModelIndex_Player_sword]   = LoadVoxModel(Memory, Heap, "models/chr_sword.vox", Memory);
  Result[ModelIndex_Player_tale]    = LoadVoxModel(Memory, Heap, "models/chr_tale.vox", Memory);
  Result[ModelIndex_Player_tama]    = LoadVoxModel(Memory, Heap, "models/chr_tama.vox", Memory);
  Result[ModelIndex_Player_tsurugi] = LoadVoxModel(Memory, Heap, "models/chr_tsurugi.vox", Memory);

  Result[ModelIndex_Bitty0] = LoadVoxModel(Memory, Heap, "models/splotion_bitty_0.vox", Memory);
  Result[ModelIndex_Bitty1] = LoadVoxModel(Memory, Heap, "models/splotion_bitty_1.vox", Memory);

  return Result;
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
#if 1
          counted_string AssetFilename = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);

          native_file AssetFile = OpenFile(AssetFilename, "r+b");
#endif
          {

            /* s32 Frequency = 0; */
            /* s32 Amplititude = 0; */
            /* s32 StartingZDepth = 0; */

            /* s32 Frequency = 50; */
            /* s32 Amplititude = 50; */
            /* s32 StartingZDepth = -40; // TODO(Jesse): Figure out why this isn't doing anything */

            s32 Frequency = 50;
            s32 Amplititude = 15;
            s32 StartingZDepth = -5;

            Assert(Chunk->Dim == World->ChunkDim);
            InitializeWorldChunkPerlinPlane( Thread,
                                             Chunk,
                                             Chunk->Dim,
                                             &AssetFile,
                                             Frequency,
                                             Amplititude,
                                             StartingZDepth );

            FullBarrier;

            /* Chunk->LodMesh_Complete = True; */
            /* Assert( NotSet(Chunk, Chunk_Queued )); */

          }

        }
        else if ( NotSet(Chunk->Flags, Chunk_MeshesInitialized) )
        {
          InvalidCodePath();
          /* Assert( IsSet(Chunk->Flags, Chunk_VoxelsInitialized) ); */

          /* /1* if ( DestChunk->FilledCount > 0 && *1/ */
          /* /1*      DestChunk->FilledCount < (u32)Volume(WorldChunkDim)) *1/ */
          /* { */
          /*   untextured_3d_geometry_buffer *NewMesh = GetMeshForChunk(&Thread->EngineResources->MeshFreelist, Thread->PermMemory); */
          /*   BuildWorldChunkMesh(Chunk, Chunk->Dim, {}, Chunk->Dim, NewMesh); */

          /*   untextured_3d_geometry_buffer *OldMesh = (untextured_3d_geometry_buffer*)AtomicReplace((volatile void**)&Chunk->Mesh, NewMesh); */
          /*   if (OldMesh) { DeallocateMesh(&OldMesh, &Thread->EngineResources->MeshFreelist, Thread->PermMemory); } */
          /* } */

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

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES();

  entity *Player = GameState->Player;

  /* v3 RotP = {}; */
  /* RotP.x = Sin(r32(Plat->GameTime)); */
  /* RotP.y = Cos(r32(Plat->GameTime)); */
  /* Player->Rotation = RotatePoint(V3(0.f, -1.f, 0.f), RotP); */

  entity *Enemy = GameState->Enemy;

  f32 StandingSpotSearchThresh = 15.f;

  standing_spot_buffer EnemySpots = GetStandingSpotsWithinRadius(World, Enemy->P, StandingSpotSearchThresh, GetTranArena());
  for (u32 SpotIndex = 0; SpotIndex < EnemySpots.Count; ++SpotIndex)
  {
    standing_spot *Spot = EnemySpots.Start + SpotIndex;
    v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera);
    DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), RED, DEFAULT_LINE_THICKNESS*3.f);
  }



  picked_voxel Pick = PickVoxel(Resources);

  if (Pick.PickedChunk.tChunk != f32_MAX)
  {
    world_chunk *ClosestChunk = Pick.PickedChunk.Chunk;
    v3 MinP =  V3(ClosestChunk->WorldP * World->ChunkDim);
    v3 VoxelP = MinP + Truncate(Pick.VoxelRelP);

    canonical_position PickCP = Canonical_Position(Pick.VoxelRelP, ClosestChunk->WorldP);

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

    local_persist player_action SelectedAction = {};
    local_persist b32 PlayerCharged = {};

    if (Input->Q.Clicked)
    {
      SelectedAction = PlayerAction_Move;
    }

    if (Input->W.Clicked)
    {
      SelectedAction = PlayerAction_Charge;
    }

    if (Input->E.Clicked)
    {
      SelectedAction = PlayerAction_Fire;
    }

    if (Input->R.Clicked)
    {
      SelectedAction = PlayerAction_Jump;
    }

    if (Input->T.Clicked)
    {
      DoSplotion(Resources, &Pick, PickCP, 4.f, GetTranArena());
    }

    if (Input->Y.Clicked)
    {
      DoSplotion(Resources, &Pick, PickCP, 6.f, GetTranArena());
    }

    if (Input->U.Clicked)
    {
      DoSplotion(Resources, &Pick, PickCP, 8.f, GetTranArena());
    }



    GetDebugState()->DebugValue_u64(SelectedAction, ToString(SelectedAction).Start);

    b32 DidPlayerAction = False;
    switch (SelectedAction)
    {
      case PlayerAction_None:
      {
      } break;

      case PlayerAction_Move:
      {
        standing_spot_buffer PlayerSpots = GetStandingSpotsWithinRadius(World, Player->P, StandingSpotSearchThresh, GetTranArena());
        for (u32 SpotIndex = 0; SpotIndex < PlayerSpots.Count; ++SpotIndex)
        {
          standing_spot *Spot = PlayerSpots.Start + SpotIndex;
          v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera);
          DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), GREEN, DEFAULT_LINE_THICKNESS*3.f);
        }


        for (u32 StandingSpotIndex = 0;
                 StandingSpotIndex < AtElements(&ClosestChunk->StandingSpots);
               ++StandingSpotIndex)
        {
          v3 SpotOffset = V3(ClosestChunk->StandingSpots.Start[StandingSpotIndex]);
          standing_spot Spot = StandingSpot(SpotOffset, ClosestChunk->WorldP);

          aabb SpotRect = AABBMinMax(SpotOffset, SpotOffset+Global_StandingSpotDim);
          if (IsInside(SpotRect, Truncate(Pick.VoxelRelP)))
          {
            /* untextured_3d_geometry_buffer SpotAABB = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB); */
            v3 RenderP = GetRenderP(World->ChunkDim, MinP+SpotOffset, Camera);
            DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), RED, DEFAULT_LINE_THICKNESS*3.f);
            if (Input->LMB.Clicked)
            {
              v3 PlayerSimP = GetSimSpaceP(World, Player->P);
              v3 SpotSimP = GetSimSpaceP(World, Spot.P);
              if (PointsAreWithinDistance(PlayerSimP, SpotSimP, StandingSpotSearchThresh))
              {
                DidPlayerAction = True;
                Player->P = MoveToStandingSpot(World, Canonical_Position(SpotOffset, ClosestChunk->WorldP) );
              }
            }
          }
        }
      } break;

      case PlayerAction_Charge:
      {
        if (PlayerCharged)
        {
        }
        else
        {
          if (Input->LMB.Clicked)
          {
            PlayerCharged = True;
            SpawnFire(Player, &GameState->Entropy, Global_EntityFireballOffset);
            DidPlayerAction = True;
          }
        }
      } break;

      case PlayerAction_Fire:
      {
        if (PlayerCharged)
        {
          OutlineAABB.At = 0;
          DEBUG_DrawAABB( &OutlineAABB,
                          GetRenderP(World->ChunkDim, VoxelP-Offset, Camera),
                          GetRenderP(World->ChunkDim, VoxelP+V3(1.f)+Offset, Camera),
                          RED, 0.15f);
          if (Input->LMB.Clicked)
          {
            PlayerCharged = False;
            Deactivate(Player->Emitter);
            DidPlayerAction = True;
            DoSplotion(Resources, &Pick, PickCP, 5.f, GetTranArena());
          }
        }
      } break;

      case PlayerAction_Jump:
      {
        if (Input->LMB.Clicked)
        {
          DidPlayerAction = True;
        }
      } break;
    }


    if (DidPlayerAction)
    {
      SelectedAction = PlayerAction_None;
      /* u32 EnemyChoice = RandomU32(&Global_GameEntropy) % 4; */
      u32 EnemyChoice = 0;

      local_persist u32 EnemyPower = 0;
      switch(EnemyChoice)
      {
        case 0:
        case 1:
        case 2:
        {
          DebugLine("move");
          /* world_chunk *EnemyChunk = GetWorldChunk(World, ); */
          if (EnemySpots.Count)
          {
            for (;;)
            {
              u32 SpotChoice = RandomU32(&Global_GameEntropy) % EnemySpots.Count;
              canonical_position ChoiceP = EnemySpots.Start[SpotChoice].P;
              v3 ChoiceSimP = GetSimSpaceP(World, ChoiceP);
              v3 EnemySimP = GetSimSpaceP(World, Enemy->P);

              if (PointsAreWithinDistance(ChoiceSimP, EnemySimP, 5.f))
              {
                if (EnemySpots.Count == 1) break;
              }
              else
              {
                Enemy->P = MoveToStandingSpot(World, ChoiceP); break;
              }
            }
          }
        } break;

        case 3:
        {
          if (EnemyPower)
          {
            DebugLine("shoot");
            EnemyPower=0;
          }
          else
          {
            DebugLine("EnemyPower");
            EnemyPower++;
          }
        } break;


        InvalidDefaultCase;
      }
    }
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

  if ( IsGrounded(World, Player, World->VisibleRegion) && Hotkeys->Player_Jump )
  {
    Player->Physics.Force += V3(0.f, 0.f, 0.5f) * Player->Physics.Speed;
  }

  /* if (IsGrounded(World, Player, World->VisibleRegion))// && Hotkeys->Player_Jump) */
  /* { */
  /*   Player->Physics.Force += V3(0, 0, 1); */
  /* } */

  /* if (Hotkeys->Player_Spawn) */
  /* { */
  /*   Unspawn(Player); */
  /*   SpawnPlayer(Plat, World, GameState->Models, Player,  Canonical_Position(V3(0,0,0), World_Position(0,0,2)), &GameState->Entropy); */
  /*   World->Center = World_Position(0, 0, 2); */
  /* } */


  return;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  SetThreadLocal_ThreadIndex(0);

  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/turn_based/assets");

  world_position WorldCenter = World_Position(2, 2, 0);
  canonical_position PlayerSpawnP = Canonical_Position(Voxel_Position(0), WorldCenter + World_Position(0,0,3));

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f, PlayerSpawnP);
  /* Graphics->Camera->CurrentP.WorldP = World_Position(1, -1, 1); */
  /* Graphics->Camera->CurrentP.Offset = V3(1, -1, 1); */

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateAndInitWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  GameState->Models = AllocateGameModels(GameState, Memory, Heap);

  u32 PlayerModelIndex = RandomBetween(ModelIndex_FirstPlayer, &GameState->Entropy, ModelIndex_LastPlayer+1);
  GameState->Player = GetFreeEntity(EntityTable);
  SpawnPlayer(Plat, World, GameState->Models + PlayerModelIndex, GameState->Player, PlayerSpawnP, &GameState->Entropy);

  u32 EnemyCount = 32;
  v3i HalfVisibleRegion = g_VisibleRegion / 2;
  HalfVisibleRegion.z = 0;
  for (u32 EnemyIndex = 0; EnemyIndex < EnemyCount; ++EnemyIndex)
  {
    world_position WP = World_Position(
        (s32)RandomBetween(0, &GameState->Entropy, (u32)g_VisibleRegion.x),
        (s32)RandomBetween(0, &GameState->Entropy, (u32)g_VisibleRegion.y),
        1);

    u32 EnemyModelIndex = RandomBetween(ModelIndex_FirstEnemy, &GameState->Entropy, ModelIndex_LastEnemy+1);
    Assert(EnemyModelIndex >= ModelIndex_FirstEnemy);
    Assert(EnemyModelIndex <= ModelIndex_LastEnemy);

    auto EnemySpawnP = Canonical_Position(V3(0), WorldCenter + WP - HalfVisibleRegion );
    GameState->Enemy = GetFreeEntity(EntityTable);
    SpawnPlayer(Plat, World, GameState->Models + EnemyModelIndex, GameState->Enemy, EnemySpawnP, &GameState->Entropy, 0.35f);
  }

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
