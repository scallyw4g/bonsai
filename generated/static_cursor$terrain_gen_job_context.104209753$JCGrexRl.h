// callsite
// src/engine/graphics.h:125:0

// def (static_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1331:0


struct terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME
poof(  )
{
  terrain_gen_job_context_ptr Start[TERRAIN_JOBS_PER_FRAME]; poof(@array_length(Element->At))
  u32 At;
};

link_inline terrain_gen_job_context_ptr*
GetPtr( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME *Buf, umm Index)
{
  terrain_gen_job_context_ptr *Result = {};
  if ( Index < umm(TERRAIN_JOBS_PER_FRAME) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline terrain_gen_job_context_ptr*
TryGetPtr( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline terrain_gen_job_context_ptr
Get( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME *Buf, umm Index)
{
  Assert(Index < umm(TERRAIN_JOBS_PER_FRAME));
  Assert(Index < umm((Buf->At)));
  terrain_gen_job_context_ptr Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME  *Buf)
{
  return Buf->At;
}

link_internal umm
TotalElements( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME *Buf)
{
  return TERRAIN_JOBS_PER_FRAME;
}

link_inline void
Push( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME *Buf, terrain_gen_job_context_ptr *E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = *E;
}

link_inline void
Push( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME *Buf, terrain_gen_job_context_ptr E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = E;
}

link_internal u32
Capacity( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME *Arr )
{
  u32 Result = TERRAIN_JOBS_PER_FRAME;
  return Result;
}

link_internal u32
Remaining( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME *Arr )
{
  u32 Rem = Capacity(Arr) - u32(AtElements(Arr));
  return Rem;
}

link_internal void
ClearList( terrain_gen_job_context_ptr_static_cursor_TERRAIN_JOBS_PER_FRAME *Buf )
{
  umm Bytes = sizeof(Buf->Start);
  ZeroMemory(Buf->Start, Bytes);
  Buf->At = 0;
}


