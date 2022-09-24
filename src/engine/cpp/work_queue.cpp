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

  /* WakeThread( Queue->GlobalQueueSemaphore ); */
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


