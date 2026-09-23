// callsite
// external/bonsai_stdlib/src/work_queue.cpp:5:0

// def (hashtable_get)
// external/bonsai_stdlib/src/poof_functions.h:1011:0
link_internal work_queue_job_stats_linked_list_node*
GetBucketByHashValue( work_queue_job_stats_hashtable *Table, u32 Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  work_queue_job_stats_linked_list_node* Result = {};

  auto *Bucket = GetHashBucket(Hash(&Query), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

        if (Bucket->Tombstoned == False && AreEqual(E->HashValue, Query))

    {
      Result = Bucket;
      break;
    }
    else
    {
      Bucket = Bucket->Next;
    }
  }

  return Result;
}

link_internal work_queue_job_stats *
GetByHashValue( work_queue_job_stats_hashtable *Table, u32 Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  work_queue_job_stats *Result = {};

  work_queue_job_stats_linked_list_node *Bucket = GetBucketByHashValue(Table, Query);
  if (Bucket)
  {
    Result = &Bucket->Element;
  }

  return Result;
}


link_internal b32
Tombstone(u32 Key, work_queue_job_stats_hashtable *Table, memory_arena *Memory)
{
  b32 Result = False;
  work_queue_job_stats_linked_list_node *Bucket = GetBucketByHashValue(Table, Key);
  if (Bucket)
  {
    Assert(Bucket->Tombstoned == False);
    Bucket->Tombstoned = True;
    Result = True;
  }
  return Result;
}

link_internal b32
Drop( work_queue_job_stats_hashtable *Table, u32 Key )
{
  return Tombstone(Key, Table, 0);
}

