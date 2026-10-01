// callsite
// external/bonsai_stdlib/src/work_queue.cpp:7:0

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
  work_queue_job_stats_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  while (Bucket)
  {
     if (Bucket->Tombstoned == False && AreEqual(Bucket->Element.Job , KeyQuery)) 
    {
      Result = &Bucket->Element;
      break;
    }
    else
    {
      Bucket = Bucket->Next;
    }
  }

  return Result;
}

