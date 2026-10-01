
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

struct world_chunk;

struct work_queue_entry_build_chunk_mesh
{
  gen_chunk   *GenChunk;
  octree_node *DestNode;
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





#if 0
poof(
  d_union work_queue_entry
  {
    work_queue_entry_build_chunk_mesh
    work_queue_entry_init_asset
    work_queue_entry_sim_particle_system


    // NOTE(Jesse): This is kind of a hack to put render commands onto the work
    // queue so I don't have to invent a whole generic system for having queues
    // with seperate work entry types.  I should probably do this sometime in
    // the future, but for now I'm just going to stuff it on here and call it good.
    work_queue_entry__bonsai_render_command

    work_queue_entry_async_function_call

    work_queue_entry__align_to_cache_line_helper
  },
  {
    // NOTE(Jesse): This is the queue the job needs to be submitted to to
    // complete this task.
    work_queue_ptr Queue;
  }
)
#include <generated/poof_builtin.d_union$$YdAfLGDb.h>
#endif














link_internal s32
EventsCurrentlyInQueue(work_queue *Queue)
{
  u32 Enqueue = Queue->EnqueueIndex;
  u32 Dequeue = Queue->DequeueIndex;

  s32 Result = 0;
  if (Dequeue < Enqueue)
  {
    Result = s32(Enqueue - Dequeue);
  }

  if (Enqueue < Dequeue)
  {
    Result = s32((WORK_QUEUE_SIZE - Dequeue) + Enqueue);
  }

  Assert(Result >= 0);
  return Result;
}

#if 0
// TODO(Jesse): Gen this from the constructors generator
link_internal work_queue_entry
WorkQueueEntry( work_queue *Queue, particle_system *System, v3 EntityDelta, v3 RenderSpaceP, r32 dt)
{
  work_queue_entry Result = WorkQueueEntry(WorkQueueEntrySimParticleSystem(System, EntityDelta, RenderSpaceP, dt), Queue);
  return Result;
}

link_internal b32
MaybeResubmitJob(work_queue_job *Job)
{
  b32 Result = False;
  if (work_queue_entry *Next = PeekNextTask(Job))
  {
    Result = True;
    SubmitJob(Next->Queue, Job);
  }
  else
  {
    ReleaseWorkQueueJob(GetPlatform(), Job);
  }
  return Result;
}

link_internal void
HandleJob(work_queue_job *Job, thread_local_state *Thread, application_api *GameApi)
{
  if ( GameApi->WorkerMain &&
       GameApi->WorkerMain(Job, Thread))
  {
    // Game exported a WorkerMain, and it handled the job
  }
  else
  {
    WorkerThread_ApplicationDefaultImplementation(Job, Thread);
  }

  MaybeResubmitJob(Job);
}
#endif
link_internal void
CancelAllWorkQueueJobs(platform *Plat, work_queue *Queue);

link_internal untextured_3d_geometry_buffer *
TakeOwnershipSync(lod_element_buffer *Buf, world_chunk_mesh_bitfield MeshBit);

link_internal void
ReleaseOwnership(lod_element_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf);

struct work_queue_task_async_function_call;

link_internal b32
ValidateRPCForRenderQ(work_queue_task_async_function_call *RPC);
