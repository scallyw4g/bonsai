// callsite
// external/bonsai_stdlib/src/work_queue.h:7:0

// def (static_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1323:0


struct work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS
poof(@do_editor_ui)
{
  work_queue_job_ptr Start[WORLD_CHUNK_INIT_JOBS]; poof(@array_length(Element->At))
  u32 At;
};

link_inline work_queue_job_ptr*
GetPtr( work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS *Buf, umm Index)
{
  work_queue_job_ptr *Result = {};
  if ( Index < umm(WORLD_CHUNK_INIT_JOBS) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline work_queue_job_ptr*
TryGetPtr( work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline work_queue_job_ptr
Get( work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS *Buf, umm Index)
{
  Assert(Index < umm(WORLD_CHUNK_INIT_JOBS));
  Assert(Index < umm((Buf->At)));
  work_queue_job_ptr Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS  *Buf)
{
  return Buf->At;
}

link_internal umm
TotalElements( work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS *Buf)
{
  return WORLD_CHUNK_INIT_JOBS;
}

link_inline void
Push( work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS *Buf, work_queue_job_ptr *E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = *E;
}

link_inline void
Push( work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS *Buf, work_queue_job_ptr E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = E;
}

link_internal u32
Capacity( work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS *Arr )
{
  u32 Result = WORLD_CHUNK_INIT_JOBS;
  return Result;
}

link_internal u32
Remaining( work_queue_job_ptr_static_cursor_WORLD_CHUNK_INIT_JOBS *Arr )
{
  u32 Rem = Capacity(Arr) - u32(AtElements(Arr));
  return Rem;
}

