
link_internal void
DrainQueue(work_queue* Queue, thread_local_state* Thread, bonsai_worker_thread_callback GameWorkerThreadCallback)
{
  for (;;)
  {
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
      /* Callback(Entry, Thread); */
      GameWorkerThreadCallback(Entry, Thread);
      /* Entry->Type = type_work_queue_entry_noop; */
    }
  }
}

