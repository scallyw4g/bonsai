void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  TIMED_FUNCTION();

  while (QueueIsFull(Queue))
  {
    Perf("Queue full!");
    SleepMs(1);
  }

  volatile work_queue_entry* Dest = Queue->Entries + Queue->EnqueueIndex;
  Clear(Dest);
  Assert(Dest->Type == type_work_queue_entry_noop);

  MemCopy((u8*)Entry, (u8*)Dest, sizeof(work_queue_entry));
  /* *Dest = *((volatile work_queue_entry*)Entry); */

  Assert(Dest->Type != type_work_queue_entry_noop);

  FullBarrier;

  u32 NewIndex = GetNextQueueIndex(Queue->EnqueueIndex);
  AtomicExchange(&Queue->EnqueueIndex, NewIndex);

  FullBarrier;

  /* WakeThread( Queue->GlobalQueueSemaphore ); */
}

link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_copy_buffer *Job)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_copy_buffer,
    .work_queue_entry_copy_buffer = *Job,
  };

  return Result;
}

link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_copy_buffer_set *CopySet)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_copy_buffer_set,
    .work_queue_entry_copy_buffer_set = *CopySet,
  };

  return Result;
}

link_internal void
PushCopyJob(work_queue *Queue, work_queue_entry_copy_buffer_set *Set, work_queue_entry_copy_buffer *Job)
{
  TIMED_FUNCTION();

  Set->CopyTargets[Set->Count] = *Job;
  ++Set->Count;

  if (Set->Count == WORK_QUEUE_MAX_COPY_TARGETS)
  {
    work_queue_entry Entry = WorkQueueEntry(Set);
    PushWorkQueueEntry(Queue, &Entry);
    Clear(Set);
    Assert(Set->Count == 0);
  }
}

link_internal void
DoCopyJob(volatile work_queue_entry_copy_buffer *Job)
{
  untextured_3d_geometry_buffer* Src = Job->Src;
  untextured_3d_geometry_buffer* Dest = (untextured_3d_geometry_buffer*)&Job->Dest;
  Assert(Src->At <= Dest->End);

  v3 Basis = *(v3*)&Job->Basis;
  BufferVertsChecked(Src, Dest, Basis, V3(1.0f));
}
