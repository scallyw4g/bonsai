// callsite
// external/bonsai_stdlib/src/work_queue.cpp:5:0

// def (hashtable_get_by_key)
// external/bonsai_stdlib/src/poof_functions.h:1012:0
//
// Get
//





link_internal work_queue_job_stats *
GetByKey( work_queue_job_stats_hashtable *Table, work_queue_job *KeyQuery )
{
  work_queue_job_stats *Result = {};

  u32 HashValue = Hash(KeyQuery);
  auto Bucket = GetFirstAtBucket(HashValue, Table);
  while (Bucket)
  {
        Result = &Bucket->Element;
    break;

    else
    {
      Bucket = Bucket->Next;
    }
  }

  return Result;
}

