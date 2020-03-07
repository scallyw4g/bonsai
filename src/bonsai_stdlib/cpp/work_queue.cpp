void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  TIMED_FUNCTION();
  work_queue_entry* Dest = Queue->Entries + Queue->EnqueueIndex;
  Assert(Dest->Type == type_work_queue_entry_noop);

  *Dest = *Entry;

  FullBarrier;

  Queue->EnqueueIndex = (Queue->EnqueueIndex+1) % WORK_QUEUE_SIZE;

  WakeThread( Queue->GlobalQueueSemaphore );

  return;
}

inline void
QueueChunkForInit(work_queue *Queue, world_chunk *Chunk)
{
  TIMED_FUNCTION();
  Assert( Chunk->Data->Flags == Chunk_Uninitialized);

  work_queue_entry Entry = {};

  Entry.work_queue_entry_init_world_chunk.Input = (void*)Chunk;
  Entry.Type = type_work_queue_entry_init_world_chunk;

  Chunk->Data->Flags = Chunk_Queued;

  PushWorkQueueEntry(Queue, &Entry);

  return;
}

