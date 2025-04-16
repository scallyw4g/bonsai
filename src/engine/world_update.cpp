link_export THREAD_MAIN_RETURN
WorldUpdateThread_Main(void *ThreadStartupParams)
{
  b32 InitResult = True;
  thread_startup_params *ThreadParams = Cast(thread_startup_params*, ThreadStartupParams);

  engine_resources *Engine = Cast(engine_resources*, ThreadParams->EngineResources);

  SetThreadLocal_ThreadIndex(ThreadParams->ThreadIndex);

  thread_local_state *Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);

  Assert(Global_EngineResources);
  Assert(Global_ThreadStates);

  while ( FutexNotSignaled(ThreadParams->WorkerThreadsExitFutex) )
  {
    UNPACK_ENGINE_RESOURCES(Engine);

    WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();
    WorkerThread_BeforeJobStart(ThreadParams);

    world_chunk_ptr_block_array UpdatedChunks = {0, 0, Thread->TempMemory};

    while (work_queue_entry *Entry = PopWorkQueueEntry(WorldUpdateQ))
    {
      tswitch(Entry)
      {
        case type_work_queue_entry_noop:
        case type_work_queue_entry_init_world_chunk:
        case type_work_queue_entry_copy_buffer_set:
        case type_work_queue_entry_copy_buffer_ref:
        case type_work_queue_entry_init_asset:
        case type_work_queue_entry_rebuild_mesh:
        case type_work_queue_entry_finalize_noise_values:
        case type_work_queue_entry_build_chunk_mesh:
        case type_work_queue_entry_sim_particle_system:
        case type_work_queue_entry__align_to_cache_line_helper:
        case type_work_queue_entry_async_function_call:
        case type_work_queue_entry__bonsai_render_command:
        {
          InvalidCodePath();
        } break;

#if 0
        case type_work_queue_entry_update_world_region:
        {
          work_queue_entry_update_world_region *Job = SafeAccess(work_queue_entry_update_world_region, Entry);
          DoWorldUpdate(&Engine->Stdlib.Plat.LowPriority, World, Thread, Job);

          RangeIterator_t(u32, ChunkIndex, Job->ChunkCount)
          {
            world_chunk *Chunk = Job->DestChunkBuffer[ChunkIndex];
            // Only push each chunk to have it's mesh rebuilt once, even if it
            // was updated multiple times.
            if (Chunk->Flags & Chunk_Queued)
            {
              UnSetFlag(&Chunk->Flags, Chunk_Queued);
              Push(&UpdatedChunks, &Chunk);
            }
          }


        } break;
#endif

      }

    }

    IterateOver(&UpdatedChunks, Chunk, ChunkIndex)
    {
      QueueChunkForMeshRebuild(&Plat->LowPriority, Chunk);
    }

    // NOTE(Jesse): This is intentionally after all the updates have completed
    // such that the UpdatedChunks block array persists between jobs and we
    // clear it at the end.
    RewindArena(GetTranArena());

    SleepMs(1);

    if (FutexIsSignaled(ThreadParams->WorkerThreadsExitFutex)) break;

    if (FutexIsSignaled(ThreadParams->WorkerThreadsSuspendFutex)) { WaitOnFutex(ThreadParams->WorkerThreadsSuspendFutex); }
  }


  Info("Exiting WorldUpdate Thread (%d)", ThreadParams->ThreadIndex);
  WaitOnFutex(ThreadParams->WorkerThreadsExitFutex);

  THREAD_MAIN_RETURN Result = ReinterpretCast(THREAD_MAIN_RETURN, InitResult);
  return Result;
}

link_internal void
QueueWorldUpdateForRegion( engine_resources *Engine,
                            world_edit_blend_mode  Mode,
                   world_edit_blend_mode_modifier  Modifier,
                           /* world_edit_shape *Shape, */
                                         v3  HSVColor,
                                        b32  PersistWhitespace,
                               memory_arena *Memory )
{
  TIMED_FUNCTION();

  NotImplemented;
#if 0
  // TODO(Jesse): Should we just remove the paramter?  Or do we sometimes not pass this?
  Assert(Memory == Engine->WorldUpdateMemory);

  UNPACK_ENGINE_RESOURCES(Engine);

  cp MinPCoarse = {};
  cp MaxPCoarse = {};

  // NOTE(Jesse): We have to expand by at least two in either direction because
  // we need at least one additional voxel to tell if we have a solid face, and
  // another one after that in case the last one on the exterior edge is
  // transparent and the next one is solid
  //
  // I can't remember why the MaxPStroke has to be one more, and I actually
  // think that it might not .
  f32 MinPStroke = 2.f;
  f32 MaxPStroke = 3.f;

  switch (Shape->Type)
  {
    InvalidCase(type_world_update_op_shape_params_noop);
    InvalidCase(type_world_update_op_shape_params_count);

    case type_world_update_op_shape_params_sphere:
    {
      auto *ShapeSphere = SafeCast(world_update_op_shape_params_sphere, Shape);
      cp P = ShapeSphere->Location;

      MinPCoarse = Canonicalize(World, P-V3(ShapeSphere->Radius+MinPStroke) - V3(Global_ChunkApronMinDim));
      MaxPCoarse = Canonicalize(World, P+V3(ShapeSphere->Radius+MaxPStroke) + V3(Global_ChunkApronMaxDim));
    } break;

    case type_world_update_op_shape_params_rect:
    {
      auto *ShapeRect = SafeCast(world_update_op_shape_params_rect, Shape);

#if 0
      MinPCoarse = ShapeRect->Region.Min;
      MaxPCoarse = ShapeRect->Region.Max;
#else
      v3 MinP = Min(ShapeRect->Region.Min, ShapeRect->Region.Max);
      v3 MaxP = Max(ShapeRect->Region.Min, ShapeRect->Region.Max);

      ShapeRect->Region.Min = MinP;
      ShapeRect->Region.Max = MaxP;

      MinPCoarse = SimSpaceToCanonical(World, MinP - V3(MinPStroke) - V3(Global_ChunkApronMinDim));
      MaxPCoarse = SimSpaceToCanonical(World, MaxP + V3(MaxPStroke) + V3(Global_ChunkApronMaxDim));
#endif
    } break;

    case type_world_update_op_shape_params_chunk_data:
    {
      auto *ShapeChunk = SafeCast(world_update_op_shape_params_chunk_data, Shape);

      v3 MinSimP = ShapeChunk->SimSpaceOrigin;
      v3 MaxSimP = MinSimP + ShapeChunk->Data.Dim;

      MinPCoarse = SimSpaceToCanonical(World, MinSimP-MinPStroke);
      MaxPCoarse = SimSpaceToCanonical(World, MaxSimP+MaxPStroke);
    } break;

    case type_world_update_op_shape_params_asset:
    {
      auto *ShapeAsset = SafeCast(world_update_op_shape_params_asset, Shape);

      v3 MinSimP = GetSimSpaceP(World, ShapeAsset->Origin);

      asset *Asset = GetAndLockAssetSync(GetEngineResources(), &ShapeAsset->AssetId);
      v3 MaxSimP = MinSimP + GetDimForAssetModel(Asset, u32(ShapeAsset->ModelIndex)).Value;

      UnlockAsset(Engine, Asset);

      MinPCoarse = SimSpaceToCanonical(World, MinSimP-MinPStroke);
      MaxPCoarse = SimSpaceToCanonical(World, MaxSimP+MaxPStroke);
    } break;
  }

  // These value align the min/max positions to StandingSpot boundaries in global space
  auto MinPFixup = V3i(MinPCoarse.Offset) % V3i(Global_StandingSpotDim.xy, 1);
  auto MaxPFixup = V3i(Global_StandingSpotDim.xy, 0) - V3i(MaxPCoarse.Offset) % V3i(Global_StandingSpotDim.xy, 1);

  Assert(MinPFixup.z == 0);
  Assert(MaxPFixup.z == 0);

  auto MinP = Canonicalize(World, MinPCoarse - V3(MinPFixup));
  auto MaxP = Canonicalize(World, MaxPCoarse + V3(MaxPFixup));

  MinP.Offset = Truncate(MinP.Offset);
  MaxP.Offset = Truncate(MaxP.Offset);

  Assert(u32(MinP.Offset.x) % u32(Global_StandingSpotDim.x) == 0 );
  Assert(u32(MinP.Offset.y) % u32(Global_StandingSpotDim.y) == 0 );

  Assert(u32(MaxP.Offset.x) % u32(Global_StandingSpotDim.x) == 0 );
  Assert(u32(MaxP.Offset.y) % u32(Global_StandingSpotDim.y) == 0 );

  /* { */
  /*   r32 Thickness = 0.15f; */
  /*   DEBUG_HighlightVoxel(Engine, MinP, BLUE, Thickness); */
  /*   DEBUG_HighlightVoxel(Engine, MaxP, RED, Thickness); */
  /* } */

  /* world_position Delta = Max(MaxP.WorldP - MinP.WorldP, World_Position(1)); */
  v3i Delta = MaxP.WorldP - MinP.WorldP + 1;
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
        world_chunk *Chunk = GetWorldChunkFromHashtable(World, ChunkP);
        if (Chunk && Chunk->Flags & Chunk_VoxelsInitialized)
        {
          Assert(Chunk->Flags != Chunk_Uninitialized);
          Assert(ChunkIndex < TotalChunkCount);
          Buffer[ChunkIndex++] = Chunk;
          SetFlag(Chunk, Chunk_Queued);
        }
      }
    }
  }

  // NOTE(Jesse): If none of the world chunks were in the hashtable yet we get ChunkCount == 0
  if (ChunkIndex > 0)
  {
    v3 SimFloodOrigin = GetHotVoxelForFlood(Engine, Mode, Modifier);

    work_queue_entry Entry = {
      .Type = type_work_queue_entry_update_world_region,
      .work_queue_entry_update_world_region = WorkQueueEntryUpdateWorldRegion(Mode, Modifier, SimFloodOrigin, Shape, HSVColor, PersistWhitespace, MinP, MaxP, Buffer, ChunkIndex),
    };
    PushWorkQueueEntry(&Plat->WorldUpdateQ, &Entry);
  }
#endif
}

link_internal u32
MapIntoQueryBox(v3i SimSpaceVoxP, v3i UpdateMinP, v3i UpdateDim)
{
  auto Rel = SimSpaceVoxP - UpdateMinP;
  auto Result = GetIndex(Rel, UpdateDim);
  return (u32)Result;
}

link_internal u32
MapIntoQueryBox(v3 SimSpaceVoxP, v3 UpdateMinP, v3i UpdateDim)
{
  v3 Rel = SimSpaceVoxP - UpdateMinP;
  s32 Result = GetIndex(Rel, UpdateDim);
  return (u32)Result;
}


link_internal void
DEBUG_AssertVoxelFloodStartsInEmptyVoxel(v3i SimSphereP, rect3i *SimSpaceUpdateBounds, voxel *CopiedVoxels)
{
  NotImplemented;
#if 0
#if BONSAI_INTERNAL
    v3i UpdateDim = GetDim(*SimSpaceUpdateBounds);

    { // NOTE(Jesse): Debug.  Don't rely on the optimizer to remove this in release mode
      //
      // This asserts that we're not trying to flood starting at a voxel that's inside the world.
      v3i RelVoxP = SimSphereP - SimSpaceUpdateBounds->Min;
      s32 VoxelIndex = TryGetIndex(RelVoxP, UpdateDim);
      if (VoxelIndex > -1)
      {
        voxel *OriginV  = CopiedVoxels+VoxelIndex;
        Assert( (OriginV->Flags&Voxel_Filled) == 0);
      }
    }
#endif
#endif
}








//
// NOTE(Jesse): UserCode gets called when the first
poof(
  func flood_fill_iteration_pattern(type_poof_symbol FloodPredicate, type_poof_symbol UserCode, type_poof_symbol UserCode2) @code_fragment
  {
    random_series ColorEntropy = {4654376543246};

    voxel *V = {};
    // TODO(Jesse): Do we want to try and keep the amount of temp memory to a minimum here?
    voxel_stack_element_cursor Stack = VoxelStackElementCursor(umm(TotalVoxels*6), Thread->TempMemory);

    // Unfortunately, can't #if this out in a poof function.  Should probably
    // put it on a #define switch to make sure it gets compiled out.
    DEBUG_AssertVoxelFloodStartsInEmptyVoxel(FloodOrigin, &SimSpaceUpdateBounds, CopiedChunk->Voxels);

    Push(&Stack, VoxelStackElement(FloodOrigin, VoxelRuleDir_Count));
    while (AtElements(&Stack))
    {
      b32 OverwriteVoxel = False;

      voxel_stack_element Element = Pop(&Stack);
      v3i SimVoxP = Element.VoxSimP + AllDirections[Element.Dir];
      v3i RelVoxP = SimVoxP - SimSpaceUpdateBounds.Min;

      s32 VoxelIndex = TryGetIndex(RelVoxP, UpdateDim);
      if (VoxelIndex > -1)
      {
        V = CopiedChunk->Voxels+VoxelIndex;

        v3i CenterToVoxP = SimVoxP - FloodOrigin;

        (FloodPredicate)
        {
          NotImplemented;
          /* if ( (V->Flags & Voxel_MarkBit) == 0) */
          {
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ));
          }
        }

        (UserCode)

          NotImplemented;
        /* V->Flags |= Voxel_MarkBit; */

        if ( ((OverwriteVoxel == True)  && (Invert == False)) ||
             ((OverwriteVoxel == False) && (Invert == True))  )
        {
          /* if (Mode == WorldEdit_Mode_Paint) */
          /* { */
          /*   V->Color = NewVoxelValue->Color; */
          /* } */
          /* else */
          {
            *V = *NewVoxelValue;
          }
        }
      }
    }

    Push(&Stack, VoxelStackElement(FloodOrigin, VoxelRuleDir_Count));
    while (AtElements(&Stack))
    {
      voxel_stack_element Element = Pop(&Stack);
      v3i Dir = AllDirections[Element.Dir];
      {
        v3i SimVoxP = Element.VoxSimP + Dir;
        v3i RelVoxP = SimVoxP - SimSpaceUpdateBounds.Min;

        s32 VoxelIndex = TryGetIndex(RelVoxP, UpdateDim);

        if (VoxelIndex > -1)
        {
          V = CopiedChunk->Voxels+VoxelIndex;

          (UserCode2)

          NotImplemented;
          /* if ( (V->Flags&Voxel_MarkBit)) */
          {
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ));
          }

          NotImplemented;
          /* V->Flags &= ~Voxel_MarkBit; */
        }
      }
    }
  }
)


poof(
  func rectalinear_iteration_pattern(type_poof_symbol UserCode) @code_fragment
  {
    DimIterator(x, y, z, UpdateDim)
    {
      b32 OverwriteVoxel = False;

      v3i VoxP = V3i(x,y,z);
      v3i SimVoxP = VoxP + SimSpaceUpdateBounds.Min;
      voxel *V = CopiedChunk->Voxels + GetIndex(VoxP, UpdateDim);

      if (Contains(SSRect, SimVoxP))
      {
        UserCode

        if ( ((OverwriteVoxel == True ) && (Invert == False)) ||
             ((OverwriteVoxel == False) && (Invert == True ))  )
        {
          /* if (Mode == WorldEdit_Mode_Paint) */
          /* { */
          /*   V->Color = NewVoxelValue->Color; */
          /* } */
          /* else */
          {
            if (Mode == WorldEdit_Mode_Subtractive) { *V = {}; }
            else { *V = *NewVoxelValue; }
          }

          // Knock out face flags so the 'surface' algorithm doesn't "self-apply"
          // We recompute these, so it's fine there.  It's slower on non-surface
          // paths, but .. when that's the bottleneck, we've won.
          NotImplemented;
          /* V->Flags = voxel_flag(V->Flags&~VoxelFaceMask); */
        }
      }
    }

  }
)

// TODO(Jesse): Make this not a macro.
#define poof_check_for_unfilled_border() NotImplemented

#if 0
  {                                                                    \
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(1,0,0)))  \
    {                                                                  \
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }      \
    }                                                                  \
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(-1,0,0))) \
    {                                                                  \
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }      \
    }                                                                  \
                                                                       \
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(0,1,0)))  \
    {                                                                  \
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }      \
    }                                                                  \
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(0,-1,0))) \
    {                                                                  \
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }      \
    }                                                                  \
                                                                       \
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(0,0,1)))  \
    {                                                                  \
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }      \
    }                                                                  \
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(0,0,-1))) \
    {                                                                  \
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }      \
    }                                                                  \
  }
#endif

#if 0
poof(
  func poof_check_for_unfilled_border(ignored)
  {
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(1,0,0)))
    {
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
    }
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(-1,0,0)))
    {
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
    }

    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(0,1,0)))
    {
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
    }
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(0,-1,0)))
    {
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
    }

    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(0,0,1)))
    {
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
    }
    if (voxel *Vn = TryGetVoxel(CopiedChunk, VoxP + V3(0,0,-1)))
    {
      if ((Vn->Flags&VoxelFaceMask)) { IsUnfilledBorder = True; }
    }
  }
)
#endif







//
//
// shape_sphere
//
//


link_internal void
WorldEdit_shape_sphere_Surface(apply_world_edit_params *Params, r32 RadiusSquared, v3i EditCenterP, voxel *NewVoxelValue)
{
  UNPACK_APPLY_WORLD_EDIT_PARAMS(Params);
#if 0
  switch (Mode)
  {
    case WorldEdit_Mode_Disabled: {} break;

    case WorldEdit_Mode_Additive:
    {
      poof(rectalinear_iteration_pattern({

        v3i CenterToVoxP = SimVoxP - EditCenterP;
        NotImplemented;
        /* if (LengthSq(CenterToVoxP) < RadiusSquared && (V->Flags&Voxel_Filled) == False) */
        {
          b32 IsUnfilledBorder = False;
          poof_check_for_unfilled_border();
          /* Assert(NewVoxelValue->Flags & Voxel_Filled); */
          if (IsUnfilledBorder)
          {
            OverwriteVoxel = True;
          }
        }
      }))
#include <generated/rectalinear_iteration_pattern_398799212.h>
    } break;

    /* case WorldEdit_Mode_Paint: */
    case WorldEdit_Mode_Subtractive:
    {
      poof(rectalinear_iteration_pattern({
        v3i CenterToVoxP = SimVoxP - EditCenterP;
        NotImplemented;
        /* if (LengthSq(CenterToVoxP) < RadiusSquared && (V->Flags&VoxelFaceMask)) */
        {
          OverwriteVoxel = True;
        }
      }))
#include <generated/rectalinear_iteration_pattern_920026661.h>
    } break;
  }
#endif
}

link_internal void
WorldEdit_shape_sphere_Flood(apply_world_edit_params *Params, thread_local_state *Thread, r32 RadiusSquared, v3i EditCenterP, v3i FloodOrigin, voxel *NewVoxelValue)
{
  UNPACK_APPLY_WORLD_EDIT_PARAMS(Params);
  s32 TotalVoxels = Volume(UpdateDim);

  Assert(Params->Mode == WorldEdit_Mode_Subtractive);

  poof(flood_fill_iteration_pattern(
    {
      NotImplemented;
      /* if (LengthSq(CenterToVoxP) < RadiusSquared && (V->Flags&Voxel_Filled) == 0) */
    },
    {
      NotImplemented;
      /* if ( Length(CenterToVoxP) < SquareRoot(RadiusSquared)-1.f && (V->Flags&Voxel_Filled) ) */
         /* { V->Flags = Voxel_Empty; } */
    },
    {
      v3i CenterToVoxP = SimVoxP - EditCenterP;
      if (Length(CenterToVoxP) < SquareRoot(RadiusSquared)-1.f)
      {
        NotImplemented;
        /* if (V->Flags & Voxel_Filled) */
        {
          V->Color = MagicaVoxelDefaultPaletteToPackedHSV(SafeTruncateU8(RandomBetween((u32)MCV_GREY_5, &ColorEntropy, (u32)MCV_GREY_8+1)));
        }

      }
      else if (LengthSq(CenterToVoxP) < RadiusSquared)
      {
        V->Color = MagicaVoxelDefaultPaletteToPackedHSV(MCV_GREY_8);
      }
    }
    ))
#include <generated/flood_fill_iteration_pattern_199741702_161749140_632272777.h>
}

link_internal void
WorldEdit_shape_sphere_Default(apply_world_edit_params *Params, r32 RadiusSquared, v3i EditCenterP, voxel *NewVoxelValue)
{
  UNPACK_APPLY_WORLD_EDIT_PARAMS(Params);

  poof(rectalinear_iteration_pattern({
    v3i CenterToVoxP = SimVoxP - EditCenterP;
    if (LengthSq(CenterToVoxP) < RadiusSquared)
    {
      OverwriteVoxel = True;
    }
  }))
#include <generated/rectalinear_iteration_pattern_812652930.h>
}


//
//
// shape_rect
//
//

link_internal void
WorldEdit_shape_rect_Surface(apply_world_edit_params *Params, voxel *NewVoxelValue)
{
  UNPACK_APPLY_WORLD_EDIT_PARAMS(Params);

#if 0
  switch (Mode)
  {
    case WorldEdit_Mode_Disabled: {} break;

    case WorldEdit_Mode_Additive:
    {
      poof(rectalinear_iteration_pattern({

        NotImplemented;
        /* if ((V->Flags&Voxel_Filled) == False) */
        {
          b32 IsUnfilledBorder = False;
          poof_check_for_unfilled_border();
          NotImplemented;
          /* Assert(NewVoxelValue->Flags & Voxel_Filled); */
          if (IsUnfilledBorder)
          {
            OverwriteVoxel = True;
          }
        }
      }))
#include <generated/rectalinear_iteration_pattern_643608995.h>
    } break;

    /* case WorldEdit_Mode_Paint: */
    case WorldEdit_Mode_Subtractive:
    {
      poof(rectalinear_iteration_pattern({
        NotImplemented;
        /* if ( (V->Flags&VoxelFaceMask)) */
        {
          OverwriteVoxel = True;
        }
      }))
#include <generated/rectalinear_iteration_pattern_530902269.h>
    } break;
  }
#endif
}

link_internal void
WorldEdit_shape_rect_Flood( apply_world_edit_params *Params, thread_local_state *Thread, v3i FloodOrigin, voxel *NewVoxelValue)
{
  UNPACK_APPLY_WORLD_EDIT_PARAMS(Params);
  s32 TotalVoxels = Volume(UpdateDim);

#if 0
  switch (Mode)
  {
    case WorldEdit_Mode_Disabled: {} break;

    /* case WorldEdit_Mode_Paint: */
    case WorldEdit_Mode_Additive:
    case WorldEdit_Mode_Subtractive:
    {
      poof(flood_fill_iteration_pattern(
        {
          NotImplemented;
          /* { if ( (V->Flags&Voxel_Filled) == (Voxel_Filled*(Mode==WorldEdit_Mode_Additive)) ) }, */
        },
        {
          NotImplemented;
          /* if ( Mode == WorldEdit_Mode_Additive && (V->Flags&Voxel_Filled) ) */
          /* { } */
          /* else */
          {
            OverwriteVoxel = True;
          }
        },
        {}
        ))
#include <generated/flood_fill_iteration_pattern_846291950_267608728_0.h>
    } break;
  }
#endif
}

link_internal void
WorldEdit_shape_rect_Default(apply_world_edit_params *Params, voxel *NewVoxelValue)
{
  UNPACK_APPLY_WORLD_EDIT_PARAMS(Params);
#if 0
  switch (Mode)
  {
    case WorldEdit_Mode_Disabled: {} break;
    case WorldEdit_Mode_Additive:
    case WorldEdit_Mode_Subtractive:
    {
      poof(rectalinear_iteration_pattern({
        OverwriteVoxel = True;
      }))
#include <generated/rectalinear_iteration_pattern_416827956.h>
    } break;

    /* case WorldEdit_Mode_Paint: */
    /* { */
    /*   poof(rectalinear_iteration_pattern({ */
    /*     NotImplemented; */
    /*     /1* if (V->Flags & Voxel_Filled) *1/ */
    /*     { */
    /*       OverwriteVoxel = True; */
    /*     } */
    /*   })) */
/* #include <generated/rectalinear_iteration_pattern_99934950.h> */
    /* } break; */
  }
#endif
}






//
//
// shape_chunk_data
//
//

link_internal void
WorldEdit_shape_chunk_data_Surface(apply_world_edit_params *Params, v3 SimOrigin, chunk_data *Data)
{
  UNPACK_APPLY_WORLD_EDIT_PARAMS(Params);

#if 0
  switch (Mode)
  {
    case WorldEdit_Mode_Disabled: {} break;

    case WorldEdit_Mode_Additive:
    {
      poof(rectalinear_iteration_pattern({

        v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
        voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP);

        NotImplemented;
        /* if ((V->Flags&Voxel_Filled)==False) */
        {
          b32 IsUnfilledBorder = False;
          poof_check_for_unfilled_border();
          if (IsUnfilledBorder)
          {
            OverwriteVoxel = True;
          }
        }
      }))
#include <generated/rectalinear_iteration_pattern_631222419.h>
    } break;

    /* case WorldEdit_Mode_Paint: */
    case WorldEdit_Mode_Subtractive:
    {
      poof(rectalinear_iteration_pattern({
        v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
        voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP);
        if (NewVoxelValue)
        {
          NotImplemented;
          /* if ( (V->Flags&VoxelFaceMask) && (NewVoxelValue->Flags&Voxel_Filled) ) */
          {
            OverwriteVoxel = True;
          }
        }
      }))
#include <generated/rectalinear_iteration_pattern_992879728.h>
    } break;
  }
#endif
}

link_internal void
WorldEdit_shape_chunk_data_Flood(
    apply_world_edit_params *Params,
    v3 SimOrigin, chunk_data *Data,
    thread_local_state *Thread,  v3i FloodOrigin
  )
{
  UNPACK_APPLY_WORLD_EDIT_PARAMS(Params);

  s32 TotalVoxels = Volume(UpdateDim);
#if 0
  switch (Mode)
  {
    case WorldEdit_Mode_Disabled: {} break;
    case WorldEdit_Mode_Additive:
    {
      poof(flood_fill_iteration_pattern(
        {
          v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
          voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP);
          NotImplemented;
          /* if ((V->Flags&Voxel_Filled)) */
        },
        {
          NotImplemented;
          /* if ( ((V->Flags&Voxel_Filled) == 0) && NewVoxelValue && (NewVoxelValue->Flags&Voxel_Filled)) { *V = *NewVoxelValue; } */
        },
        {}
        ))
#include <generated/flood_fill_iteration_pattern_275071431_785723886_0.h>
    } break;

    case WorldEdit_Mode_Subtractive:
    {
      poof(flood_fill_iteration_pattern(
        {
          v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
          voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP);
          NotImplemented;
          /* if (Contains(SSRect, SimVoxP) && (V->Flags&Voxel_Filled) == 0) */
        },
        {
          NotImplemented;
          /* if (NewVoxelValue && (NewVoxelValue->Flags&Voxel_Filled)) { *V = {}; } */
        },
        {}
        ))
#include <generated/flood_fill_iteration_pattern_275071431_101859599_0.h>
    } break;

    /* case WorldEdit_Mode_Paint: */
    /* { */
    /*   poof(rectalinear_iteration_pattern({ */
    /*     v3i OriginToCurrentVoxP = SimVoxP - SimOrigin; */
    /*     voxel *NewVoxelValue = TryGetVoxel(Data, OriginToCurrentVoxP); */
    /*     NotImplemented; */
    /*     /1* if (NewVoxelValue && (NewVoxelValue->Flags&Voxel_Filled)) { V->Color = NewVoxelValue->Color; } *1/ */
    /*   })) */
/* #include <generated/rectalinear_iteration_pattern_583358156.h> */
    /* } break; */

  }
#endif
}

link_internal void
WorldEdit_shape_chunk_data_Default(apply_world_edit_params *Params, v3 SimOrigin, chunk_data *Data)
{
  UNPACK_APPLY_WORLD_EDIT_PARAMS(Params);

  voxel InvertV = { Params->Transparency, Params->Color };

#if 0
  switch (Mode)
  {
    case WorldEdit_Mode_Disabled: {} break;

    /* case WorldEdit_Mode_Paint: */
    case WorldEdit_Mode_Additive:
    case WorldEdit_Mode_Subtractive:
    {
      poof(rectalinear_iteration_pattern({
        v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
        voxel *AssetV = TryGetVoxel(Data, OriginToCurrentVoxP);
        voxel *NewVoxelValue = &InvertV;
        NotImplemented;
        /* if ( (AssetV && (AssetV->Flags&Voxel_Filled)) || Params->PersistWhitespace ) */
        {
          NewVoxelValue  = AssetV;
          OverwriteVoxel = True;
        }
      }))
#include <generated/rectalinear_iteration_pattern_428632106.h>
    } break;
  }
#endif
}




#if 0
link_internal void
ApplyUpdateToRegion(thread_local_state *Thread, work_queue_entry_update_world_region *Job, rect3i SimSpaceUpdateBounds, world_chunk *CopiedChunk, b32 Invert /* = False */, b32 PersistWhitespace /* = False */ )
{
  world *World = GetWorld();
  NotImplemented;

  random_series ColorEntropy = {4654376543246};

  v3i UpdateDim = GetDim(SimSpaceUpdateBounds);
  s32 TotalVoxels = Volume(UpdateDim);

  world_edit_blend_mode              Mode =     Job->Brush.Mode;
  world_edit_blend_mode_modifier Modifier =     Job->Brush.Modifier;
  world_edit_shape            Shape =     Job->Brush.Shape;
  /* v3i                   FloodOrigin = V3i(Job->Brush.SimFloodOrigin); */

  u16 NewColor               = PackHSVColor(Job->HSVColor);
  u8 NewTransparency         = Job->Transparency;

  voxel _NewVoxelValue = {};
  voxel *NewVoxelValue = &_NewVoxelValue;

  if (Mode == WorldEdit_Mode_Additive) // || Mode == WorldEdit_Mode_Paint)
  {
    _NewVoxelValue = { NewTransparency, NewColor};
  }

  switch (Shape.Type)
  {
    InvalidCase(type_world_update_op_shape_params_count);
    InvalidCase(type_world_update_op_shape_params_noop);

    case type_world_update_op_shape_params_sphere:
    {
      world_update_op_shape_params_sphere *Sphere = SafeCast(world_update_op_shape_params_sphere, &Shape);

      v3i EditCenterP = V3i(Floor(GetSimSpaceP(World, Sphere->Location)));
      r32 RadiusSquared = Square(Sphere->Radius);
      rect3i SSRect = SimSpaceUpdateBounds;

      // NOTE(Jesse): In the case of a 1-wide update region, the EditCenterP
      // ends up on the min edge
      Assert(EditCenterP >= SimSpaceUpdateBounds.Min);
      Assert(EditCenterP <= SimSpaceUpdateBounds.Max);

      /* rect3i SSRect = RectCenterRad(EditCenterP, V3i(Sphere->Radius)); */
      apply_world_edit_params Params = {Mode, SSRect, SimSpaceUpdateBounds, CopiedChunk, Invert, PersistWhitespace, NewColor, NewTransparency};

      switch(Modifier)
      {
        case WorldEdit_Modifier_Default:
        {
          WorldEdit_shape_sphere_Default(&Params, RadiusSquared, EditCenterP, NewVoxelValue);
        } break;

        case WorldEdit_ValueModifier_Surface:
        {
          WorldEdit_shape_sphere_Surface(&Params, RadiusSquared, EditCenterP, NewVoxelValue);
        } break;

        /* case WorldEdit_Modifier_Flood: */
        /* { */
        /*   v3i FloodOrigin = EditCenterP; */
        /*   WorldEdit_shape_sphere_Flood(&Params, Thread, RadiusSquared, EditCenterP, FloodOrigin, NewVoxelValue); */
        /* } break; */
      }
} break;

    case type_world_update_op_shape_params_rect:
    {
      world_update_op_shape_params_rect *Rect = SafeCast(world_update_op_shape_params_rect, &Shape);

      // NOTE(Jesse): Outside world should have min/max'd these already
      Assert(Rect->Region.Min <= Rect->Region.Max);

      // NOTE(Jesse): These are specifically meant to truncate, not floor
      rect3i SSRect = {V3i(Rect->Region.Min), V3i(Rect->Region.Max)};
      v3i EditCenterP = V3i(Floor(Rect->Region.Min+(GetDim(SSRect)/2.f)));

      apply_world_edit_params Params = {Mode, SSRect, SimSpaceUpdateBounds, CopiedChunk, Invert, PersistWhitespace, NewColor, NewTransparency};
      switch (Modifier)
      {
        case WorldEdit_Modifier_Surface:
        {
          WorldEdit_shape_rect_Surface(&Params, NewVoxelValue);
        } break;

        case WorldEdit_Modifier_Flood:
        {
          v3i FloodOrigin = EditCenterP;
          WorldEdit_shape_rect_Flood(&Params, Thread, FloodOrigin, NewVoxelValue);
        } break;

        case WorldEdit_Modifier_Default:
        {
          WorldEdit_shape_rect_Default(&Params, NewVoxelValue);
        } break;
      }
    } break;

    {
      case type_world_update_op_shape_params_asset:
      case type_world_update_op_shape_params_chunk_data:
      {
             asset *Asset     = 0;
        chunk_data  Data      = {};
                v3  SimOrigin = {};

        if (Shape.Type == type_world_update_op_shape_params_asset)
        {
          world_update_op_shape_params_asset *Casted = SafeCast(world_update_op_shape_params_asset, &Shape);
          asset_id *AID = &Casted->AssetId;
          Asset = GetAndLockAssetSync(GetEngineResources(), AID);
          Data = GetChunkDataForAssetModel(Asset, u32(Casted->ModelIndex)).Value;
          SimOrigin = GetSimSpaceP(World, Casted->Origin);
        }
        else if (Shape.Type == type_world_update_op_shape_params_chunk_data)
        {
          auto *Casted = SafeCast(world_update_op_shape_params_chunk_data, &Shape);
          Data = Casted->Data;
          SimOrigin = Casted->SimSpaceOrigin;
        }
        else
        {
          InvalidCodePath();
        }

        rect3i SSRect = RectMinDim(V3i(SimOrigin), Data.Dim);
        v3i EditCenterP = V3i(SimOrigin) + V3i(Data.Dim/2.f);

        apply_world_edit_params Params = {Mode, SSRect, SimSpaceUpdateBounds, CopiedChunk, Invert, PersistWhitespace, NewColor, NewTransparency};
        switch (Modifier)
        {
          case WorldEdit_Modifier_Surface:
          {
            WorldEdit_shape_chunk_data_Surface(&Params, SimOrigin, &Data);
          } break;

          case WorldEdit_Modifier_Flood:
          {
            v3i FloodOrigin = EditCenterP;
            WorldEdit_shape_chunk_data_Flood(&Params, SimOrigin, &Data, Thread, FloodOrigin);
          } break;

          case WorldEdit_Modifier_Default:
          {
            WorldEdit_shape_chunk_data_Default(&Params, SimOrigin, &Data);
          } break;
        }

        if (Asset) { UnlockAsset(GetEngineResources(), Asset); Asset = 0; }
      } break;
    }
  }

}
#endif

#if 0
link_internal void
DoWorldUpdate(work_queue *Queue, world *World, thread_local_state *Thread, work_queue_entry_update_world_region *Job)
{
  TIMED_FUNCTION();

  world_edit_blend_mode              Mode = Job->Brush.Mode;
  world_edit_blend_mode_modifier Modifier = Job->Brush.Modifier;
  world_edit_shape            Shape = Job->Brush.Shape;
  /* v3i                   FloodOrigin = V3i(Job->Brush.SimFloodOrigin); */

  u16 NewColor                  = PackHSVColor(Job->HSVColor);
  u8  NewTransparency           = Job->Transparency;
  cp  MaxP                      = Job->MaxP;
  cp  MinP                      = Job->MinP;
  world_chunk **DestChunkBuffer = Job->DestChunkBuffer;
  u32 ChunkCount                = Job->ChunkCount;

  v3 _P0 = GetSimSpaceP(World, MinP);
  v3 _P1 = GetSimSpaceP(World, MaxP);

  // TODO(Jesse): Should these not just be assertions?  Pretty sure the calling
  // code makes sure these are already in 'sorted' order.
  v3 P0 = Min(_P0, _P1);
  v3 P1 = Max(_P0, _P1);

  // NOTE(Jesse): These are meant to truncate instead of floor
  rect3i SimSpaceUpdateBounds = Rect3iMinMax( V3i(P0), V3i(P1) );

  v3i UpdateDim = GetDim(SimSpaceUpdateBounds);
  v3i UpdateMinP = SimSpaceUpdateBounds.Min;

  Assert(UpdateDim.x % Global_StandingSpotDim.x == 0);
  Assert(UpdateDim.y % Global_StandingSpotDim.y == 0);

  /* s32 TotalVoxels_signed = Volume(SimSpaceUpdateBounds); */
  /* Assert(TotalVoxels_signed > 0); */

  world_chunk CopiedChunk = {};

  s32 TotalVoxels = Volume(UpdateDim);
  CopiedChunk.Voxels = Allocate(voxel, Thread->PermMemory, TotalVoxels);
  CopiedChunk.Dim = UpdateDim;

  for (s32 VoxelIndex = 0; VoxelIndex < TotalVoxels; ++VoxelIndex) { CopiedChunk.Voxels[VoxelIndex] = Global_UnsetVoxel; }

#if 1
  for (u32 ChunkIndex = 0; ChunkIndex < ChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = DestChunkBuffer[ChunkIndex];
    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceIntersectionRect = Union(&SimSpaceChunkRect, &SimSpaceUpdateBounds);

    auto SimSpaceIntersectionMin = SimSpaceIntersectionRect.Min;
    auto SimSpaceIntersectionMax = SimSpaceIntersectionRect.Max;

    auto SimSpaceChunkMin = SimSpaceChunkRect.Min;
    auto SimSpaceChunkMax = SimSpaceChunkRect.Max;

    auto ChunkRelRectMin = SimSpaceIntersectionMin - SimSpaceChunkMin;
    auto ChunkRelRectMax = SimSpaceIntersectionMax - SimSpaceChunkMin;

    for (s32 zVoxel = s32(ChunkRelRectMin.z); zVoxel < s32(ChunkRelRectMax.z); zVoxel += 1)
    {
      for (s32 yVoxel = s32(ChunkRelRectMin.y); yVoxel < s32(ChunkRelRectMax.y); yVoxel += 1)
      {
        for (s32 xVoxel = s32(ChunkRelRectMin.x); xVoxel < s32(ChunkRelRectMax.x); xVoxel += 1)
        {
          v3i RelVoxP = V3i(s32(xVoxel), s32(yVoxel), s32(zVoxel));
          voxel *V = GetVoxel(Chunk, RelVoxP);
          NotImplemented;
          /* Assert( (V->Flags & Voxel_MarkBit) == 0); */

          v3i SimSpaceVoxPExact = V3i(xVoxel, yVoxel, zVoxel) + SimSpaceChunkMin;

          Assert(UpdateMinP <= SimSpaceVoxPExact);
          u32 Index = MapIntoQueryBox(SimSpaceVoxPExact, UpdateMinP, UpdateDim);
          Assert(s32(Index) < TotalVoxels);
          Assert(CopiedChunk.Voxels[Index] == Global_UnsetVoxel);
          CopiedChunk.Voxels[Index] = *V;
        }
      }
    }
  }
#endif

#if 0
  world_update_callback Callback = Global_WorldUpdateCallbackTable[Mode][Modifier][Shape.Type];
  if (Callback)
  {
    Callback(Mode, Modifier, &Shape, DestChunkBuffer, ChunkCount, &SimSpaceUpdateBounds, CopiedChunk.Voxels);
  }
  else
  {
    Warn("Attempted to index into an un-implemented world update callback (%S)(%S)(%S).", ToString(Mode), ToString(Modifier), ToString(Shape.Type));
  }
#endif

  b32 Invert = False;
  ApplyUpdateToRegion(Thread, Job, SimSpaceUpdateBounds, &CopiedChunk, Invert, Job->PersistWhitespace);


  // NOTE(Jesse): We can actually do the entire dim here, but it's probably
  // better (faster) to just do what we actually need to

  MakeFaceMasks_NoExteriorFaces( CopiedChunk.Occupancy, CopiedChunk.xOccupancyBorder, CopiedChunk.FaceMasks, CopiedChunk.Voxels, UpdateDim, {{1,1,1}}, UpdateDim-1);
  /* MarkBoundaryVoxels_NoExteriorFaces( CopiedChunk.Voxels, UpdateDim, {}, UpdateDim); */
  /* MarkBoundaryVoxels_MakeExteriorFaces( CopiedChunk.Voxels, UpdateDim, {{1,1,1}}, UpdateDim-1); */
  /* MarkBoundaryVoxels_MakeExteriorFaces( CopiedChunk.Voxels, UpdateDim, {}, UpdateDim); */


#if 1
  for (u32 ChunkIndex = 0; ChunkIndex < ChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = DestChunkBuffer[ChunkIndex];
    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceIntersectionRect = Union(&SimSpaceChunkRect, &SimSpaceUpdateBounds);

    auto SimSpaceIntersectionMin = SimSpaceIntersectionRect.Min;
    auto SimSpaceIntersectionMax = SimSpaceIntersectionRect.Max;

    auto SimSpaceChunkMin = SimSpaceChunkRect.Min;
    auto SimSpaceChunkMax = SimSpaceChunkRect.Max;

    auto ChunkRelRectMin = SimSpaceIntersectionMin - SimSpaceChunkMin;
    auto ChunkRelRectMax = SimSpaceIntersectionMax - SimSpaceChunkMin;

    s32 StartedFilled = 0;
    s32 EndedFilled = 0;
    for (s32 zVoxel = s32(ChunkRelRectMin.z); zVoxel < s32(ChunkRelRectMax.z); zVoxel += 1)
    {
      for (s32 yVoxel = s32(ChunkRelRectMin.y); yVoxel < s32(ChunkRelRectMax.y); yVoxel += 1)
      {
        for (s32 xVoxel = s32(ChunkRelRectMin.x); xVoxel < s32(ChunkRelRectMax.x); xVoxel += 1)
        {
          voxel_position RelVoxP = Voxel_Position(s32(xVoxel), s32(yVoxel), s32(zVoxel));
          voxel *V = GetVoxel(Chunk, RelVoxP);

          v3i SimSpaceVoxPExact = V3i(xVoxel, yVoxel, zVoxel) + SimSpaceChunkMin;

          Assert(UpdateMinP <= SimSpaceVoxPExact);
          u32 Index = MapIntoQueryBox(SimSpaceVoxPExact, UpdateMinP, UpdateDim);
          Assert(s32(Index) < TotalVoxels);
          /* Assert(CopiedChunk.Voxels[Index].Flags        != Global_UnsetVoxel.Flags); */
          Assert(CopiedChunk.Voxels[Index].Transparency != Global_UnsetVoxel.Transparency);
          Assert(CopiedChunk.Voxels[Index].Color        != Global_UnsetVoxel.Color);

          NotImplemented;
          /* Assert( (V->Flags & Voxel_MarkBit) == 0); */

          StartedFilled += GetOccupancyBit(Chunk, s32(Index));
          *V = CopiedChunk.Voxels[Index];
          EndedFilled += GetOccupancyBit(&CopiedChunk, s32(Index));
          NotImplemented;
          /* Assert( (V->Flags & Voxel_MarkBit) == 0); */
        }
      }
    }

    s32 DiffFilled = EndedFilled - StartedFilled;
    Chunk->FilledCount += DiffFilled;
    Assert(Chunk->FilledCount >= 0);
  }
#endif



  // DEBUG CODE
  //

#define DEBUG_VIEW_WORLD_UPDATE 0
#if DEBUG_VIEW_WORLD_UPDATE
  untextured_3d_geometry_buffer *DebugMesh = &GetEngineDebug()->WorldEditDebugMesh;
  // TODO(Jesse): Need to copy the voxels because the Greedy thing blows away
  // the face flags as it does the traversal.
  /* NotImplemented; */

  chunk_data CD = { Chunk_VoxelsInitialized, UpdateDim, CopiedChunk.Voxels, 0 };
  vox_data Vox = {&CD};
  BuildWorldChunkMeshFromMarkedVoxels_Greedy( &Vox, DebugMesh, 0, GetTranArena());

  /* aabb QueryAABB = AABBMinMax( {}, V3i(7.f + Radius*2.f) ); */

  DEBUG_DrawAABB(DebugMesh, AABBMinDim({}, V3(UpdateDim)), BLUE);

  DEBUG_DrawAABB(DebugMesh, AABBMinDim(V3(-1), V3(2)), PINK);

/*   world_chunk *TempChunk = AllocateWorldChunk(Thread->PermMemory, MinP.WorldP, UpdateDim); */
/*   picked_world_chunk *PickedChunk = Allocate(picked_world_chunk, Thread->PermMemory, 1); */
/*   PickedChunk->Chunk = TempChunk; */

  /* v3 QueryRelLocation = V3(SimSphereP) - UpdateMinP; */
  /* DrawVoxel_MinDim(DebugMesh, QueryRelLocation, V4(1,0,0,1), V3(1.f)); */
#endif

  voxel_position_cursor StandingSpots = V3iCursor(ChunkCount*WORLD_CHUNK_STANDING_SPOT_COUNT, Thread->TempMemory);

  /* if (GetLevelEditor()->Flags & LevelEditorFlags_StandingSpotsOnLevelLoad) */
#if 0
  NotImplemented;
#else
  {
    ComputeStandingSpots( UpdateDim,
                          CopiedChunk.Occupancy,
                          CopiedChunk.Voxels,
                          {},
                          {},
                          Global_StandingSpotDim,
                          UpdateDim,
                          0,
                          &StandingSpots, Thread->TempMemory );
  }
#endif

  FullBarrier;
  for (u32 ChunkIndex = 0; ChunkIndex < ChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = DestChunkBuffer[ChunkIndex];

    rect3i SimSpaceChunkAABB = GetSimSpaceAABBi(World, Chunk);
    auto QueryRelChunkAABB = SimSpaceChunkAABB - UpdateMinP;
#if DEBUG_VIEW_WORLD_UPDATE
    DEBUG_DrawAABB(DebugMesh, &QueryRelChunkAABB, RED);
#endif

    /* DebugLine("Start StandingSpotCount(%d)/(%d)", AtElements(&Chunk->StandingSpots), Count(&Chunk->StandingSpots)); */

    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceChunkMin = SimSpaceChunkRect.Min;

    // NOTE(Jesse): Cull old standing spots that are entirely inside the update AABB
    u32 StandingSpotIndex = 0;
    for (;;)
    {
      u32 LastIndex = (u32)AtElements(&Chunk->StandingSpots);
      if (StandingSpotIndex >= LastIndex) { break; }

      voxel_position ChunkSpot = Chunk->StandingSpots.Start[StandingSpotIndex];
      voxel_position ChunkSimSpot = SimSpaceChunkMin + ChunkSpot;
      rect3i SimSpotAABB = Rect3iMinDim(ChunkSimSpot, Global_StandingSpotDim);

      voxel_position QueryRelChunkSpot = ChunkSimSpot - UpdateMinP;

      {
        /* DrawStandingSpot(Mesh, V3(QueryRelChunkSpot), V3(Global_StandingSpotDim), TEAL, DEFAULT_STANDING_SPOT_THICKNESS*1.5f); */
      }

      auto SimSpaceSpotUnion = Union(&SimSpotAABB, &SimSpaceUpdateBounds);
      auto SimSpaceUnionDim = GetDim(SimSpaceSpotUnion);
      if (Volume(SimSpaceSpotUnion) == Volume(Global_StandingSpotDim)) // Cull
      {
        voxel_position QueryRelUnion = SimSpaceSpotUnion.Min - UpdateMinP;
        voxel_position SwapSpot = Pop<voxel_position, voxel_position_cursor>(&Chunk->StandingSpots);
        Chunk->StandingSpots.Start[StandingSpotIndex] = SwapSpot;
        /* DebugLine("Dropping StandingSpot(%d,%d,%d)", ChunkSpot.x, ChunkSpot.y, ChunkSpot.z); */
        /* DrawVoxel_MinDim(Mesh, V3(QueryRelUnion), ORANGE, V3(SimSpaceUnionDim), DEFAULT_LINE_THICKNESS*2.f); */
#if DEBUG_VIEW_WORLD_UPDATE
        DrawStandingSpot(DebugMesh, V3(QueryRelChunkSpot), V3(Global_StandingSpotDim), RED, DEFAULT_STANDING_SPOT_THICKNESS*2.f);
#endif
      }
      else
      {
        /* DebugLine("Keeping StandingSpot(%d,%d,%d)", ChunkSpot.x, ChunkSpot.y, ChunkSpot.z); */
#if DEBUG_VIEW_WORLD_UPDATE
        DrawStandingSpot(DebugMesh, V3(QueryRelChunkSpot), V3(Global_StandingSpotDim), GREEN, DEFAULT_STANDING_SPOT_THICKNESS*2.f);
#endif
        ++StandingSpotIndex;
      }
    }

    // Cull gen'd standing spots that overlap with any current standing spots
    for (StandingSpotIndex = 0; StandingSpotIndex < AtElements(&StandingSpots); ++StandingSpotIndex)
    {
      voxel_position QueryRelSpot = StandingSpots.Start[StandingSpotIndex];
      voxel_position SimSpot = QueryRelSpot + UpdateMinP;
      rect3i SimSpotAABB = Rect3iMinDim(SimSpot, Global_StandingSpotDim);

      //
      // NOTE(Jesse): I'm not entirely sure this is working properly, but it seems to be
      b32 Skip = false;
      for (u32 ChunkStandingSpotIndex = 0; ChunkStandingSpotIndex < AtElements(&Chunk->StandingSpots); ++ChunkStandingSpotIndex)
      {
        voxel_position ChunkSpot = Chunk->StandingSpots.Start[ChunkStandingSpotIndex];
        voxel_position ChunkSimSpot = ChunkSpot + SimSpaceChunkMin;
        rect3i ChunkSimSpotAABB = Rect3iMinDim(ChunkSimSpot, Global_StandingSpotDim);
        if (Volume(Union(&SimSpotAABB, &ChunkSimSpotAABB)))
        {
          Skip = true;
          break;
        }
      }

      if (!Skip)
      {
#if DEBUG_VIEW_WORLD_UPDATE
        DrawStandingSpot(DebugMesh, V3(QueryRelSpot), V3(Global_StandingSpotDim), TEAL, DEFAULT_STANDING_SPOT_THICKNESS*1.5f);
#endif
        if ( Contains(SimSpaceChunkAABB, SimSpot) )
        {
          voxel_position ChunkRelSpot = SimSpot - SimSpaceChunkAABB.Min;
          Assert(Contains(World->ChunkDim, ChunkRelSpot));
          if (Remaining(&Chunk->StandingSpots))
          {
            Push(ChunkRelSpot, &Chunk->StandingSpots);
          }
          else
          {
            Warn("Ran out of standing spots on world_chunk (%p)", Chunk);
            break;
          }
        }
      }
    }
  }
}
#endif

