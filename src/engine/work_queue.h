
struct work_queue_entry_copy_buffer_ref
{
  untextured_3d_geometry_buffer *Dest;

  lod_element_buffer *Buf;
  world_chunk_mesh_bitfield MeshBit;

  v3 Basis;
};
CAssert(sizeof(work_queue_entry_copy_buffer_ref) == 8*4);

struct particle_system;
struct work_queue_entry_sim_particle_system
{
  particle_system *System;
  untextured_3d_geometry_buffer *TranspDest;
  /* untextured_3d_geometry_buffer *EmissiveDest; */
  untextured_3d_geometry_buffer *SolidDest;
  v3 EntityDelta;
  v3 RenderSpaceP;
  r32 dt;
};
// TODO(Jesse)(poof): Make poof able to generate this!
/* poof(gen_constructor(work_queue_entry_sim_particle_system)) */

link_internal work_queue_entry_sim_particle_system
WorkQueueEntrySimParticleSystem( particle_system *System, v3 EntityDelta, v3 RenderSpaceP, r32 dt)
{
  UNPACK_ENGINE_RESOURCES( GetEngineResources() );

  untextured_3d_geometry_buffer *TranspDest = &Graphics->Transparency.GpuBuffer.Buffer;
  /* untextured_3d_geometry_buffer *EmissiveDest = &Graphics->Bloom.GpuBuffer.Buffer; */
  untextured_3d_geometry_buffer *SolidDest = &GpuMap->Buffer;

  work_queue_entry_sim_particle_system Result = {
    .System = System,

    .TranspDest = TranspDest,
    /* .EmissiveDest = EmissiveDest, */
    .SolidDest = SolidDest,

    .EntityDelta = EntityDelta,
    .RenderSpaceP = RenderSpaceP,
    .dt = dt,
  };
  return Result;
}

#define WORK_QUEUE_MAX_COPY_TARGETS 7
struct work_queue_entry_copy_buffer_set
{
  u32 Count;
  work_queue_entry_copy_buffer_ref CopyTargets[WORK_QUEUE_MAX_COPY_TARGETS];
};

struct world_chunk;
struct work_queue_entry_init_world_chunk
{
  world_chunk *Chunk;
  /* world_chunk_mesh_bitfield MeshBit; */
};

struct work_queue_entry_rebuild_mesh
{
  world_chunk *Chunk;
  chunk_init_flags Flags;
  /* world_chunk_mesh_bitfield MeshBit; */
};






struct work_queue_entry_update_world_region
{
  world_edit_brush Brush;

  // TODO(Jesse): These feel like part of the brush?
  u16 ColorIndex;
  u8  Transparency;

  cp MinP;
  cp MaxP;

  world_chunk **DestChunkBuffer;
  u32 ChunkCount;
};

struct asset;
struct work_queue_entry_init_asset
{
  asset *Asset;
};

struct work_queue_entry__align_to_cache_line_helper
{
  // NOTE(Jesse): This ensures the union size is a multiple of a cache line.
  // Sometimes needs to be adjusted if the number of cache lines spanned grows
  //
  // Sub 8 for the type tag in the work_queue_entry
  u8 Pad[(CACHE_LINE_SIZE*4) -8];
};
CAssert( (sizeof(work_queue_entry__align_to_cache_line_helper)+8) % CACHE_LINE_SIZE == 0);






#if VOXEL_DEBUG_COLOR
global_variable voxel Global_UnsetVoxel = { 0xff, 0xff, 0xffff, {}, {}};
#else
global_variable voxel Global_UnsetVoxel = { 0xff, 0xff, 0xffff };
#endif


#if 0
poof(
  func rectilinear_world_update_inplace(type_poof_symbol MetaMode,
                                        type_poof_symbol MetaModifier,
                                        type_poof_symbol MetaShapeType,
                                        type_poof_symbol UserCode)
  {
    link_internal void
    RectilinearWorldUpdate_(MetaMode)_(MetaModifier)_(MetaShapeType)
    (          world_edit_mode   Mode,
      world_edit_mode_modifier   Modifier,
         world_update_op_shape  *Shape,
                   world_chunk **ChunkBuffer,
                           u32   ChunkCount,
                        rect3i  *SimSpaceQueryAABB,
                         voxel  *CopiedVoxels )
    {

      Assert(Mode == MetaMode);
      Assert(Modifier == MetaModifier);
      Assert(Shape->Type == MetaShapeType);

      /* v3i SimSpaceQueryMinP = V3i(SimSpaceQueryAABB->Min); */
      v3i SimSpaceQueryDim = GetDim(*SimSpaceQueryAABB);
      Assert(SimSpaceQueryDim.x % Global_StandingSpotDim.x == 0);
      Assert(SimSpaceQueryDim.y % Global_StandingSpotDim.y == 0);

      DimIterator(x, y, z, SimSpaceQueryDim)
      {
        v3i SimRelVoxP = V3i(x,y,z);
        /* v3i SimVoxP = SimRelVoxP + SimSpaceQueryAABB.Min; */
        voxel *V = CopiedVoxels + GetIndex(SimRelVoxP, SimSpaceQueryDim);

        UserCode
      }
    }

    // Unfortunately, the C compiler is too stupid to do this.  I'm leaving
    // this here such that when adding a new callback you can manually paste
    // this line into Initialize_Global_UpdateWorldCallbackTable()
    //
    static const char* lolwut_StringHack_(MetaMode)_(MetaModifier)_(MetaShapeType) = "Global_WorldUpdateCallbackTable[MetaMode][MetaModifier][MetaShapeType] = RectilinearWorldUpdate_(MetaMode)_(MetaModifier)_(MetaShapeType);";
  }
)


typedef void(*world_update_callback)(world_edit_mode, world_edit_mode_modifier, world_update_op_shape *, world_chunk **, u32, rect3i *, voxel *);

global_variable world_update_callback
Global_WorldUpdateCallbackTable[WorldEdit_Mode_Count][WorldEdit_Modifier_Count][type_world_update_op_shape_params_count];


poof(rectilinear_world_update_inplace( {WorldEdit_Mode_Attach},
                                       {WorldEdit_Modifier_None},
                                       {type_world_update_op_shape_params_rect},
  {
    world_update_op_shape_params_rect *Rect = SafeCast(world_update_op_shape_params_rect, Shape);
    rect3i SSRect = {V3i(Rect->P0), V3i(Rect->P1)};

    /* voxel NewVoxelValue = { Voxel_Filled, NewTransparency, NewColor}; */
    /* if (Contains(SSRect, SimVoxP)) { *V = NewVoxelValue; } */
  }
))
#include <generated/rectilinear_world_update_inplace_606674763_323214162_687443974_863974643.h>

link_internal void
Initialize_Global_UpdateWorldCallbackTable()
{
  Global_WorldUpdateCallbackTable[WorldEdit_Mode_Attach][WorldEdit_Modifier_None][type_world_update_op_shape_params_rect] = RectilinearWorldUpdate_WorldEdit_Mode_Attach_WorldEdit_Modifier_None_type_world_update_op_shape_params_rect;
}
#endif









poof(
  d_union work_queue_entry
  {
    work_queue_entry_init_world_chunk
    /* work_queue_entry_copy_buffer */
    work_queue_entry_copy_buffer_set
    work_queue_entry_copy_buffer_ref
    work_queue_entry_init_asset
    work_queue_entry_update_world_region
    work_queue_entry_rebuild_mesh
    work_queue_entry_sim_particle_system

    // NOTE(Jesse): This is kind of a hack to put render commands onto the work
    // queue so I don't have to invent a whole generic system for having queues
    // with seperate work entry types.  I should probably do this sometime in
    // the future, but for now I'm just going to stuff it on here and call it good.
    work_queue_entry__bonsai_render_command

    work_queue_entry__align_to_cache_line_helper
  }
)
#include <generated/d_union_work_queue_entry.h>

// nocheckin
// TODO(Jesse): Turn this on
CAssert(sizeof(work_queue_entry) % CACHE_LINE_SIZE == 0);

poof(d_union_constructors(work_queue_entry))
#include <generated/d_union_constructors_work_queue_entry.h>

// TODO(Jesse): Gen this from the constructors generator
link_internal work_queue_entry
WorkQueueEntry( particle_system *System, v3 EntityDelta, v3 RenderSpaceP, r32 dt)
{
  work_queue_entry Result = WorkQueueEntry(WorkQueueEntrySimParticleSystem(System, EntityDelta, RenderSpaceP, dt));
  return Result;
}



link_internal void
HandleJob(volatile work_queue_entry *Entry, thread_local_state *Thread, application_api *GameApi)
{
  if ( GameApi->WorkerMain &&
       GameApi->WorkerMain(Entry, Thread))
  {
    // Game exported a WorkerMain, and it handled the job
  }
  else if (WorkerThread_ApplicationDefaultImplementation)
  {
    WorkerThread_ApplicationDefaultImplementation(Entry, Thread);
  }
}


/* link_internal untextured_3d_geometry_buffer * */
/* GetMeshFor(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit); */

/* link_internal untextured_3d_geometry_buffer * */
/* TakeOwnershipSync(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit); */

/* link_internal void */
/* ReleaseOwnership(threadsafe_geometry_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf); */

link_internal untextured_3d_geometry_buffer *
TakeOwnershipSync(lod_element_buffer *Buf, world_chunk_mesh_bitfield MeshBit);

link_internal void
ReleaseOwnership(lod_element_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf);



