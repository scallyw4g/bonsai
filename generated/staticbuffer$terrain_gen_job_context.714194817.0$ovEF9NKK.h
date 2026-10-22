// callsite
// src/engine/graphics.h:175:0

// def (staticbuffer)
// external/bonsai_stdlib/src/poof_functions.h:1273:0
struct terrain_gen_job_context_static_buffer_WORLD_CHUNK_INIT_JOBS
{
  terrain_gen_job_context Start[WORLD_CHUNK_INIT_JOBS];
};

link_inline terrain_gen_job_context*
GetPtr( terrain_gen_job_context_static_buffer_WORLD_CHUNK_INIT_JOBS *Buf, umm Index)
{
  terrain_gen_job_context *Result = {};
  if ( Index < umm(WORLD_CHUNK_INIT_JOBS) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline terrain_gen_job_context*
TryGetPtr( terrain_gen_job_context_static_buffer_WORLD_CHUNK_INIT_JOBS *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline terrain_gen_job_context
Get( terrain_gen_job_context_static_buffer_WORLD_CHUNK_INIT_JOBS *Buf, umm Index)
{
  Assert(Index >= 0);
  Assert(Index < umm(WORLD_CHUNK_INIT_JOBS));
  terrain_gen_job_context Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( terrain_gen_job_context_static_buffer_WORLD_CHUNK_INIT_JOBS  *Buf)
{
  return WORLD_CHUNK_INIT_JOBS;
}

link_internal umm
TotalElements( terrain_gen_job_context_static_buffer_WORLD_CHUNK_INIT_JOBS *Buf)
{
  return WORLD_CHUNK_INIT_JOBS;
}



