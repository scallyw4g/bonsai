// callsite
// external/bonsai_stdlib/src/work_queue.h:7:0

// def (static_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1331:0


struct work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT
poof(  )
{
  work_queue_job_ptr Start[TERRAIN_JOB_COUNT]; poof(@array_length(Element->At))
  u32 At;
};

link_inline work_queue_job_ptr*
GetPtr( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT *Buf, umm Index)
{
  work_queue_job_ptr *Result = {};
  if ( Index < umm(TERRAIN_JOB_COUNT) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline work_queue_job_ptr*
TryGetPtr( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline work_queue_job_ptr
Get( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT *Buf, umm Index)
{
  Assert(Index < umm(TERRAIN_JOB_COUNT));
  Assert(Index < umm((Buf->At)));
  work_queue_job_ptr Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT  *Buf)
{
  return Buf->At;
}

link_internal umm
TotalElements( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT *Buf)
{
  return TERRAIN_JOB_COUNT;
}

link_inline void
Push( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT *Buf, work_queue_job_ptr *E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = *E;
}

link_inline void
Push( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT *Buf, work_queue_job_ptr E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = E;
}

link_internal u32
Capacity( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT *Arr )
{
  u32 Result = TERRAIN_JOB_COUNT;
  return Result;
}

link_internal u32
Remaining( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT *Arr )
{
  u32 Rem = Capacity(Arr) - u32(AtElements(Arr));
  return Rem;
}

link_internal void
ClearList( work_queue_job_ptr_static_cursor_TERRAIN_JOB_COUNT *Buf )
{
  umm Bytes = sizeof(Buf->Start);
  ZeroMemory(Buf->Start, Bytes);
  Buf->At = 0;
}


