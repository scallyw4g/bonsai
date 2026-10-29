// callsite
// src/engine/graphics.h:152:0

// def (staticbuffer)
// external/bonsai_stdlib/src/poof_functions.h:1273:0
struct terrain_gen_job_context_static_buffer_TERRAIN_JOB_COUNT
poof(  @do_editor_ui  )
{
  terrain_gen_job_context Start[TERRAIN_JOB_COUNT];
};

link_inline terrain_gen_job_context*
GetPtr( terrain_gen_job_context_static_buffer_TERRAIN_JOB_COUNT *Buf, umm Index)
{
  terrain_gen_job_context *Result = {};
  if ( Index < umm(TERRAIN_JOB_COUNT) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline terrain_gen_job_context*
TryGetPtr( terrain_gen_job_context_static_buffer_TERRAIN_JOB_COUNT *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline terrain_gen_job_context
Get( terrain_gen_job_context_static_buffer_TERRAIN_JOB_COUNT *Buf, umm Index)
{
  Assert(Index >= 0);
  Assert(Index < umm(TERRAIN_JOB_COUNT));
  terrain_gen_job_context Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( terrain_gen_job_context_static_buffer_TERRAIN_JOB_COUNT  *Buf)
{
  return TERRAIN_JOB_COUNT;
}

link_internal umm
TotalElements( terrain_gen_job_context_static_buffer_TERRAIN_JOB_COUNT *Buf)
{
  return TERRAIN_JOB_COUNT;
}

link_internal void
ClearList( terrain_gen_job_context_static_buffer_TERRAIN_JOB_COUNT *Buf )
{
  umm Bytes = sizeof(Buf->Start);
  ZeroMemory(Buf->Start, Bytes);
}



