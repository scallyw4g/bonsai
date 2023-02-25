#define PLATFORM_GL_IMPLEMENTATIONS 1
#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

link_internal void
QueueWorldUpdateForRegion(platform *Plat, world *World, picked_voxel *Location, f32 Radius, memory_arena *Memory)
{
  TIMED_FUNCTION();

  canonical_position P = Canonical_Position(Location);

  auto MinP = Canonicalize(World, P-V3(Radius));
  auto MaxP = Canonicalize(World, P+V3(Radius));

  auto LocationSimSpace = GetSimSpaceP(World, P);

  aabb SimSpaceQueryAABB = AABBMinMax( (LocationSimSpace - Radius),
                                       (LocationSimSpace + Radius) + 1.0f );

  world_position Delta = MaxP.WorldP - MinP.WorldP + 1;
  u32 TotalChunkCount = Abs(Volume(Delta));

  // TODO(Jesse)(leak): Each one of these gets leaked at the moment
  world_chunk **Buffer = AllocateAligned(world_chunk*, Memory, TotalChunkCount, CACHE_LINE_SIZE);

  u32 ChunkIndex = 0;
  for (s32 zChunk = MinP.WorldP.z; zChunk <= MaxP.WorldP.z; ++zChunk)
  {
    for (s32 yChunk = MinP.WorldP.y; yChunk <= MaxP.WorldP.y; ++yChunk)
    {
      for (s32 xChunk = MinP.WorldP.x; xChunk <= MaxP.WorldP.x; ++xChunk)
      {
        world_position ChunkP = World_Position(xChunk, yChunk, zChunk);
        world_chunk *Chunk = GetWorldChunk(World, ChunkP);
        if (Chunk)
        {
          Assert(ChunkIndex < TotalChunkCount);
          Buffer[ChunkIndex++] = Chunk;
        }
      }
    }
  }

  work_queue_entry Entry = {
    .Type = type_work_queue_entry_update_world_region,
    .work_queue_entry_update_world_region = WorkQueueEntryUpdateWorldRegion(Location, Radius, Buffer, ChunkIndex),
  };
  PushWorkQueueEntry(&Plat->LowPriority, &Entry);
}

link_internal void
DoWorldUpdate(work_queue *Queue, world *World, world_chunk **ChunkBuffer, u32 ChunkCount, picked_voxel *Location, f32 Radius)
{
  TIMED_FUNCTION();

  canonical_position P = Canonical_Position(Location);

  auto MinP = Canonicalize(World, P-V3(Radius));
  auto MaxP = Canonicalize(World, P+V3(Radius));

  auto LocationSimSpace = GetSimSpaceP(World, P);

  aabb SimSpaceQueryAABB = AABBMinMax( (LocationSimSpace - Radius),
                                       (LocationSimSpace + Radius) + 1.0f );

  for (u32 ChunkIndex = 0; ChunkIndex < ChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = ChunkBuffer[ChunkIndex];
    Assert(Chunk);
    {
      aabb SimSpaceChunkRect = GetSimSpaceAABB(World, Chunk);
      aabb SimSpaceIntersectionRect = Difference(&SimSpaceChunkRect, &SimSpaceQueryAABB);

      auto SimSpaceIntersectionMin = GetMin(SimSpaceIntersectionRect);
      auto SimSpaceIntersectionMax = GetMax(SimSpaceIntersectionRect);

      v3 SimSpaceChunkMin = GetMin(SimSpaceChunkRect);
      /* v3 SimSpaceChunkMax = GetMax(SimSpaceChunkRect); */

      auto ChunkRelRectMin = SimSpaceIntersectionMin - SimSpaceChunkMin;
      auto ChunkRelRectMax = SimSpaceIntersectionMax - SimSpaceChunkMin;

      b32 AnyVoxelsModified = False;
      for (s32 zVoxel = s32(ChunkRelRectMin.z); zVoxel < s32(ChunkRelRectMax.z); ++zVoxel)
      {
        for (s32 yVoxel = s32(ChunkRelRectMin.y); yVoxel < s32(ChunkRelRectMax.y); ++yVoxel)
        {
          for (s32 xVoxel = s32(ChunkRelRectMin.x); xVoxel < s32(ChunkRelRectMax.x); ++xVoxel)
          {
            voxel_position RelVoxP = Voxel_Position(xVoxel, yVoxel, zVoxel);
            voxel *V = GetVoxel(Chunk, RelVoxP);

            v3 SimSpaceVoxP = V3(RelVoxP) + SimSpaceChunkMin;
            if (LengthSq(SimSpaceVoxP - LocationSimSpace) < Square(Radius))
            {
              if (V->Flags & Voxel_Filled) { --Chunk->FilledCount; AnyVoxelsModified = True; }
              V->Flags = Voxel_Empty;
            }

          }
        }
      }

      FullBarrier;

      if (AnyVoxelsModified)
      {
        UnSetFlag(&Chunk->Flags, Chunk_Queued);
        UnSetFlag(&Chunk->Flags, Chunk_MeshesInitialized);
        /* QueueChunkForInit(Queue, Chunk); */
        QueueChunkForMeshRebuild(Queue, Chunk);
      }

    }
  }
}


model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  model *Result = Allocate(model, Memory, ModelIndex_Count);
  Result[ModelIndex_Enemy] = LoadVoxModel(Memory, Heap, "models/chr_tama.vox");

  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_bow.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_cat.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_fox.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_gumi.vox"); */
  Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_jp.vox");
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_knight.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_man.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_mom.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_old.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_poem.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_rain.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_sasami.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_sol.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_sword.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_tale.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_tama.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_tsurugi.vox"); */
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "../voxel-model/vox/monument/monu10.vox"); */
  /* Result[ModelIndex_Player] = LoadWorldChunk(Memory, Heap, "assets/world_chunk_1_0_0"); */

  return Result;
}

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  work_queue_entry_type Type = Entry->Type;
  switch (Type)
  {
    case type_work_queue_entry_noop: { InvalidCodePath(); } break;

    case type_work_queue_entry_init_asset:
    {
      InvalidCodePath();

#if 0
      volatile work_queue_entry_init_asset *Job = SafeAccess(work_queue_entry_init_asset, Entry);
      world_chunk *Chunk = Job->Chunk;

      if (ChunkIsGarbage(Chunk))
      {
      }
      else
      {
        // TODO(Jesse): Improve ergonomics here?
        counted_string AssetPath = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);
        const char* zAssetPath = GetNullTerminated(AssetPath, Thread->TempMemory);
        DeserializeChunk(zAssetPath, Chunk);
      }

      FinalizeChunkInitialization(Chunk);
#endif

    } break;

    case type_work_queue_entry_update_world_region:
    {
      /* DebugLine("update world!"); */
      work_queue_entry_update_world_region *Job = SafeAccess(work_queue_entry_update_world_region, Entry);

      picked_voxel Location = Job->Location;
      r32 Radius = Job->Radius;
      world *World = Thread->EngineResources->World;

      DoWorldUpdate(&Thread->EngineResources->Plat->LowPriority, World, Job->ChunkBuffer, Job->ChunkCount, &Location, Radius);
    } break;

    case type_work_queue_entry_rebuild_mesh:
    {
      work_queue_entry_rebuild_mesh *Job = SafeAccess(work_queue_entry_rebuild_mesh, Entry);
      world_chunk *Chunk = Job->Chunk;
      world *World = Thread->EngineResources->World;

      Assert( IsSet(Chunk->Flags, Chunk_VoxelsInitialized) );

      if ( Chunk->SelectedMeshes & MeshIndex_Main )
      {
        untextured_3d_geometry_buffer *NewMesh = GetMeshForChunk(&Thread->EngineResources->MeshFreelist, Thread->PermMemory);
        BuildWorldChunkMesh(Chunk, Chunk->Dim, {}, Chunk->Dim, NewMesh);

        untextured_3d_geometry_buffer *OldMesh = (untextured_3d_geometry_buffer*)AtomicReplace((volatile void**)&Chunk->Mesh, NewMesh);
        if (OldMesh) { DeallocateMesh(&OldMesh, &Thread->EngineResources->MeshFreelist, Thread->PermMemory); }
      }

      if ( Chunk->SelectedMeshes & MeshIndex_Lod )
      {
        untextured_3d_geometry_buffer *LodMesh = GetMeshForChunk(&Thread->EngineResources->MeshFreelist, Thread->PermMemory);
        ComputeLodMesh( Thread, Chunk, World->ChunkDim, Chunk, World->ChunkDim, LodMesh, False);

        untextured_3d_geometry_buffer *OldMesh = (untextured_3d_geometry_buffer*)AtomicReplace((volatile void**)&Chunk->LodMesh, LodMesh);
        if (OldMesh) { DeallocateMesh(&OldMesh, &Thread->EngineResources->MeshFreelist, Thread->PermMemory); }
      }

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
#if 0
          counted_string AssetFilename = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, Thread->TempMemory);

          native_file AssetFile = OpenFile(AssetFilename, "r+b");
          if (AssetFile.Handle)
          {
            DeserializeChunk(&AssetFile, Chunk, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
            CloseFile(&AssetFile);
            // fsync?
          }
          else
#endif
          {
            s32 Frequency = 20;
            s32 Amplititude = 6;
            s32 StartingZDepth = 4;
            InitializeWorldChunkPerlinPlane( Thread,
                                             Chunk,
                                             WORLD_CHUNK_DIM,
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

global_variable random_series EnemyEntropy = {543232654};

link_internal canonical_position
MoveToStandingSpot(world *World, canonical_position P)
{
  canonical_position Result = Canonicalize(World->ChunkDim, Canonical_Position(P.Offset + V3(0,0,5), P.WorldP));
  return Result;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES();

  entity *Player = GameState->Player;
  Player->Physics.Speed = 60.f;
  Player->Physics.Mass = 35.f;

  entity *Enemy = GameState->Enemy;

  f32 StandingSpotSearchThresh = 15.f;

  standing_spot_buffer EnemySpots = GetStandingSpotsWithinRadius(World, Enemy->P, StandingSpotSearchThresh, GetTranArena());
  for (u32 SpotIndex = 0; SpotIndex < EnemySpots.Count; ++SpotIndex)
  {
    standing_spot *Spot = EnemySpots.Start + SpotIndex;
    v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera);
    DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), RED, DEFAULT_LINE_THICKNESS*3.f);
  }



  standing_spot_buffer PlayerSpots = GetStandingSpotsWithinRadius(World, Player->P, StandingSpotSearchThresh, GetTranArena());
  for (u32 SpotIndex = 0; SpotIndex < PlayerSpots.Count; ++SpotIndex)
  {
    standing_spot *Spot = PlayerSpots.Start + SpotIndex;
    v3 RenderP = GetRenderP(World->ChunkDim, Spot, Camera);
    DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), GREEN, DEFAULT_LINE_THICKNESS*3.f);
  }


  picked_voxel Pick = PickVoxel(Resources);

  if (Pick.PickedChunk.tChunk != f32_MAX)
  {
    world_chunk *ClosestChunk = Pick.PickedChunk.Chunk;
    v3 MinP =  V3(ClosestChunk->WorldP * World->ChunkDim);
    v3 VoxelP = MinP + Truncate(Pick.VoxelRelP);

    untextured_3d_geometry_buffer OutlineAABB = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
    v3 Offset = V3(0.001f);
    DEBUG_DrawAABB( &OutlineAABB,
                    GetRenderP(World->ChunkDim, VoxelP-Offset, Camera),
                    GetRenderP(World->ChunkDim, VoxelP+V3(1.f)+Offset, Camera),
                    WHITE, 0.05f);


    if (Input->F8.Clicked)
    {
#if 1
      QueueWorldUpdateForRegion(Plat, World, &Pick, 2.f, Resources->Memory);
#else
      DoFireballAt(World, &Pick, 100.f);
#endif
    }


    for (u32 StandingSpotIndex = 0;
             StandingSpotIndex < ClosestChunk->StandingSpots.Count;
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
            Player->P = MoveToStandingSpot(World, Canonical_Position(SpotOffset, ClosestChunk->WorldP) );

            u32 EnemyChoice = 0; // RandomU32(&EnemyEntropy) % 4;

            local_persist b32 Power = False;
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
                    u32 SpotChoice = RandomU32(&EnemyEntropy) % EnemySpots.Count;
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
                if (Power)
                {
                  DebugLine("shoot");
                  Power=False;
                }
                else
                {
                  DebugLine("power");
                  Power=True;
                }
              } break;


              InvalidDefaultCase;
            }
          }
        }
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

  if (Hotkeys->Player_Spawn)
  {
    Unspawn(Player);
    SpawnPlayer(Plat, World, GameState->Models, Player,  Canonical_Position(V3(0,0,0), World_Position(0,0,2)), &GameState->Entropy);
    World->Center = World_Position(0, 0, 2);
  }


  return;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  SetThreadLocal_ThreadIndex(0);

  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/asset_picker/assets");

  world_position WorldCenter = World_Position(2, 2, 3);
  canonical_position PlayerSpawnP = Canonical_Position(Voxel_Position(0), WorldCenter);

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f, PlayerSpawnP);
  /* Graphics->Camera->CurrentP.WorldP = World_Position(1, -1, 1); */
  /* Graphics->Camera->CurrentP.Offset = V3(1, -1, 1); */

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateAndInitWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  GameState->Models = AllocateGameModels(GameState, Memory, Heap);

  GameState->Player = GetFreeEntity(EntityTable);
  SpawnPlayer(Plat, World, GameState->Models + ModelIndex_Player, GameState->Player, PlayerSpawnP, &GameState->Entropy);

  auto EnemySpawnP = Canonical_Position(V3(0), World_Position(2,1,3));
  GameState->Enemy = GetFreeEntity(EntityTable);
  SpawnPlayer(Plat, World, GameState->Models + ModelIndex_Enemy, GameState->Enemy, EnemySpawnP, &GameState->Entropy);


  GameState->CameraTarget = GetFreeEntity(EntityTable);
  SpawnEntity( 0, GameState->CameraTarget, EntityType_Default);

  GameState->CameraTarget->P = Canonical_Position(Voxel_Position(0), {{2,2,0}});

  Resources->CameraTargetP = &GameState->CameraTarget->P;

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}
