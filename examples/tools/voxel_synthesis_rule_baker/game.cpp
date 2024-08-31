#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include "game_constants.h"
#include "game_types.h"

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  b32 Result = True;
  engine_resources *EngineResources = GetEngineResources();
  world *World = EngineResources->World;
  game_state *GameState = EngineResources->GameState;

  work_queue_entry_type Type = Entry->Type;
  switch (Type)
  {
    InvalidCase(type_work_queue_entry_noop);
    InvalidCase(type_work_queue_entry__align_to_cache_line_helper);

    default: { Result = False; } break;

    case type_work_queue_entry_init_world_chunk:
    {
      work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;

      Assert(Chunk->DEBUG_OwnedByThread == 0);
      Chunk->DEBUG_OwnedByThread = ThreadLocal_ThreadIndex;

      if (GameState->GenInfo.TileSuperpositions == 0) break;

      if (ChunkIsGarbage(Chunk))
      {
        Chunk->Flags = Chunk_Deallocate;
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
          voxel_synthesis_gen_info *GenInfo = &GameState->GenInfo;
          auto AllTiles = &GameState->BakeResult.Tiles;
          /* auto BakeSrcVoxels = GenInfo->VoxData.ChunkData->Voxels; */
          /* auto BakeSrcVoxelsDim = GenInfo->VoxData.ChunkData->Dim; */

          MinMaxIterator(xTile, yTile, zTile, AbsTileMin, AbsTileMax)
          {
            v3i TileP = V3i(xTile, yTile, zTile);
            s32 TileIndex = GetIndex(TileP, BakeResult->TileSuperpositionsDim);
            tile_rule TileRule = GenInfo->TileSuperpositions[TileIndex];

            u32 TileOptionsCount = CountOptions(&TileRule);
            if (TileOptionsCount)
            {
              if (TileOptionsCount == 1)
              {
                tile_rule_id RuleId = GetRuleId(&TileRule);
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

                if (Match)
                {
                  Assert(Match);
                  Assert(Match->RuleId == RuleId);
                  /* Assert(TileRule & (1<<RuleId)); */

                  /* v3i SrcVoxMin = V3iFromIndex(s32(Match->VoxelIndex), Match->SrcChunk->Dim ); */
                  /* v3i SrcVoxMax = SrcVoxMin + TileDim; */

                  /* Assert(Chunk->FilledCount == 0); */
                  DimIterator(xTileVox, yTileVox, zTileVox, Global_TileDim)
                  {
                    v3i TileVox = V3i(xTileVox, yTileVox, zTileVox);
                    /* v3i SrcVox = SrcVoxMin + TileVox; */
                    /* s32 SrcVoxIndex = GetIndex(SrcVox, BakeSrcVoxelsDim); */
                    s32 SrcVoxIndex = GetIndex(TileVox, Global_TileDim);

                    v3i DestVox = (TileVox+(TileP*Global_TileDim)) % World->ChunkDim;
                    s32 DestVoxIndex = GetIndex(DestVox, World->ChunkDim);

                    Chunk->Voxels[DestVoxIndex] = Match->Voxels[SrcVoxIndex];

                    Chunk->FilledCount +=
                      (Chunk->Voxels[DestVoxIndex].Flags&Voxel_Filled) ?
                      1 :
                      0 ;
                  }
                }
                else
                {
#if 0
                  // NOTE(Jesse): Marks the null tiles
                  Assert(TileOptionsCount == 1);
                  Assert((TileRule.Pages[0]&1) == 1);
                  /* DimIterator(xTileVox, yTileVox, zTileVox, Global_TileDim) */
                  {
                    v3i TileVox = V3i(0, 0, 0);

                    v3i DestVox = (TileVox+(TileP*Global_TileDim)) % World->ChunkDim;
                    s32 DestVoxIndex = GetIndex(DestVox, World->ChunkDim);
                    Chunk->Voxels[DestVoxIndex] = { .Flags = Voxel_Filled, .Color = RED };
                    Chunk->FilledCount = 1;
                  }
#endif
                }
              }
              else
              {
#if 0
                // NOTE(Jesse): Fills undecided voxels with something to
                // indicate their level of indecision
                u32 TotalChoices = CountBitsSet_Kernighan(GameState->GenInfo.MaxTileEntropy);
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

          // TODO(Jesse): Redundant, remove
          /* Chunk->Flags = chunk_flag(Chunk->Flags | Chunk_VoxelsInitialized); */

          MarkBoundaryVoxels_MakeExteriorFaces(Chunk->Voxels, World->ChunkDim, {}, World->ChunkDim);

          /* ComputeStandingSpots( World->ChunkDim, Chunk, {}, {}, Global_StandingSpotDim, */
          /*                       World->ChunkDim, 0, &Chunk->StandingSpots, Thread->TempMemory); */

        }

      }


      FinalizeChunkInitialization(Chunk);
      QueueChunkForMeshRebuild(&EngineResources->Stdlib.Plat.HighPriority, Chunk);

      Chunk->DEBUG_OwnedByThread = 0;
    } break;

  }

  return Result;
}

link_internal s32
PickNewTileIndex(entropy_lists *EntropyLists, random_series *VoxelSynthesisEntropy, s32 LastTileIndex, v3i TileSuperpositionsDim)
{
#define TILE_INDEX_GENERATION_COMPLETE (s32_MAX)
  s32 Result = TILE_INDEX_GENERATION_COMPLETE;
  // NOTE(Jesse): For now, I never push data onto lists of tiles with 0 entropy (it is an error to have 0)
  // or tiles with 1 entropy (which are fully decided)
  /* Assert( CurrentCount(GetPtr(EntropyLists, 0)) == 0); */
  /* Assert( CurrentCount(GetPtr(EntropyLists, 1)) == 0); */

  auto LastTileP = V3(V3iFromIndex(LastTileIndex, TileSuperpositionsDim));

  r32 ClosestDistanceSq = f32_MAX;
  s32 ClosestTileIndex = TILE_INDEX_GENERATION_COMPLETE;
  /* IterateOverBackwards(EntropyLists, EntropyList, ListIdx) */
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

        /* Info("Chose new tile from EntropyIndex (%d)", ListIdx); */
        Assert(ClosestTileIndex < TILE_INDEX_GENERATION_COMPLETE);
        /* Ensure( Remove(EntropyList, u32(ClosestTileIndex) ) ); */
        Result = ClosestTileIndex;
        break;
      }

    }
  }

  // If the lists are all empty, we return TILE_INDEX_GENERATION_COMPLETE
  /* Assert(Result < Volume(TileSuperpositionsDim)); */
  return Result;
}

link_internal tile_rule
GetDefaultValueForTile(v3i TileP, v3i TileSuperpositionsDim, tile_rule MaxTileEntropy)
{
  tile_rule Result = MaxTileEntropy;
  if ( TileP.x == 0 ||
       TileP.y == 0 ||
       TileP.z == 0 ||
       TileP.x == TileSuperpositionsDim.x-1 ||
       TileP.y == TileSuperpositionsDim.y-1 ||
       TileP.z == TileSuperpositionsDim.z-1 )
  {
    Result = NullTileRule;
  }
  return Result;
}

// NOTE(Jesse): This approach is fundamentally broken.  For simple tilesets it
// works, but for complex tilesets the problem is the following:
//
// If we nuke an area from orbit, tiles in the area have contributed to the
// entropy outside of the area we're nuking which could be the thing why we're
// failing to solve.  What we actually need to do is, for each tile we're trying
// to reset, do the inverse of what the propagation does (or the options into
// the surrounding tiles instead of and-ing them) .. or something .. I think ..
#if 0
link_internal void
PartiallyResetVoxelSynthesisProgress( world *World,
                                      voxel_synthesis_result *BakeResult,
                                      v3i ResetMin,
                                      v3i ResetMax,
                                      voxel_synthesis_change_propagation_info_stack *ChangePropagationInfoStack,
                                      random_series *Entropy,
                                      memory_arena *TempMemory )
{
  Assert(ChangePropagationInfoStack->At == 0);

  tile_ruleset_buffer     *Rules                 = &BakeResult->Rules;
  entropy_lists *EntropyLists          = &BakeResult->EntropyLists;
  tile_rule                MaxTileEntropy        =  BakeResult->MaxTileEntropy;

  tile_rule *TileSuperpositions      = BakeResult->TileSuperpositions;
  v3i        TileSuperpositionsDim   = BakeResult->TileSuperpositionsDim;
  s32        TileSuperpositionsCount = Volume(TileSuperpositionsDim);

  u32 MaxTileOptions = CountOptions(&MaxTileEntropy);

  MinMaxIterator(xIndex, yIndex, zIndex, ResetMin, ResetMax)
  {
    v3i P = V3i(xIndex, yIndex, zIndex);
    s32 TileIndex = TryGetIndex(P, TileSuperpositionsDim);
    if (TileIndex > -1)
    {
      tile_rule *Rule = TileSuperpositions + TileIndex;
      {
        RemoveEntropyListEntry(EntropyLists, Rule, TileIndex, TileSuperpositions);
        *Rule = GetDefaultValueForTile(P, TileSuperpositionsDim, MaxTileEntropy);
        PushEntropyListEntry(EntropyLists, Rule, TileIndex, TileSuperpositions);
      }
    }
  }

  do
  {
    s32 PropagationResult = 0;
    ResetChangePropagationStack(ChangePropagationInfoStack);
    MinMaxIterator(xIndex, yIndex, zIndex, ResetMin-1, ResetMax+1)
    {
      v3i P = V3i(xIndex, yIndex, zIndex);
      s32 TileIndex = TryGetIndex(P, TileSuperpositionsDim);
      auto Tile = TileSuperpositions+TileIndex;
      if (TileIndex > -1) { PushDirectionsOntoStackForTile(ChangePropagationInfoStack, TileSuperpositionsDim,  TileIndex, Tile); }
      s32 LocalPropagationResult = PropagateChangesTo(ChangePropagationInfoStack, TileSuperpositionsDim, TileSuperpositions, Rules, EntropyLists);

      if (LocalPropagationResult == -1)
      {
        PropagationResult = -1;
        break;
      }
      else
      {
        PropagationResult += LocalPropagationResult;
      }
    }

    // We're finished when we propagated 0 results or we error
    if (PropagationResult == 0 || PropagationResult == -1) break;


    SanityCheckEntropyLists(EntropyLists, TileSuperpositions, TileSuperpositionsDim);
  } while (true);

  SanityCheckEntropyLists(EntropyLists, TileSuperpositions, TileSuperpositionsDim);
  /* return PropagationResult; */
}
#endif

link_internal void
PropagateInitialState(world *World, voxel_synthesis_result *BakeResult, voxel_synthesis_gen_info *InitInfo, random_series *Entropy, memory_arena *TempMemory)
{
  tile_ruleset_buffer *Rules            = &BakeResult->Rules;
  tile_rule MaxTileEntropy              =  BakeResult->MaxTileEntropy;
  v3i TileSuperpositionsDim             =  BakeResult->TileSuperpositionsDim;

  tile_rule *TileSuperpositions         =  InitInfo->TileSuperpositions;
  entropy_lists *EntropyLists = &InitInfo->EntropyLists;

  s32 TileSuperpositionsCount = Volume(TileSuperpositionsDim);
  IterateOver(EntropyLists, List, Idx) { Assert(List->At = List->Start); }

  u32 MaxTileOptions = CountOptions(&MaxTileEntropy);
  u32_cursor *EntropyList = GetPtr(EntropyLists, MaxTileOptions);
  EntropyList->At = EntropyList->Start;

  umm MaxStackDepth = umm(TileSuperpositionsCount) * 6;
  voxel_synthesis_change_propagation_info_stack ChangePropagationInfoStack = VoxelSynthesisChangePropagationInfoStack(MaxStackDepth, TempMemory);

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionsCount; ++TileIndex)
  {
    auto Tile = TileSuperpositions+TileIndex;
    auto P = V3iFromIndex(TileIndex, TileSuperpositionsDim);

    *Tile = GetDefaultValueForTile(P, TileSuperpositionsDim, MaxTileEntropy);
    PushEntropyListEntry(EntropyLists, Tile, TileIndex, TileSuperpositions);
    if (*Tile == NullTileRule) { PushDirectionsOntoStackForTile(&ChangePropagationInfoStack, TileSuperpositionsDim, TileIndex, Tile); }
  }
  SanityCheckEntropyLists(EntropyLists, TileSuperpositions, TileSuperpositionsDim);

  s32 Changes = PropagateChangesTo(&ChangePropagationInfoStack, TileSuperpositionsDim, TileSuperpositions, Rules, EntropyLists);
  Assert(Changes != -1);
}

link_internal void
InitializeWorkingStateFromBakeResult( world *World,
                                      /* v3i TileSuperpositionsDim, */
                                      s32 TileSuperpositionsCount,
                                      voxel_synthesis_gen_info *InitInfo,
                                      voxel_synthesis_gen_info *GenInfo,
                                      random_series *Entropy )
{
  /* v3i TileSuperpositionsDim             =  BakeResult->TileSuperpositionsDim; */
  tile_rule *TileSuperpositions         =  GenInfo->TileSuperpositions;
  entropy_lists *EntropyLists = &GenInfo->EntropyLists;


  {
    /* s32 TileSuperpositionsCount = Volume(TileSuperpositionsDim); */
    DeepCopy(&InitInfo->EntropyLists, &GenInfo->EntropyLists);
    // @memcopy_tile_superpositions
    MemCopy((u8*)InitInfo->TileSuperpositions, (u8*)GenInfo->TileSuperpositions, (umm)((umm)TileSuperpositionsCount*sizeof(tile_rule)));
  }

#if 0
  // Seed world
  //
  v3i TileP = RandomV3i(Entropy, TileSuperpositionsDim);
  TileP.z = 1;
  s32 TileIndex = GetIndex(TileP, TileSuperpositionsDim);

  for (u32 DirIndex = 0; DirIndex < ArrayCount(AllDirections); ++DirIndex)
  {
    v3i NextDir = AllDirections[DirIndex];
    auto Rule = TileSuperpositions + TileIndex;
    Push(&ChangePropagationInfoStack, VoxelSynthesisChangePropagationInfo(*Rule, TileP, NextDir));
  }

  Ensure( InitializeWorld_VoxelSynthesis_Partial( BakeResult, World, Global_TileDim, Entropy, TileIndex, &ChangePropagationInfoStack ) );
  SanityCheckEntropyLists(EntropyLists, TileSuperpositions, TileSuperpositionsDim);
#endif

  /* return TileIndex; */
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  memory_arena *Memory = Resources->GameMemory;

  v3i TileSuperpositionsDim = GameState->BakeResult.TileSuperpositionsDim;
  s32 TileSuperpositionsCount = Volume(GameState->BakeResult.TileSuperpositionsDim);

  voxel_synthesis_gen_info *GenInfo = &GameState->GenInfo;
  voxel_synthesis_gen_info *InitInfo = &GameState->InitInfo;

  entropy_lists *EntropyLists = &GenInfo->EntropyLists;
  tile_rule *TileSuperpositions         =  GenInfo->TileSuperpositions;

  local_persist random_series VoxelSynthesisEntropy = {543295643};
  umm MaxStackDepth = (umm)TileSuperpositionsCount * VoxelRuleDir_Count;
  local_persist v3i ResetMinTile;
  local_persist v3i ResetMaxTile;

  voxel_synthesis_result *BakeResult = &GameState->BakeResult;
  if (BakeResult->Errors == 0)
  {
    local_persist s32 OriginTileIndex = PickNewTileIndex(EntropyLists, &VoxelSynthesisEntropy, 0, BakeResult->TileSuperpositionsDim);
    local_persist s32 PrevTileIndex   = OriginTileIndex;
    local_persist s32 NextTileIndex   = OriginTileIndex;

    if (NextTileIndex != TILE_INDEX_GENERATION_COMPLETE && PrevTileIndex != TILE_INDEX_GENERATION_COMPLETE)
    {
      PrevTileIndex = NextTileIndex;
      NextTileIndex = PickNewTileIndex(EntropyLists, &VoxelSynthesisEntropy, OriginTileIndex, BakeResult->TileSuperpositionsDim);
    }

    u32 TilesToCollapsePerFrame = 1;
    local_persist b32 FatalError = False;
    local_persist s32 ErrorLevel = False;


    if (Input->Space.Clicked)
    {
      ErrorLevel = 0;
      InitializeWorkingStateFromBakeResult(World, TileSuperpositionsCount, &GameState->InitInfo, GenInfo, &VoxelSynthesisEntropy);
      OriginTileIndex = PickNewTileIndex(EntropyLists, &VoxelSynthesisEntropy, OriginTileIndex, BakeResult->TileSuperpositionsDim);
      NextTileIndex = OriginTileIndex;
      SanityCheckEntropyLists(EntropyLists, TileSuperpositions, TileSuperpositionsDim);
    }


    /* local_persist f32 SinceLast = 0.f; */
    /* SinceLast += Plat->dt; */

    if (FatalError == False)
    {
      /* SinceLast -= 0.1f; */
      voxel_synthesis_change_propagation_info_stack ChangePropagationInfoStack = VoxelSynthesisChangePropagationInfoStack(MaxStackDepth, GetTranArena());
      if ( InitializeWorld_VoxelSynthesis_Partial( BakeResult,
                                                   GenInfo,
                                                   Global_TileDim,
                                                  &VoxelSynthesisEntropy,
                                                   NextTileIndex,
                                                  &ChangePropagationInfoStack) )
      {
        /* ErrorLevel = Max(0, ErrorLevel-1); */
      }
      else
      {
        ++ ErrorLevel;
      }

      SanityCheckEntropyLists(EntropyLists, TileSuperpositions, TileSuperpositionsDim);

      v3i Radius = {}; //V3i(200) + V3i(2*ErrorLevel);
      Assert(ChangePropagationInfoStack.Min != V3i(s32_MAX));
      Assert(ChangePropagationInfoStack.Max != V3i(s32_MIN));
      ResetMinTile = ChangePropagationInfoStack.Min-Radius;
      ResetMaxTile = ChangePropagationInfoStack.Max+Radius;

      {
        u8 Color;
        if (ErrorLevel)
        {
          Color = RED;
          ResetChangePropagationStack(&ChangePropagationInfoStack);
          /* PartiallyResetVoxelSynthesisProgress(World, BakeResult, ResetMinTile, ResetMaxTile, &ChangePropagationInfoStack, &VoxelSynthesisEntropy, GetTranArena()); */
          InitializeWorkingStateFromBakeResult(World, TileSuperpositionsCount, InitInfo, GenInfo, &VoxelSynthesisEntropy);
          OriginTileIndex = PickNewTileIndex(EntropyLists, &VoxelSynthesisEntropy, OriginTileIndex, BakeResult->TileSuperpositionsDim);
          ErrorLevel = 0;
        }
        else
        {
          Color = GREEN;
        }

        if (NextTileIndex != TILE_INDEX_GENERATION_COMPLETE)
        {
          v3i TileP = V3iFromIndex(NextTileIndex, TileSuperpositionsDim);
          v3i MinVoxBaseP = ResetMinTile * Global_TileDim;
          v3i MaxVoxBaseP = ResetMaxTile * Global_TileDim;
          v3 MinRenderBaseP = GetRenderP(World->ChunkDim, V3(MinVoxBaseP), Camera);
          v3 MaxRenderBaseP = GetRenderP(World->ChunkDim, V3(MaxVoxBaseP), Camera);
          aabb TileRect = AABBMinMax(MinRenderBaseP, MaxRenderBaseP);
          DEBUG_DrawAABB(&GpuMap->Buffer, TileRect, Color, DEFAULT_LINE_THICKNESS*2.f);
        }
      }
    }

    /* if (ErrorLevel > 10) */
    /* { */
    /*   FatalError = True;; */
    /* } */


    if (PrevTileIndex != TILE_INDEX_GENERATION_COMPLETE)
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
            {
              TIMED_NAMED_BLOCK(GetWorldChunk);

              if (world_chunk *Chunk = GetWorldChunkFromHashtable( World, P ))
              {
                if ( Chunk->Flags & Chunk_Queued ) { continue; }
                Assert(Chunk->Flags & Chunk_VoxelsInitialized);

                /* DeallocateWorldChunk(&Plat->RenderQ, Chunk, MeshFreelist); */
                /* FreeWorldChunk(World, &Plat->RenderQ, Chunk, MeshFreelist); */

                s16 AnyHandlesQueued = {};
                RangeIterator(MeshIndex, MeshIndex_Count)
                {
                  AnyHandlesQueued |= Chunk->Meshes.GpuBufferHandles[MeshIndex].Flags;
                }

                if (AnyHandlesQueued) { continue; }

                local_persist random_series RNG = {654376587568};

                if (RandomBetween(0, &RNG, 100) > 90)
                {
                  DeallocateAndClearWorldChunk(Resources, Chunk);
                  Chunk->WorldP = P;
                  QueueChunkForInit(&Plat->HighPriority, Chunk, MeshBit_Lod0);
                }

/*                 Chunk->Flags = Chunk_Deallocate; */
/*                 world_chunk *NewChunk         = GetFreeWorldChunk(World); */
/*                              NewChunk->WorldP = P; */
/*                 QueueChunkForInit(&Plat->HighPriority, NewChunk, MeshBit_Lod0); */

              }
            }
          }
        }
      }
    }


    SanityCheckEntropyLists(EntropyLists, TileSuperpositions, TileSuperpositionsDim);
    if (NextTileIndex != TILE_INDEX_GENERATION_COMPLETE)
    {
      /* v3i ResetRadius = V3i(4); */
      /* PartiallyResetVoxelSynthesisProgress(World, BakeResult, ResetRadius, NextTileIndex, MaxStackDepth, &VoxelSynthesisEntropy, GetTranArena()); */
    }

    if (PrevTileIndex != TILE_INDEX_GENERATION_COMPLETE)
    {
      v3i TileP = V3iFromIndex(PrevTileIndex, BakeResult->TileSuperpositionsDim);

      v3i VoxBaseP = TileP * Global_TileDim;
      v3 VoxRenderBaseP = GetRenderP(World->ChunkDim, V3(VoxBaseP), Camera);
      aabb TileRect = AABBMinDim(VoxRenderBaseP, Global_TileDim);
      DEBUG_DrawAABB(&GpuMap->Buffer, TileRect, GREEN, DEFAULT_LINE_THICKNESS*2.f);
    }

    if (NextTileIndex != TILE_INDEX_GENERATION_COMPLETE)
    {
      v3i TileP = V3iFromIndex(NextTileIndex, BakeResult->TileSuperpositionsDim);

      v3i VoxBaseP = TileP * Global_TileDim;
      v3 VoxRenderBaseP = GetRenderP(World->ChunkDim, V3(VoxBaseP), Camera);
      aabb TileRect = AABBMinDim(VoxRenderBaseP, Global_TileDim);
      u8 Color = ErrorLevel > 0 ? RED : YELLOW;
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
          v3i TileP = V3iFromIndex(TmpTileIndex, BakeResult->TileSuperpositionsDim);

          {
            // DEBUG code
            u32 BitsSet = CountOptions(&GenInfo->TileSuperpositions[TmpTileIndex]);
            Assert(BitsSet == EntropyListIndex);
          }

          v3i VoxBaseP = TileP * Global_TileDim;
          v3 VoxRenderBaseP = GetRenderP(World->ChunkDim, V3(VoxBaseP), Camera);
          aabb TileRect = AABBMinDim(VoxRenderBaseP, Global_TileDim);
          /* DEBUG_DrawAABB(&GpuMap->Buffer, TileRect, BLUE); */
        }

        break;
      }
    }
  }

  entity *HoverEntity = 0;
  /* entity *HoverEntity = MousePickEntity(Resources); */
  entity *BakeEntity = GameState->BakeEntity;

  local_persist entity *MousePickedEntities[2] = {};
  if (Input->MMB.Clicked) { MousePickedEntities[0] = {}; MousePickedEntities[1] = {};}

  if (HoverEntity != BakeEntity)
  {
    if (MousePickedEntities[0]) { DrawEntityCollisionVolume(MousePickedEntities[0], &GpuMap->Buffer, Graphics, World->ChunkDim, ORANGE, 0.2f ); }
    if (MousePickedEntities[1]) { DrawEntityCollisionVolume(MousePickedEntities[1], &GpuMap->Buffer, Graphics, World->ChunkDim, ORANGE, 0.2f ); }

    if (Input->Enter.Clicked && MousePickedEntities[0] && MousePickedEntities[1])
    {
      voxel_synth_tile *Tile0 = (voxel_synth_tile*)MousePickedEntities[0]->UserData;
      voxel_synth_tile *Tile1 = (voxel_synth_tile*)MousePickedEntities[1]->UserData;
    }

    if (HoverEntity)
    {
      if (Input->LMB.Clicked)
      {
        if (MousePickedEntities[0] == 0) { MousePickedEntities[0] = HoverEntity; }
        else                             { MousePickedEntities[1] = HoverEntity; }
      }

      if      (HoverEntity == MousePickedEntities[0]) { DrawEntityCollisionVolume(HoverEntity, &GpuMap->Buffer, Graphics, World->ChunkDim, LIGHT_ORANGE, 0.3f ); }
      else if (HoverEntity == MousePickedEntities[1]) { DrawEntityCollisionVolume(HoverEntity, &GpuMap->Buffer, Graphics, World->ChunkDim, DARK_ORANGE,  0.3f ); }
      else                                            { DrawEntityCollisionVolume(HoverEntity, &GpuMap->Buffer, Graphics, World->ChunkDim, BLACK,        0.3f ); }
    }
  }

  {
    voxel_synth_tile *HoverTile = HoverEntity
      ? (voxel_synth_tile*)HoverEntity->UserData
      : 0;

    v3 EntityBasis = GetRenderP(World->ChunkDim, BakeEntity, Camera);

    voxel_synth_tile_buffer BakedTiles = BakeResult->Tiles;
    for (u32 BakedTileIndex = 0; BakedTileIndex < BakedTiles.Count; ++BakedTileIndex)
    {
      voxel_synth_tile *BakeTile = BakedTiles.Start+BakedTileIndex;

      v3i VoxBaseP = V3iFromIndex(s32(BakeTile->VoxelIndex), BakeTile->SrcChunk->Dim);
      aabb Rect = AABBMinDim(EntityBasis + VoxBaseP, Global_TileDim);

      voxel_synth_tile *MousePickedTile0 = MousePickedEntities[0] ? Cast(voxel_synth_tile*, MousePickedEntities[0]->UserData) : 0;
      voxel_synth_tile *MousePickedTile1 = MousePickedEntities[1] ? Cast(voxel_synth_tile*, MousePickedEntities[1]->UserData) : 0;

      if      (MousePickedTile0 && MousePickedTile0->RuleId == BakeTile->RuleId) { DEBUG_DrawAABB(&GpuMap->Buffer, Rect, LIGHT_ORANGE, 0.2f); }
      else if (MousePickedTile1 && MousePickedTile1->RuleId == BakeTile->RuleId) { DEBUG_DrawAABB(&GpuMap->Buffer, Rect, DARK_ORANGE,  0.2f); }
      else if (HoverTile        && HoverTile->RuleId        == BakeTile->RuleId) { DEBUG_DrawAABB(&GpuMap->Buffer, Rect, BLACK, 0.2f); }
    }
  }

}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->GameMemory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/voxel_synthesis_rule_baker/assets");


  /* world_position WorldCenter = {{2,2,2}}; */
  world_position WorldCenter = g_VisibleRegion/2;
  /* world_position WorldCenter = {{}}; */

  /* canonical_position CameraTargetP = Canonical_Position(V3(0), {{4,4,4}}); */
  /* canonical_position CameraTargetP = {}; */
  canonical_position CameraTargetP = Canonical_Position(V3(0), WorldCenter);

  StandardCamera(Graphics->Camera, 10000.0f, 1500.0f);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  random_series WorldEntropy = {54930695483};
  WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/test2.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/square.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/happy_square.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/square_expanded.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/archway.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/gravel_blocks.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/archway_with_floor.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/block_farm_degrading.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/grassy_block.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/grassy_block_2.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/simple_grass.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/pipes.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/random_squares.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("models/AncientTemple.vox"); */

  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu1.vox"); */

  // Hardmode
  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu3.vox"); */

  // GOOD
  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu4.vox"); */
  GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu5.vox");

  // Castle
  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu10.vox"); */

  // TOO MANY OPTIONS
  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu2.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu6.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu7.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu8.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu9.vox"); */
  /* GameState->BakeResult = BakeVoxelSynthesisRules("../voxel-model/vox/monument/monu16.vox"); */

  memory_arena *TempMemory = AllocateArena();
  DEBUG_REGISTER_ARENA(TempMemory, ThreadLocal_ThreadIndex);

  tile_ruleset_buffer    *Rulesets   = &GameState->BakeResult.Rules;
  voxel_synth_tile_buffer BakedTiles = GameState->BakeResult.Tiles;
  vox_data               *VoxData    = &GameState->BakeResult.VoxData;
  chunk_data             *ChunkData  = VoxData->ChunkData;

  cp BakeEntityP = Canonical_Position(World->ChunkDim, V3(-ChunkData->Dim.x-8, 0, 0), V3i(0));

  {
    maybe_asset_ptr MaybeAsset = AllocateAsset(Resources);
    if (MaybeAsset.Tag)
    {
      entity *BakeEntity = TryGetFreeEntityPtr(EntityTable);
      Assert(BakeEntity);

      GameState->BakeEntity = BakeEntity;

      BakeEntity->_CollisionVolumeRadius = ChunkData->Dim/2.f;
      BakeEntity->P = BakeEntityP;

      model *Model = Allocate(model, Resources->GameMemory, 1);
      AllocateAndBuildMesh(&GameState->BakeResult.VoxData, Model, TempMemory, Resources->GameMemory);

      asset *Asset = MaybeAsset.Value;
      Asset->LoadState = AssetLoadState_Loaded;
      Asset->Models.Start = Model;
      Asset->Models.Count = 1;

      BakeEntity->AssetId = Asset->Id;

      SpawnEntity(BakeEntity);
    }
  }


  Info("Drawing (%d) Baked tiles", BakedTiles.Count);

  for (u32 RuleIndex = 0; RuleIndex < Rulesets->Count; ++RuleIndex)
  {
    /* tile_ruleset *Ruleset = Get(Rulesets, RuleIndex); */
    tile_rule_id RuleId = GetRuleIdFromIndex(RuleIndex);
    for (u32 SynthTileIndex = 1; SynthTileIndex < BakedTiles.Count; ++SynthTileIndex)
    {
      voxel_synth_tile *Tile = Get(&BakedTiles, SynthTileIndex);
      if (RuleId == Tile->RuleId)
      {
        maybe_asset_ptr MaybeAsset = AllocateAsset(Resources);
        if (MaybeAsset.Tag)
        {
          asset *Asset = MaybeAsset.Value;
          Asset->LoadState = AssetLoadState_Loaded;

          model *Model = Allocate(model, Resources->GameMemory, 1);
          untextured_3d_geometry_buffer *Mesh = AllocateTempMesh(Resources->GameMemory, DataType_v3);
          AtomicReplaceMesh(&Model->Meshes, MeshBit_Lod0, Mesh, __rdtsc());

          Asset->Models.Start = Model;
          Asset->Models.Count = 1;

          v3i VoxOffset = V3iFromIndex(s32(Tile->VoxelIndex), ChunkData->Dim);

          entity *TileEntity = TryGetFreeEntityPtr(EntityTable);
          TileEntity->_CollisionVolumeRadius = V3(Global_TileDim/2);

          TileEntity->AssetId = MaybeAsset.Value->Id;

          /* BuildWorldChunkMeshFromMarkedVoxels_Greedy( ChunkData->Voxels, */
          /*                                             ChunkData->Dim, */
          /*                                             VoxOffset, VoxOffset+Global_TileDim, */
          /*                                            &TileEntity->Model.Mesh, */
          /*                                             TempMemory, */
          /*                                             VoxData->Palette ); */

          BuildWorldChunkMeshFromMarkedVoxels_Greedy_v3( ChunkData->Voxels,
                                                         ChunkData->Dim,
                                                         VoxOffset, VoxOffset+Global_TileDim,
                                                         Model->Meshes.E[0],
                                                         0,
                                                         GetTranArena() );


          /* BuildWorldChunkMesh_DebugVoxels( ChunkData->Voxels, */
          /*                                  ChunkData->Dim, */
          /*                                  VoxOffset, */
          /*                                  VoxOffset+Global_TileDim, */
          /*                                 &TileEntity->Model.Mesh, */
          /*                                  TempMemory, */
          /*                                  VoxData->Palette ); */

          TileEntity->P = BakeEntityP;
          TileEntity->P.WorldP += V3i(0, -1, 0);

          auto xRuleMultiplier = RuleIndex % 8;
          auto yRuleMultiplier = RuleIndex / 8;

          auto xOffset = s32(xRuleMultiplier)*(Global_TileDim.x+2);
          auto yOffset = -s32(yRuleMultiplier)*(Global_TileDim.x+2);

          TileEntity->P.Offset += V3(xOffset, yOffset, 0);
          /* TileEntity->P = Canonicalize(World->ChunkDim, TileEntity->P); */
          TileEntity->UserData = umm(Tile);

          SpawnEntity(TileEntity);
        }

        break;
      }
    }

  }

  memory_arena *Memory = Resources->GameMemory;

  v3i TileSuperpositionsDim = World->VisibleRegion*World->ChunkDim / Global_TileDim;
  Assert(World->VisibleRegion*World->ChunkDim  % Global_TileDim == V3i(0));

  s32 TileSuperpositionsCount = Volume(TileSuperpositionsDim);
  tile_rule *TileSuperpositions = Allocate(tile_rule, Memory, TileSuperpositionsCount);

  GameState->BakeResult.TileSuperpositionsDim = TileSuperpositionsDim;
  GameState->InitInfo.TileSuperpositions = TileSuperpositions;

  // Initialize EntropyLists
  {
    entropy_lists *EntropyLists = &GameState->InitInfo.EntropyLists;
    RangeIterator(EntropyListIndex, (s32)TILE_RULESETS_COUNT)
    {
      GetPtr(EntropyLists, u32(EntropyListIndex))[0] = U32Cursor(umm(TileSuperpositionsCount), Memory);
    }
  }

  {
    entropy_lists *EntropyLists = &GameState->GenInfo.EntropyLists;
    RangeIterator(EntropyListIndex, (s32)TILE_RULESETS_COUNT)
    {
      GetPtr(EntropyLists, u32(EntropyListIndex))[0] = U32Cursor(umm(TileSuperpositionsCount), Memory);
    }
    GameState->GenInfo.TileSuperpositions = Allocate(tile_rule, Memory, TileSuperpositionsCount);
  }

  // TODO(Jesse): Feels weird to use world entropy for this..?
  PropagateInitialState(World, &GameState->BakeResult, &GameState->InitInfo, &WorldEntropy, TempMemory);
  InitializeWorkingStateFromBakeResult(World, TileSuperpositionsCount, &GameState->InitInfo, &GameState->GenInfo, &WorldEntropy);

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
