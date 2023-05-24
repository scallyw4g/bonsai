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

/* link_internal s32 */
/* PushEntropyListValue(u32_cursor_staticbuffer *EntropyLists, random_series *VoxelSynthesisEntropy) */
/* { */
/* } */

link_internal s32
PickNewTileIndex(u32_cursor_staticbuffer *EntropyLists, random_series *VoxelSynthesisEntropy, s32 LastTileIndex, v3i TileSuperpositionsDim)
{
  s32 Result = s32_MAX;
  // NOTE(Jesse): For now, I never push data onto lists of tiles with 0 entropy (it is an error to have 0)
  // or tiles with 1 entropy (which are fully decided)
  Assert( CurrentCount(GetPtr(EntropyLists, 0)) == 0);
  /* Assert( CurrentCount(GetPtr(EntropyLists, 1)) == 0); */

  auto LastTileP = V3(V3iFromIndex(LastTileIndex, TileSuperpositionsDim));

  r32 ClosestDistanceSq = f32_MAX;
  s32 ClosestTileIndex = s32_MAX;
  IterateOver(EntropyLists, EntropyList, ListIdx)
  {
    if (ListIdx > 1)
    {
      umm ListCount = CurrentCount(EntropyList);
      if (ListCount)
      {
        IterateOver(EntropyList, Element, ElementIndex)
        {
          s32 TestTileIndex = s32(Get(EntropyList, ElementIndex));
          auto P = V3(V3iFromIndex(TestTileIndex, TileSuperpositionsDim));
          r32 DistanceSq = LengthSq(LastTileP - P);
          if (DistanceSq < ClosestDistanceSq)
          {
            ClosestDistanceSq = DistanceSq;
            ClosestTileIndex = TestTileIndex;
          }

        }

        Info("Chose new tile from EntropyIndex (%d)", ListIdx);
        Assert(ClosestTileIndex < s32_MAX);
        Ensure( Remove(EntropyList, u32(ClosestTileIndex) ) );
        Result = ClosestTileIndex;
        break;
      }

    }
  }

  // If the lists are all empty, we return s32_MAX
  /* Assert(Result < Volume(TileSuperpositionsDim)); */
  return Result;
}

link_internal s32
ResetVoxelSynthesisProgress(world *World, voxel_synthesis_result *BakeResult, random_series *Entropy, memory_arena *TempMemory)
{
  u64 MaxTileEntropy                    = BakeResult->MaxTileEntropy;
  v3i TileSuperpositionsDim             = BakeResult->TileSuperpositionsDim;
  u64 *TileSuperpositions               = BakeResult->TileSuperpositions;
  u32_cursor_staticbuffer *EntropyLists = &BakeResult->EntropyLists;

  s32 TileSuperpositionsCount = Volume(TileSuperpositionsDim);
  IterateOver(EntropyLists, List, Idx)
  {
    List->At = List->Start;
  }

  u32 MaxBitCount = CountBitsSet_Kernighan(MaxTileEntropy);
  u32_cursor *EntropyList = GetPtr(EntropyLists, MaxBitCount);
  EntropyList->At = EntropyList->Start;

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionsCount; ++TileIndex)
  {
    TileSuperpositions[TileIndex] = MaxTileEntropy;
    Push(EntropyList, u32(TileIndex));
  }

  // Seed world
  umm MaxStackDepth = umm(TileSuperpositionsCount);
  voxel_synthesis_change_propagation_info_stack ChangePropagationInfoStack = VoxelSynthesisChangePropagationInfoStack(MaxStackDepth, TempMemory);
  v3i TileP = RandomV3i(Entropy, TileSuperpositionsDim);
  TileP.z = 8;
  s32 TileIndex = GetIndex(TileP, TileSuperpositionsDim);
  /* s32 TileIndex = RandomBetween(0, Entropy, TileSuperpositionsCount); */
  /* s32 BitIndex = RandomBetween(0, Entropy, s32(CountBitsSet_Kernighan(MaxTileEntropy))); */

  Ensure( Remove(GetPtr(EntropyLists, MaxBitCount), u32(TileIndex)) );
  Ensure( InitializeWorld_VoxelSynthesis_Partial( BakeResult, World, Global_TileDim, Entropy, TileIndex, &ChangePropagationInfoStack ) );

  return TileIndex;
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

  s32 TileSuperpositionsCount = Volume(GameState->BakeResult.TileSuperpositionsDim);
  u32_cursor_staticbuffer *EntropyLists = &GameState->BakeResult.EntropyLists;
  local_persist random_series VoxelSynthesisEntropy = {543295643};

  local_persist s32 OriginTileIndex = ResetVoxelSynthesisProgress(World, &GameState->BakeResult, &VoxelSynthesisEntropy, GetTranArena());;
  local_persist s32 PrevTileIndex = OriginTileIndex;
  local_persist s32 NextTileIndex = PickNewTileIndex(EntropyLists, &VoxelSynthesisEntropy, 0, GameState->BakeResult.TileSuperpositionsDim);

  u32 TilesToCollapsePerFrame = 1;
  local_persist b32 Error = False;


  if (Input->Space.Clicked)
  {
    Error = False;
    OriginTileIndex = ResetVoxelSynthesisProgress(World, &GameState->BakeResult, &VoxelSynthesisEntropy, GetTranArena());;
    NextTileIndex = PickNewTileIndex(EntropyLists, &VoxelSynthesisEntropy, OriginTileIndex, GameState->BakeResult.TileSuperpositionsDim);
  }


  if (Resources->FrameIndex % 5 == 0)
  {
    umm MaxStackDepth = (umm)TileSuperpositionsCount;
    voxel_synthesis_change_propagation_info_stack ChangePropagationInfoStack = VoxelSynthesisChangePropagationInfoStack(MaxStackDepth, GetTranArena());
    if ( InitializeWorld_VoxelSynthesis_Partial( &GameState->BakeResult, World,
                                                  Global_TileDim,
                                                  &VoxelSynthesisEntropy,
                                                  NextTileIndex,
                                                 &ChangePropagationInfoStack) == False )
    {
      /* SoftError("Partial update failed."); */
      Error = True;
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

    if (!Error)
    {
      if (NextTileIndex != s32_MAX && PrevTileIndex != s32_MAX)
      {
        PrevTileIndex = NextTileIndex;
        NextTileIndex = PickNewTileIndex(EntropyLists, &VoxelSynthesisEntropy, OriginTileIndex, GameState->BakeResult.TileSuperpositionsDim);
      }
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
    u8 Color = Error ? RED : YELLOW;
    DEBUG_DrawAABB(&GpuMap->Buffer, TileRect, Color, DEFAULT_LINE_THICKNESS*2.f);
  }

  IterateOver(EntropyLists, EntropyList, EntropyListIndex)
  {
    umm EntropyEntryCount = CurrentCount(EntropyList);
    if (EntropyListIndex > 1 &&
        EntropyEntryCount &&
        EntropyEntryCount < TotalElements(EntropyList)-1) // Hack to stop drawing a screenful of AABBs on the first pass
    {
      RangeIterator(TileIndexIndex, (s32)EntropyEntryCount)
      {
        s32 TmpTileIndex = (s32)Get(EntropyList, (umm)TileIndexIndex);
        v3i TileP = V3iFromIndex(TmpTileIndex, GameState->BakeResult.TileSuperpositionsDim);

        {
          // DEBUG code
          u32 BitsSet = CountBitsSet_Kernighan(GameState->BakeResult.TileSuperpositions[TmpTileIndex]);
          Assert(BitsSet == EntropyListIndex);
        }

        v3i VoxBaseP = TileP * Global_TileDim;
        v3 VoxRenderBaseP = GetRenderP(World->ChunkDim, V3(VoxBaseP), Camera);
        aabb TileRect = AABBMinDim(VoxRenderBaseP, Global_TileDim);
        DEBUG_DrawAABB(&GpuMap->Buffer, TileRect, BLUE);
      }

      break;
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


  world_position WorldCenter = {{2,2,2}};
  /* world_position WorldCenter = {{}}; */

  canonical_position CameraTargetP = Canonical_Position(V3(0), {{4,4,4}});

  StandardCamera(Graphics->Camera, 10000.0f, 700.0f, CameraTargetP);

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
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/block_farm_degrading.vox"); */
  GameState->BakeResult = BakeVoxelSynthesisRules("models/grassy_block.vox");

  memory_arena *TempMemory = AllocateArena();
  DEBUG_REGISTER_ARENA(TempMemory, ThreadLocal_ThreadIndex);

  tile_ruleset_buffer *Rules         = &GameState->BakeResult.Rules;
  voxel_synth_tile_buffer BakedTiles = GameState->BakeResult.Tiles;
  vox_data *VoxData                  = &GameState->BakeResult.VoxData;
  chunk_data *ChunkData              = VoxData->ChunkData;

  entity *BakeEntity = GetFreeEntity(EntityTable);
  GameState->BakeEntity = BakeEntity;

  BakeEntity->CollisionVolumeRadius = ChunkData->Dim/2.f;
  BakeEntity->P = Canonical_Position(World->ChunkDim, V3(0), V3i(5,0,0));
  AllocateAndBuildMesh(&GameState->BakeResult.VoxData, &BakeEntity->Model, TempMemory, Resources->Memory);

  SpawnEntity(BakeEntity);

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

        TileEntity->P = BakeEntity->P;
        TileEntity->P.WorldP += V3i(0, -1, 0);

        auto xRuleMultiplier = RuleIndex % 8;
        auto yRuleMultiplier = RuleIndex / 8;

        auto xOffset = s32(xRuleMultiplier)*(Global_TileDim.x+2);
        auto yOffset = -s32(yRuleMultiplier)*(Global_TileDim.x+2);

        TileEntity->P.Offset += V3(xOffset, yOffset, 0);
        TileEntity->P = Canonicalize(World->ChunkDim, TileEntity->P);
        SpawnEntity(TileEntity);

        TileEntity->UserData = (void*)Tile;
        break;
      }
    }

  }

  v3i TileSuperpositionsDim = World->VisibleRegion*World->ChunkDim / Global_TileDim;
  Assert(World->VisibleRegion*World->ChunkDim  % Global_TileDim == V3i(0));

  s32 TileSuperpositionsCount = Volume(TileSuperpositionsDim);
  u64 *TileSuperpositions = Allocate(u64, Memory, TileSuperpositionsCount);

  GameState->BakeResult.TileSuperpositionsDim = TileSuperpositionsDim;
  GameState->BakeResult.TileSuperpositions    = TileSuperpositions;

  u32_cursor_staticbuffer *EntropyLists = &GameState->BakeResult.EntropyLists;
  RangeIterator(EntropyListIndex, (s32)MAX_TILE_RULESETS)
  {
    GetPtr(EntropyLists, u32(EntropyListIndex))[0] = U32Cursor(umm(TileSuperpositionsCount), Memory);
  }


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
