/* poof(block_array_c(work_queue_entry, {8})) */
/* #include <generated/block_array_c$work_queue_entry.688856411$T8TeCSiR.h> */

link_internal void
Replace(volatile void** Dest, void* Element)
{
  Ensure( AtomicExchange(Dest, Element) == 0);
}

link_internal void
CancelAllWorkQueueJobs(platform *Plat, work_queue *Queue)
{
  NotImplemented;

#if 0
  Assert(FutexIsSignaled(&Plat->WorkerThreadsSuspendFutex));
  Assert(Plat->WorkerThreadsSuspendFutex.ThreadsWaiting == GetWorkerThreadCount());

  // TODO(Jesse): Might as well use memset?
  RangeIterator(EntryIndex, WORK_QUEUE_SIZE)
  {
    work_queue_entry *Entry = GetEntryForJob(Plat, u32(EntryIndex));
    *Entry = {};
  }

  Queue->EnqueueIndex = 0;
  Queue->DequeueIndex = 0;
#endif
}
