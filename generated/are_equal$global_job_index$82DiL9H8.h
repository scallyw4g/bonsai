// callsite
// external/bonsai_stdlib/src/work_queue.h:35:0

// def (are_equal)
// external/bonsai_stdlib/src/poof_functions.h:658:0
link_internal b32
AreEqual(global_job_index *Thing1, global_job_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( global_job_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(global_job_index Thing1, global_job_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( global_job_index ) );

  return Result;
}

