b32
QueueIsEmpty(work_queue* Queue)
{
  b32 Result = Queue->DequeueIndex == Queue->EnqueueIndex;
  return Result;
}

void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  TIMED_FUNCTION();
  work_queue_entry* Dest = Queue->Entries + Queue->EnqueueIndex;
  Assert(Dest->Type == WorkEntryType_None);

  *Dest = *Entry;

  FullBarrier;

  Queue->EnqueueIndex = (Queue->EnqueueIndex+1) % WORK_QUEUE_SIZE;

  WakeThread( Queue->GlobalQueueSemaphore );

  return;
}

inline void
QueueChunkForInit(game_state *GameState, work_queue *Queue, world_chunk *Chunk)
{
  TIMED_FUNCTION();
  Assert( Chunk->Data->Flags == Chunk_Uninitialized);

  work_queue_entry Entry = {};

  Entry.Input = (void*)Chunk;
  Entry.Type = WorkEntryType_InitWorldChunk;
  Entry.GameState = GameState;

  Chunk->Data->Flags = Chunk_Queued;

  PushWorkQueueEntry(Queue, &Entry);

  return;
}

