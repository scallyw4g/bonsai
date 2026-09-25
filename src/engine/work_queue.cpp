poof(block_array_c(work_queue_entry, {8}))
#include <generated/block_array_c$work_queue_entry.688856411$T8TeCSiR.h>

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

/* link_internal work_queue_job_stats_hashtable */
/* Allocate_work_queue_job_stats_hashtable(u32 ElementCount, memory_arena *Memory); */

link_internal void
AllocateJobsArray(platform *Plat, s32 TotalJobs)
{
  Assert(Plat->TaskMemory == 0);

  Plat->TaskMemory = AllocateArena(Megabytes(4));
  Plat->Jobs = Allocate(work_queue_job, Plat->TaskMemory, TotalJobs);

  Plat->TotalJobs = u32(TotalJobs);
  Plat->FreeJobs  = u32(TotalJobs);

  auto Freelist = Cast(volatile freelist_entry **, &Plat->JobsFreelist);
  RangeIterator_t(u32, Index, u32(TotalJobs))
  {
    work_queue_job *Job = StripVolatile(work_queue_job *, Plat->Jobs+Index);

    Job->Index.Index = Index;
    Job->Magic = WORK_QUEUE_JOB_MAGIC_NUMBER;

    Job->Tasks.Memory = Plat->TaskMemory;

    Link_TS(Freelist, Cast(freelist_entry *, Job));
  }

  /* Plat->JobStatsTable = Allocate_work_queue_job_stats_hashtable(4096, Plat->TaskMemory); */
}

