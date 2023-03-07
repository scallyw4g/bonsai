struct work_queue_entry_copy_buffer_ref
{
  threadsafe_geometry_buffer *Buf;
  world_chunk_mesh_bitfield MeshBit;

  untextured_3d_geometry_buffer *Dest;
  v3 Basis;
};

struct work_queue_entry_copy_buffer
{
  untextured_3d_geometry_buffer **Src;
  untextured_3d_geometry_buffer *Dest;
  v3 Basis;
};


#define WORK_QUEUE_MAX_COPY_TARGETS 8
struct work_queue_entry_copy_buffer_set
{
  u32 Count;
  work_queue_entry_copy_buffer CopyTargets[WORK_QUEUE_MAX_COPY_TARGETS];
};

struct world_chunk;
struct work_queue_entry_init_world_chunk
{
  world_chunk *Chunk;
};

struct work_queue_entry_rebuild_mesh
{
  world_chunk *Chunk;
};

struct work_queue_entry_update_world_region
{
  picked_voxel Location;
  f32 Radius;

  canonical_position MinP;
  canonical_position MaxP;

  world_chunk **ChunkBuffer;
  u32 ChunkCount;
};

struct work_queue_entry_init_asset
{
  world_chunk *Chunk;
};

// TODO(Jesse): Should these be cache-line sized/aligned?
poof(
  d_union work_queue_entry
  {
    work_queue_entry_init_world_chunk
    work_queue_entry_copy_buffer
    work_queue_entry_copy_buffer_set
    work_queue_entry_copy_buffer_ref
    work_queue_entry_init_asset
    work_queue_entry_update_world_region
    work_queue_entry_rebuild_mesh
  }
)
#include <generated/d_union_work_queue_entry.h>

link_internal void
DrainQueue(work_queue* Queue, thread_local_state* Thread, bonsai_worker_thread_callback GameWorkerThreadCallback)
{
  TIMED_FUNCTION();

  for (;;)
  {
    WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();

    // NOTE(Jesse): Must read and comared DequeueIndex instead of calling QueueIsEmpty
    u32 DequeueIndex = Queue->DequeueIndex;
    if (DequeueIndex == Queue->EnqueueIndex)
    {
      break;
    }

    b32 Exchanged = AtomicCompareExchange( &Queue->DequeueIndex,
                                           GetNextQueueIndex(DequeueIndex),
                                           DequeueIndex );
    if ( Exchanged )
    {
      volatile work_queue_entry* Entry = Queue->Entries + DequeueIndex;
      GameWorkerThreadCallback(Entry, Thread);
    }
  }
}

/* link_internal untextured_3d_geometry_buffer * */
/* GetMeshFor(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit); */

link_internal untextured_3d_geometry_buffer *
TakeOwnershipSync(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit);

link_internal void
ReleaseOwnership(threadsafe_geometry_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf);

