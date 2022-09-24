struct work_queue_entry_copy_buffer
{
  untextured_3d_geometry_buffer *Src;
  untextured_3d_geometry_buffer Dest;
  v3 Basis;
};


#define WORK_QUEUE_MAX_COPY_TARGETS 4
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

struct work_queue_entry_init_asset
{
  world_chunk *Chunk;
};


poof(
  d_union work_queue_entry
  {
    work_queue_entry_init_world_chunk
    work_queue_entry_copy_buffer
    work_queue_entry_copy_buffer_set
    work_queue_entry_init_asset
  }
)
#include <generated/d_union_work_queue_entry.h>

link_internal void
DrainQueue(work_queue* Queue, thread_local_state* Thread, bonsai_worker_thread_callback GameWorkerThreadCallback)
{
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
                                           (DequeueIndex+1) % WORK_QUEUE_SIZE,
                                           DequeueIndex );
    if ( Exchanged )
    {
      volatile work_queue_entry* Entry = Queue->Entries + DequeueIndex;
      GameWorkerThreadCallback(Entry, Thread);
    }
  }
}

