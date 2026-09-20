poof(block_array_c(work_queue_entry, {8}))
#include <generated/block_array_c_o3Xh63ms.h>

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

link_internal void
AllocateJobsArray(platform *Plat, s32 TotalJobs)
{
  Assert(Plat->TaskMemory == 0);

  Plat->TaskMemory = AllocateArena(Megabytes(4));
  Plat->Jobs = Allocate(work_queue_job, Plat->TaskMemory, TotalJobs);

  auto Freelist = Cast(volatile freelist_entry **, &Plat->JobsFreelist);
  RangeIterator_t(u32, Index, u32(TotalJobs))
  {
    work_queue_job *Job = StripVolatile(work_queue_job *, Plat->Jobs+Index);

    Job->Index.Index = Index;
    Job->Magic = WORK_QUEUE_JOB_MAGIC_NUMBER;

    Job->Tasks.Memory = Plat->TaskMemory;

    Link_TS(Freelist, Cast(freelist_entry *, Job));
  }
}

