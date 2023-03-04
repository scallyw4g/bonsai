link_internal void
InitQueue(work_queue* Queue, memory_arena* Memory) //, semaphore* Semaphore)
{
  Queue->EnqueueIndex = 0;
  Queue->DequeueIndex = 0;

  Queue->Entries = Allocate(work_queue_entry, Memory, WORK_QUEUE_SIZE);

  InitializeFutex(&Queue->EnqueueFutex);
}

void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  TIMED_FUNCTION();

  AcquireFutex(&Queue->EnqueueFutex);

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

  ReleaseFutex(&Queue->EnqueueFutex);

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

/* link_internal untextured_3d_geometry_buffer* */
/* TryTakeOwnership(volatile untextured_3d_geometry_buffer ** Src) */
/* { */
/*   untextured_3d_geometry_buffer *Result = (untextured_3d_geometry_buffer*)AtomicExchange((volatile void**)Src, (void*)0); */
/*   return Result; */
/* } */

link_internal untextured_3d_geometry_buffer *
TakeOwnershipSync(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit)
{
  TIMED_FUNCTION();

  /* Assert(Buf->Futexes[ToIndex(MeshBit)]); */
  AcquireFutex(&Buf->Futexes[ToIndex(MeshBit)]);

  untextured_3d_geometry_buffer *Result = (untextured_3d_geometry_buffer *)Buf->E[ToIndex(MeshBit)];
  return Result;

#if 0
  while (Result == 0)
  {
    if (Buf->MeshMask & MeshBit)
    {
      Result = TryTakeOwnership(&Buf->E[ToIndex(MeshBit)]);
    }
    else
    {
      break;
    }
  }

  if (Result)
  {
    Assert(Buf->MeshMask & MeshBit);
  }
  else
  {
    // NOTE(Jesse): This doesn't _strictly_ have to be true, but it would be
    // very unlikely to happen at the moment I added it, and I just wanted to
    // make sure it didn't fire.  Remove it
    Assert((Buf->MeshMask&MeshBit) == 0);
  }
  return Result;
#endif
}

link_internal void
ReleaseOwnership(threadsafe_geometry_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf)
{
  if (Buf) { Assert(Src->MeshMask & MeshBit); }

  // NOTE(Jesse): This is a dumb assertion; the buffer we're replacing depends on so many things it's difficult to pass it in here
/* #if BONSAI_INTERNAL */
/*   untextured_3d_geometry_buffer *OldMesh = (untextured_3d_geometry_buffer *)AtomicWrite((volatile void**)&Src->E[ToIndex(MeshBit)], (void*)Buf); */
/*   Assert(OldMesh == Buf); */
/* #endif */

  ReleaseFutex(&Src->Futexes[ToIndex(MeshBit)]);
}

/* link_internal untextured_3d_geometry_buffer * */
/* GetMeshFor(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit) */
/* { */
/*   auto Result = TakeOwnershipSync(Buf, MeshBit); */
/*   return Result; */
/* } */

link_internal void
Replace(volatile void** Dest, void* Element)
{
  Ensure( AtomicExchange(Dest, Element) == 0);
}

link_internal void
DoCopyJob(work_queue_entry_copy_buffer_ref *Job, mesh_freelist* MeshFreelist, memory_arena* PermMemory)
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
DoCopyJob(volatile work_queue_entry_copy_buffer *Job, mesh_freelist* MeshFreelist, memory_arena* PermMemory)
{
  // NOTE(Jesse): This code is broken .. use copy_buffer_ref instead
  InvalidCodePath();

#if 0
  /* untextured_3d_geometry_buffer* Src = Job->Src; */
  untextured_3d_geometry_buffer *Dest = Job->Dest;

  /* untextured_3d_geometry_buffer *SrcBuffer = (untextured_3d_geometry_buffer *)TakeOwnershipSync((volatile void**)Src); */
  /* Replace((volatile void**)Src, (void*)SrcBuffer); */


  untextured_3d_geometry_buffer *Src = TakeOwnershipSync(Job->Src, );
  untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(Dest, Src->At);
  /* umm Count = Min(Src->At, &CopyDest->End); */
  v3 Basis = *(v3*)&Job->Basis;
  BufferVertsChecked( Src, &CopyDest, Basis, V3(1.0f));

  Ensure( AtomicCompareExchange((volatile void**)Job->Src, Src, 0) );
#endif
}
