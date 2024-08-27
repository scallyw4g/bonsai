link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_copy_buffer_ref *Job)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_copy_buffer_ref,
    .work_queue_entry_copy_buffer_ref = *Job,
  };

  return Result;
}

link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_copy_buffer_set *Job)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_copy_buffer_set,
    .work_queue_entry_copy_buffer_set = *Job,
  };

  return Result;
}

link_internal void
PushCopyJob(work_queue *Queue, work_queue_entry_copy_buffer_set *Set, work_queue_entry_copy_buffer_ref *Job)
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
Replace(volatile void** Dest, void* Element)
{
  Ensure( AtomicExchange(Dest, Element) == 0);
}

link_internal void
DoCopyJob(work_queue_entry_copy_buffer_ref *Job, tiered_mesh_freelist* MeshFreelist, memory_arena* PermMemory)
{
  untextured_3d_geometry_buffer *Src = TakeOwnershipSync(Job->Buf, Job->MeshBit);

  if (Src)
  {
    untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(Job->Dest, Src->At);
    v3 Basis = Job->Basis;
    BufferVertsChecked( Src, &CopyDest, Basis, V3(1.0f));
  }

  ReleaseOwnership(Job->Buf, Job->MeshBit, Src);
}

link_internal void
CancelAllWorkQueueJobs(platform *Plat, work_queue *Queue)
{
  Assert(FutexIsSignaled(&Plat->WorkerThreadsSuspendFutex));
  Assert(Plat->WorkerThreadsSuspendFutex.ThreadsWaiting == GetWorkerThreadCount());

  // TODO(Jesse): Might as well use memset?
  RangeIterator(EntryIndex, WORK_QUEUE_SIZE)
  {
    work_queue_entry *Entry = Cast(work_queue_entry*, Queue->Entries + EntryIndex);
    *Entry = {};
  }

  Queue->EnqueueIndex = 0;
  Queue->DequeueIndex = 0;
}
