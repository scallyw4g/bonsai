
struct work_queue_entry_copy_buffer_ref
{
  untextured_3d_geometry_buffer *Dest;

  threadsafe_geometry_buffer *Buf;
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





enum world_update_op_shape_type
{
  type_world_update_op_shape_params_noop,

  type_world_update_op_shape_params_sphere,
  type_world_update_op_shape_params_rect,
  type_world_update_op_shape_params_asset,
  type_world_update_op_shape_params_chunk_data,
};

struct world_update_op_shape_params_sphere
{
  canonical_position Location;
  f32 Radius;
};

struct world_update_op_shape_params_rect
{
  v3 P0;
  v3 P1;
  /* rect3 Region; */ // TODO(Jesse): Make this a thing
  /* rect3cp Region; */
};

struct asset;
struct world_update_op_shape_params_asset
{
  /* asset *Asset; */
  /* model *Model; */

  asset_id AssetId;
  u64      ModelIndex;
  cp Origin;
};

struct world_update_op_shape_params_chunk_data
{
  chunk_data Data;
  v3 SimSpaceOrigin;
  /* cp Origin; */
};


struct world_update_op_shape
{
  world_update_op_shape_type Type;
  union {
    world_update_op_shape_params_sphere     world_update_op_shape_params_sphere;
    world_update_op_shape_params_rect       world_update_op_shape_params_rect;
    world_update_op_shape_params_asset      world_update_op_shape_params_asset;
    world_update_op_shape_params_chunk_data world_update_op_shape_params_chunk_data;
  };
};

// TODO(Jesse): Pretty sure this can be deleted and use world_edit_mode instead
enum world_update_op_mode
{
  WorldUpdateOperationMode_None,
  WorldUpdateOperationMode_Paint,
  WorldUpdateOperationMode_Additive,
  WorldUpdateOperationMode_Subtractive,

  // NOTE(Jesse): This is kind of a hack to skip the update loop and compute standing spots for a given area
  WorldUpdateOperationMode_StandingSpots,
};

enum world_update_op_mode_modifier
{
  WorldUpdateOperationModeModifier_None,
  WorldUpdateOperationModeModifier_Flood,
};


struct work_queue_entry_update_world_region
{
  world_update_op_mode          Mode;
  world_update_op_mode_modifier Modifier;

  world_update_op_shape Shape;

  u16 ColorIndex;
  u8  Transparency;

  canonical_position MinP;
  canonical_position MaxP;

  world_chunk **ChunkBuffer;
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

link_internal untextured_3d_geometry_buffer *
TakeOwnershipSync(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit);

link_internal void
ReleaseOwnership(threadsafe_geometry_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf);

link_internal untextured_3d_geometry_buffer *
TakeOwnershipSync(lod_element_buffer *Buf, world_chunk_mesh_bitfield MeshBit);

link_internal void
ReleaseOwnership(lod_element_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf);
