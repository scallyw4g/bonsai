#define PLATFORM_GL_IMPLEMENTATIONS 1
#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <examples/tools/voxel_synthesis_rule_baker/game_constants.h>
#include <examples/tools/voxel_synthesis_rule_baker/game_types.h>

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  world *World = Thread->EngineResources->World;
  game_state *GameState = Thread->EngineResources->GameState;

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

      if (GameState->TileSuperpositions == 0) return;

      if (ChunkIsGarbage(Chunk))
      {
      }
      else
      {
        if ( NotSet(Chunk->Flags, Chunk_VoxelsInitialized) )
        {
          v3i TileDim = Global_TileDim;

          /* v3i AbsTileMaxDim = World->VisibleRegion*World->ChunkDim / TileDim; */
          Assert((World->VisibleRegion*World->ChunkDim%TileDim) == V3i(0));

          v3i TilesPerWorldChunk = World->ChunkDim / TileDim;

          v3i AbsTileMin = Chunk->WorldP * TilesPerWorldChunk;
          v3i AbsTileMax = AbsTileMin + TilesPerWorldChunk;

          voxel_synthesis_result *BakeResult = &GameState->BakeResult;
          auto AllTiles = &BakeResult->Tiles;
          auto BakeSrcVoxels = BakeResult->VoxData.ChunkData->Voxels;
          auto BakeSrcVoxelsDim = BakeResult->VoxData.ChunkData->Dim;

          MinMaxIterator(xTile, yTile, zTile, AbsTileMin, AbsTileMax)
          {
            v3i TileP = V3i(xTile, yTile, zTile);
            s32 TileIndex = GetIndex(TileP, GameState->TileSuperpositionsDim);
            Assert(TileIndex < Volume(GameState->TileSuperpositionsDim));
            u64 TileRule = GameState->TileSuperpositions[TileIndex];

            if (TileRule != 0)
            {
              Assert(CountBitsSet_Kernighan(TileRule) == 1);

              u32 RuleId = GetIndexOfNthSetBit(TileRule, 1);
              voxel_synth_tile *Match = 0;
              for (u32 QueryTileIndex = 0; QueryTileIndex < AllTiles->Count; ++QueryTileIndex)
              {
                voxel_synth_tile *QueryTile = Get(AllTiles, QueryTileIndex);
                if (QueryTile->RuleId == RuleId)
                {
                  Match = QueryTile;
                  break;
                }
              }

              Assert(Match);
              Assert(Match->RuleId == RuleId);
              Assert(TileRule & (1<<RuleId));

              v3i SrcVoxMin = V3iFromIndex(s32(Match->VoxelIndex), Match->SrcChunk->Dim );
              v3i SrcVoxMax = SrcVoxMin + TileDim;

              /* v3i DestVoxMin = SrcVoxMin % World->ChunkDim; */
              /* v3i DestVoxMax = DestVoxMin + TileDim; */

              DimIterator(xTileVox, yTileVox, zTileVox, Global_TileDim)
              {
                v3i TileVox = V3i(xTileVox, yTileVox, zTileVox);
                v3i SrcVox = SrcVoxMin + TileVox;
                s32 SrcVoxIndex = GetIndex(SrcVox, BakeSrcVoxelsDim);

                v3i DestVox = (TileVox+(TileP*Global_TileDim)) % World->ChunkDim;
                s32 DestVoxIndex = GetIndex(DestVox, World->ChunkDim);

                Chunk->Voxels[DestVoxIndex] = BakeSrcVoxels[SrcVoxIndex];

                Chunk->FilledCount +=
                  (Chunk->Voxels[DestVoxIndex].Flags&Voxel_Filled) ?
                  1 :
                  0 ;
              }

              /* s32 DestVoxIndex = GetIndex( (TileP*Global_TileDim) % World->ChunkDim, World->ChunkDim); */
              /* while (CountBitsSet_Kernighan(TileOptions)) */
              /* { */
              /*   UnsetLeastSignificantSetBit(&TileOptions); */
              /*   Chunk->Voxels[DestVoxIndex].Flags = Voxel_Filled; */
              /*   Chunk->Voxels[DestVoxIndex].Color = (u8)DestVoxIndex; */
              /*   Chunk->FilledCount++; */
              /*   DestVoxIndex = (DestVoxIndex + 1); // % Global_TileDim; */
              /* } */

              /* v3i SrcVoxOffset = TileP * Global_TileDim; */
              /* DimIterator(xVox, yVox, zVox, Global_TileDim) */
              /* { */
              /*   v3i VoxOffset = V3i(xVox, yVox, zVox); */
              /*   v3i SrcVoxP = VoxOffset + SrcVoxOffset; */
              /*   s32 SrcVoxIndex = TryGetIndex( SrcVoxP, BakeSrcVoxelsDim); */
              /*   if (SrcVoxIndex > -1) */
              /*   { */
              /*     voxel *V = BakeSrcVoxels + SrcVoxIndex; */

              /*     v3i DestTileP = TileP - TileMin; */
              /*     v3i DestVoxP = VoxOffset + (DestTileP * Global_TileDim); */
              /*     s32 DestVoxIndex = GetIndex( DestVoxP, World->ChunkDim); */
              /*     Chunk->Voxels[DestVoxIndex] = *V; */
              /*   } */
              /* } */
            }
            else
            {
#if 1
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
                    Chunk->Voxels[VoxIndex].Color = RED;
                    ++Chunk->FilledCount;
                  }
                }
              }
#endif
            }
            /* Info("Tile (%d, %d, %d)(%d)", xTile, yTile, zTile, TileOptions); */
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

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  /* CameraFollowEntity(); */

  if (Hotkeys)
  {
    r32 CameraSpeed = 125.f;
    v3 CameraDelta = (GetCameraRelativeInput(Hotkeys, Camera));
    CameraDelta.z = 0.f;
    CameraDelta = Normalize(CameraDelta) * CameraSpeed * Plat->dt;

    GameState->CameraTarget->P.Offset += CameraDelta;
    Canonicalize(World->ChunkDim, GameState->CameraTarget->P);
  }

  entity *Entity = MousePickEntity(Resources);
  if (Entity)
  {
    DrawEntityCollisionVolume(Entity, &GpuMap->Buffer, Graphics, World->ChunkDim, BLACK, 0.2f );

    voxel_synth_tile *HoverTile = (voxel_synth_tile*)Entity->UserData;
    if (HoverTile)
    {
      voxel_synth_tile_buffer BakedTiles = GameState->BakeResult.Tiles;
      tile_ruleset_buffer Rules = GameState->BakeResult.Rules;
      /* vox_data *VoxData = &GameState->BakeResult.VoxData; */
      /* chunk_data *ChunkData = VoxData->ChunkData; */
      for (u32 BakedTileIndex = 0; BakedTileIndex < BakedTiles.Count; ++BakedTileIndex)
      {
        voxel_synth_tile *BakeTile = BakedTiles.Start+BakedTileIndex;
        // NOTE(Jesse): We really should just be able to check the rule id, but
        // checking the hash value as well ensures that if we have tiles that
        // entirely hang off the edge (and thus are completely 0) get skipped.
        if ( BakeTile->RuleId    == HoverTile->RuleId &&
             BakeTile->HashValue == HoverTile->HashValue &&
             BakeTile->SrcChunk  == HoverTile->SrcChunk )
        {
          entity *E = GameState->BakeEntity;
          /* Info("HoverTile RuleId(%d) HashValue(%u)", HoverTile->RuleId, HoverTile->HashValue); */
          /* Info(" BakeTile RuleId(%d) HashValue(%u)", BakeTile->RuleId, BakeTile->HashValue); */

          tile_ruleset *Rule = Get(&Rules, HoverTile->RuleId);

          DebugChars("+x rule count(%u)(", CountBitsSet_Kernighan(Rule->E[0])); PrintBinary(Rule->E[0]); DebugLine(")");
          DebugChars("-x rule count(%u)(", CountBitsSet_Kernighan(Rule->E[1])); PrintBinary(Rule->E[1]); DebugLine(")");
          DebugChars("+y rule count(%u)(", CountBitsSet_Kernighan(Rule->E[2])); PrintBinary(Rule->E[2]); DebugLine(")");
          DebugChars("-y rule count(%u)(", CountBitsSet_Kernighan(Rule->E[3])); PrintBinary(Rule->E[3]); DebugLine(")");
          DebugChars("+z rule count(%u)(", CountBitsSet_Kernighan(Rule->E[4])); PrintBinary(Rule->E[4]); DebugLine(")");
          DebugChars("-z rule count(%u)(", CountBitsSet_Kernighan(Rule->E[5])); PrintBinary(Rule->E[5]); DebugLine(")");

          v3i VoxBaseP = V3iFromIndex(s32(BakeTile->VoxelIndex), BakeTile->SrcChunk->Dim);
          v3 EntityBasis = GetRenderP(World->ChunkDim, E, Camera);

          aabb Rect = AABBMinDim(EntityBasis + VoxBaseP, Global_TileDim);
          DEBUG_DrawAABB(&GpuMap->Buffer, Rect, RED, 0.2f);
        }
      }
    }

  }
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/voxel_synthesis_rule_baker/assets");

  world_position WorldCenter = {{2,2,2}};
  canonical_position CameraTargetP = {};

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f, CameraTargetP);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  random_series WorldEntropy = {54930695483};
  WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

  GameState->CameraTarget = GetFreeEntity(EntityTable);
  SpawnEntity(GameState->CameraTarget);
  GameState->CameraTarget->P = Canonical_Position(Voxel_Position(0), {{0,0,0}});
  Resources->CameraTargetP = &GameState->CameraTarget->P;

  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/test2.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/square.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/happy_square.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/square_expanded.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/archway.vox"); */
  GameState->BakeResult = BakeVoxelSynthesisRules("models/archway_with_floor.vox");

  memory_arena *TempMemory = AllocateArena();

  tile_ruleset_buffer Rules = GameState->BakeResult.Rules;
  voxel_synth_tile_buffer BakedTiles = GameState->BakeResult.Tiles;
  vox_data *VoxData = &GameState->BakeResult.VoxData;
  chunk_data *ChunkData = VoxData->ChunkData;

  entity *BakeEntity = GetFreeEntity(EntityTable);
  GameState->BakeEntity = BakeEntity;

  BakeEntity->CollisionVolumeRadius = ChunkData->Dim/2.f;
  BakeEntity->P = Canonical_Position(World->ChunkDim, V3(-ChunkData->Dim.x-5, 0, 0), V3i(0,0,0));
  AllocateAndBuildMesh(&GameState->BakeResult.VoxData, &BakeEntity->Model, TempMemory, Resources->Memory);

#define DO_WORLD_SYNTHESIS 0

#if !DO_WORLD_SYNTHESIS
  SpawnEntity(BakeEntity);
#endif
  Info("Drawing (%d) Baked tiles", BakedTiles.Count);

  for (u32 RuleIndex = 0; RuleIndex < Rules.Count; ++RuleIndex)
  {
    for (u32 SynthTileIndex = 0; SynthTileIndex < BakedTiles.Count; ++SynthTileIndex)
    {
      voxel_synth_tile *Tile = Get(&BakedTiles, SynthTileIndex);
      if (RuleIndex == Tile->RuleId)
      {
        v3i VoxOffset = V3iFromIndex(s32(Tile->VoxelIndex), ChunkData->Dim);

        entity *TileEntity = GetFreeEntity(EntityTable);
        TileEntity->CollisionVolumeRadius = V3(Global_TileDim/2);

        // TODO(Jesse)(memory, heap, mesh)
        AllocateMesh( &TileEntity->Model.Mesh, u32(Kilobytes(4)), Memory);

        BuildWorldChunkMeshFromMarkedVoxels_Greedy( ChunkData->Voxels, ChunkData->Dim,
                                                    VoxOffset, VoxOffset+Global_TileDim,
                                                    &TileEntity->Model.Mesh,
                                                    TempMemory,
                                                    VoxData->Palette );

        TileEntity->P.Offset += V3(s32(RuleIndex)*(Global_TileDim.x+2), 0, 0 );
        TileEntity->P = Canonicalize(World->ChunkDim, TileEntity->P);
#if !DO_WORLD_SYNTHESIS
        SpawnEntity(TileEntity);
#endif
        TileEntity->UserData = (void*)Tile;
        break;
      }
    }

  }

#if DO_WORLD_SYNTHESIS
  v3i TileSuperpositionsDim = World->VisibleRegion*World->ChunkDim / Global_TileDim;
  Assert(World->VisibleRegion*World->ChunkDim  % Global_TileDim == V3i(0));

  s32 TileSuperpositionCount = Volume(TileSuperpositionsDim);
  u64 *TileSuperpositions = Allocate(u64, Memory, TileSuperpositionCount);

  GameState->TileSuperpositionsDim = TileSuperpositionsDim;
  GameState->TileSuperpositions = TileSuperpositions;

  random_series Entropy = {5432956432};
  InitializeWorld_VoxelSynthesis( World, World->VisibleRegion, Global_TileDim, &Entropy,
                                  GameState->BakeResult.MaxTileEntropy,
                                  &GameState->BakeResult.Rules,
                                  TileSuperpositionsDim, TileSuperpositions);
#endif

#if 0
  for (u32 SynthTileIndex = 0; SynthTileIndex < BakedTiles.Count; ++SynthTileIndex)
  {
    voxel_synth_tile *Tile = Get(&BakedTiles, SynthTileIndex);
    v3i VoxOffset = V3iFromIndex(s32(Tile->VoxelIndex), ChunkData->Dim);

    entity *TileEntity = GetFreeEntity(EntityTable);
    TileEntity->CollisionVolumeRadius = V3(Global_TileDim/2);

    // TODO(Jesse)(memory, heap, mesh)
    AllocateMesh( &TileEntity->Model.Mesh, u32(Kilobytes(4)), Memory);

    BuildWorldChunkMeshFromMarkedVoxels_Greedy( ChunkData->Voxels, ChunkData->Dim,
                                                VoxOffset, VoxOffset+Global_TileDim,
                                                &TileEntity->Model.Mesh,
                                                TempMemory,
                                                VoxData->Palette );

    TileEntity->P.Offset += V3(s32(SynthTileIndex)*(Global_TileDim.x+8), 0, 0 );
    TileEntity->P = Canonicalize(World->ChunkDim, TileEntity->P);
    SpawnEntity(TileEntity);
    TileEntity->UserData = (void*)Tile;
  }
#endif

  VaporizeArena(TempMemory);

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}
