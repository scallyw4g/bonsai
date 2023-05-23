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

      RebuildWorldChunkMesh(Thread, Chunk);
      FinalizeChunkInitialization(Chunk);
    } break;

    case type_work_queue_entry_init_world_chunk:
    {
      work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;

      if (GameState->BakeResult.TileSuperpositions == 0) return;

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
            s32 TileIndex = GetIndex(TileP, GameState->BakeResult.TileSuperpositionsDim);
            Assert(TileIndex < Volume(GameState->BakeResult.TileSuperpositionsDim));
            u64 TileRule = GameState->BakeResult.TileSuperpositions[TileIndex];

            if (TileRule != 0)
            {
              if (CountBitsSet_Kernighan(TileRule) == 1)
              {
                u32 RuleId = GetIndexOfNthSetBit(TileRule, 1);
                voxel_synth_tile *Match = 0;
                // TODO(Jesse): I think the RuleId corresponds to the index,
                // but I can't remember for certain right now.  This might be a
                // stright array access
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

                /* Assert(Chunk->FilledCount == 0); */
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

              }
              else
              {
#if 0
                // NOTE(Jesse): Fills undecided voxels with something to
                // indicate their level of indecision
                u32 TotalChoices = CountBitsSet_Kernighan(GameState->BakeResult.MaxTileEntropy);
                /* r32 Ratio = r32(CountBitsSet_Kernighan(TileRule))/r32(TotalChoices); */

                /* s32 NumToFill = s32(Ratio*r32(Volume(Global_TileDim))); */
                s32 NumToFill = s32(CountBitsSet_Kernighan(TileRule));

                DimIterator(xTileVox, yTileVox, zTileVox, Global_TileDim)
                {
                  if (NumToFill-- > 0)
                  {
                    v3i TileVox = V3i(xTileVox, yTileVox, zTileVox);

                    v3i DestVox = (TileVox+(TileP*Global_TileDim)) % World->ChunkDim;
                    s32 DestVoxIndex = GetIndex(DestVox, World->ChunkDim);

                    Chunk->Voxels[DestVoxIndex].Flags = Voxel_Filled;
                    Chunk->Voxels[DestVoxIndex].Color = u8(NumToFill);

                    Chunk->FilledCount +=
                      (Chunk->Voxels[DestVoxIndex].Flags&Voxel_Filled) ?
                      1 :
                      0 ;
                  }
                }
#endif

              }
            }
            else
            {
#if 1
              // NOTE(Jesse): Fill error tiles with solid RED
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

          Chunk->Flags = chunk_flag(Chunk->Flags | Chunk_VoxelsInitialized);

          MarkBoundaryVoxels_MakeExteriorFaces(Chunk->Voxels, World->ChunkDim, {}, World->ChunkDim);

          /* ComputeStandingSpots( World->ChunkDim, Chunk, {}, {}, Global_StandingSpotDim, */
          /*                       World->ChunkDim, 0, &Chunk->StandingSpots, Thread->TempMemory); */

        }

        RebuildWorldChunkMesh(Thread, Chunk);
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

link_internal s32
PickNewTileIndex(u32_cursor_staticbuffer *EntropyLists, random_series *VoxelSynthesisEntropy)
{
  s32 TileIndex = s32_MAX;
  // NOTE(Jesse): For now, I never push data onto lists of tiles with 0 entropy (it is an error to have 0)
  // or tiles with 1 entropy (which are fully decided)
  Assert( CurrentCount(GetPtr(EntropyLists,0)) == 0);
  Assert( CurrentCount(GetPtr(EntropyLists,1)) == 0);

  IterateOver(EntropyLists, EntropyList, Idx)
  {
    umm EntropyEntryCount = CurrentCount(EntropyList);
    if (EntropyEntryCount)
    {
      u32 Index = RandomBetween(0, VoxelSynthesisEntropy, u32(EntropyEntryCount) );
      TileIndex = s32(Get(EntropyList, Index));
      Ensure( Remove(EntropyList, u32(TileIndex) ) );
      break;
    }
  }

  return TileIndex;
}

link_internal void
ResetVoxelSynthesisProgress(u64 MaxTileEntropy, s32 TileSuperpositionCount, u64 *TileSuperpositions, u32_cursor_staticbuffer *EntropyLists)
{
  IterateOver(EntropyLists, List, Idx)
  {
    List->At = List->Start;
  }

  u32 MaxBitCount = CountBitsSet_Kernighan(MaxTileEntropy);
  for (s32 TileIndex = 0; TileIndex < TileSuperpositionCount; ++TileIndex)
  {
    TileSuperpositions[TileIndex] = MaxTileEntropy;
    u32_cursor *EntropyList = GetPtr(EntropyLists, MaxBitCount);
    Push(EntropyList, u32(TileIndex));
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

  u32_cursor_staticbuffer *EntropyLists = &GameState->BakeResult.EntropyLists;
  local_persist random_series VoxelSynthesisEntropy = {543295643};
  local_persist s32 PrevTileIndex = s32_MAX;
  local_persist s32 NextTileIndex = PickNewTileIndex(EntropyLists, &VoxelSynthesisEntropy);
  u32 TilesToCollapsePerFrame = 1;

  /* while (TilesToCollapsePerFrame > 0) */
  {
    /* if (NextTileIndex == s32_MAX) break; */

    /* --TilesToCollapsePerFrame; */


    /* if (Input->Space.Clicked) */
    if (Resources->FrameIndex % 10 == 0)
    {
      s32 TileSuperpositionCount = Volume(GameState->BakeResult.TileSuperpositionsDim);
      umm MaxStackDepth = (umm)TileSuperpositionCount;
      voxel_synthesis_change_propagation_info_stack InfoCursor = VoxelSynthesisChangePropagationInfoStack(MaxStackDepth, GetTranArena());
      if ( InitializeWorld_VoxelSynthesis_Partial( World, World->VisibleRegion, Global_TileDim, &VoxelSynthesisEntropy,
                                                      GameState->BakeResult.MaxTileEntropy,
                                                     &GameState->BakeResult.Rules,
                                                      GameState->BakeResult.TileSuperpositionsDim,
                                                      GameState->BakeResult.TileSuperpositions,
                                                      NextTileIndex,
                                                      EntropyLists,
                                                     &InfoCursor) == False )
      {
        SoftError("Partial update failed. Restarting..");
        ResetVoxelSynthesisProgress(GameState->BakeResult.MaxTileEntropy, TileSuperpositionCount, GameState->BakeResult.TileSuperpositions, EntropyLists);
      }

      if (PrevTileIndex != s32_MAX)
      {
        v3i Radius = World->VisibleRegion/2;
        v3i Min = World->Center - Radius;
        v3i Max = World->Center + Radius;
        for (s32 z = Min.z; z < Max.z; ++ z)
        {
          for (s32 y = Min.y; y < Max.y; ++ y)
          {
            for (s32 x = Min.x; x < Max.x; ++ x)
            {
              world_position P = World_Position(x,y,z);
              world_chunk *Chunk = 0;
              {
                TIMED_NAMED_BLOCK("GetWorldChunk");
                Chunk = GetWorldChunkFromHashtable( World, P );
                if (Chunk && !(Chunk->Flags & Chunk_Queued))
                {
                  Chunk->Flags = chunk_flag(Chunk->Flags & ~Chunk_VoxelsInitialized);
                  ClearWorldChunk(Chunk);
                  ZeroMemory( Chunk->Voxels, sizeof(voxel)*umm(Volume(Chunk->Dim)) );
                  Chunk->WorldP = P;
                  QueueChunkForInit(&Plat->LowPriority, Chunk);
                }
              }
            }
          }
        }
      }

      PrevTileIndex = NextTileIndex;
      NextTileIndex = PickNewTileIndex(EntropyLists, &VoxelSynthesisEntropy);
    }
  }



  if (PrevTileIndex != s32_MAX)
  {
    v3i TileP = V3iFromIndex(PrevTileIndex, GameState->BakeResult.TileSuperpositionsDim);

    v3i VoxBaseP = TileP * Global_TileDim;
    v3 VoxRenderBaseP = GetRenderP(World->ChunkDim, V3(VoxBaseP), Camera);
    aabb TileRect = AABBMinDim(VoxRenderBaseP, Global_TileDim);
    DEBUG_DrawAABB(&GpuMap->Buffer, TileRect, GREEN, DEFAULT_LINE_THICKNESS*2.f);
  }

  if (NextTileIndex != s32_MAX)
  {
    v3i TileP = V3iFromIndex(NextTileIndex, GameState->BakeResult.TileSuperpositionsDim);

    v3i VoxBaseP = TileP * Global_TileDim;
    v3 VoxRenderBaseP = GetRenderP(World->ChunkDim, V3(VoxBaseP), Camera);
    aabb TileRect = AABBMinDim(VoxRenderBaseP, Global_TileDim);
    DEBUG_DrawAABB(&GpuMap->Buffer, TileRect, YELLOW, DEFAULT_LINE_THICKNESS*2.f);
  }

  RangeIterator(EntropyListIndex, (s32)MAX_TILE_RULESETS)
  {
    u32_cursor *EntropyList = GetPtr(EntropyLists, (u32)EntropyListIndex);
    umm EntropyEntryCount = CurrentCount(EntropyList);
    if (EntropyEntryCount &&
        EntropyEntryCount < TotalElements(EntropyList)-1) // Hack to stop drawing a screenful of AABBs on the first pass
    {
      RangeIterator(TileIndexIndex, (s32)EntropyEntryCount)
      {
        s32 TmpTileIndex = (s32)Get(EntropyList, (umm)TileIndexIndex);
        v3i TileP = V3iFromIndex(TmpTileIndex, GameState->BakeResult.TileSuperpositionsDim);

        v3i VoxBaseP = TileP * Global_TileDim;
        v3 VoxRenderBaseP = GetRenderP(World->ChunkDim, V3(VoxBaseP), Camera);
        aabb TileRect = AABBMinDim(VoxRenderBaseP, Global_TileDim);
        DEBUG_DrawAABB(&GpuMap->Buffer, TileRect, BLUE);
      }
    }
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
        if ( BakeTile->RuleId == HoverTile->RuleId )
        {
          entity *E = GameState->BakeEntity;
          /* Info("HoverTile RuleId(%d) HashValue(%u)", HoverTile->RuleId, HoverTile->HashValue); */
          /* Info(" BakeTile RuleId(%d) HashValue(%u)", BakeTile->RuleId, BakeTile->HashValue); */
          /* DebugChars(" x rule count(%u)(", CountBitsSet_Kernighan(Rule->E[0])); PrintBinary(Rule->E[0]); DebugLine(")"); */
          /* DebugChars("-x rule count(%u)(", CountBitsSet_Kernighan(Rule->E[1])); PrintBinary(Rule->E[1]); DebugLine(")"); */
          /* DebugChars(" y rule count(%u)(", CountBitsSet_Kernighan(Rule->E[2])); PrintBinary(Rule->E[2]); DebugLine(")"); */
          /* DebugChars("-y rule count(%u)(", CountBitsSet_Kernighan(Rule->E[3])); PrintBinary(Rule->E[3]); DebugLine(")"); */
          /* DebugChars(" z rule count(%u)(", CountBitsSet_Kernighan(Rule->E[4])); PrintBinary(Rule->E[4]); DebugLine(")"); */
          /* DebugChars("-z rule count(%u)(", CountBitsSet_Kernighan(Rule->E[5])); PrintBinary(Rule->E[5]); DebugLine(")"); */

          tile_ruleset *Rule = Get(&Rules, HoverTile->RuleId);

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


#define DO_WORLD_SYNTHESIS 1
#if DO_WORLD_SYNTHESIS
  world_position WorldCenter = {{2,2,2}};
#else
  world_position WorldCenter = {{}};
#endif
  canonical_position CameraTargetP = {};

  StandardCamera(Graphics->Camera, 10000.0f, 200.0f, CameraTargetP);

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
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/gravel_blocks.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/archway_with_floor.vox"); */
  GameState->BakeResult = BakeVoxelSynthesisRules("models/block_farm_degrading.vox");

  memory_arena *TempMemory = AllocateArena();
  DEBUG_REGISTER_ARENA(TempMemory, ThreadLocal_ThreadIndex);

  tile_ruleset_buffer *Rules         = &GameState->BakeResult.Rules;
  voxel_synth_tile_buffer BakedTiles = GameState->BakeResult.Tiles;
  vox_data *VoxData                  = &GameState->BakeResult.VoxData;
  chunk_data *ChunkData              = VoxData->ChunkData;

  entity *BakeEntity = GetFreeEntity(EntityTable);
  GameState->BakeEntity = BakeEntity;

  BakeEntity->CollisionVolumeRadius = ChunkData->Dim/2.f;
  BakeEntity->P = Canonical_Position(World->ChunkDim, V3(-ChunkData->Dim.x-5, 0, 0), V3i(0,0,0));
  AllocateAndBuildMesh(&GameState->BakeResult.VoxData, &BakeEntity->Model, TempMemory, Resources->Memory);

#if !DO_WORLD_SYNTHESIS
  SpawnEntity(BakeEntity);
#endif
  Info("Drawing (%d) Baked tiles", BakedTiles.Count);

  for (u32 RuleIndex = 0; RuleIndex < Rules->Count; ++RuleIndex)
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

  GameState->BakeResult.TileSuperpositionsDim = TileSuperpositionsDim;
  GameState->BakeResult.TileSuperpositions    = TileSuperpositions;

  u32_cursor_staticbuffer *EntropyLists = &GameState->BakeResult.EntropyLists;
  RangeIterator(EntropyListIndex, (s32)MAX_TILE_RULESETS)
  {
    GetPtr(EntropyLists, u32(EntropyListIndex))[0] = U32Cursor(umm(TileSuperpositionCount), Memory);
  }

  ResetVoxelSynthesisProgress(GameState->BakeResult.MaxTileEntropy, TileSuperpositionCount, TileSuperpositions, EntropyLists);

#if 0
  // Partially initialize world, if you want
  umm MaxStackDepth = (umm)Volume(TileSuperpositionsDim);
  voxel_synthesis_change_propagation_info_stack InfoCursor = VoxelSynthesisChangePropagationInfoStack(MaxStackDepth, TempMemory);

  /* for (s32 yIndex = 0; yIndex < TileSuperpositionsDim.y; ++yIndex) */
  /* for (s32 xIndex = 0; xIndex < TileSuperpositionsDim.x; ++xIndex) */
  {
    v3i TileP = V3i(0, 0, 0);
    s32 TileIndex = GetIndex(TileP, TileSuperpositionsDim);
    TileSuperpositions[TileIndex] = 1;

    Ensure( Remove(GetPtr(EntropyLists, MaxBitCount), u32(TileIndex)) );
    RangeIterator(DirIndex, (s32)ArrayCount(AllDirections))
    {
      Push(&InfoCursor, VoxelSynthesisChangePropagationInfo(1, TileP, AllDirections[DirIndex]));
    }

    Ensure(PropagateChangesTo(&InfoCursor, TileSuperpositionsDim, TileSuperpositions, Rules, EntropyLists));
  }
#endif


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
