// callsite
// external/bonsai_stdlib/src/primitive_containers.cpp:8:0

// def (hashtable_get)
// external/bonsai_stdlib/src/poof_functions.h:1005:0
u32_linked_list_node*
GetBucketByValue( u32_hashtable *Table, u32 Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  u32_linked_list_node* Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Query)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

        if (Bucket->Tombstoned == False && AreEqual(*E, Query))

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

maybe_u32
GetByValue( u32_hashtable *Table, u32 Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_u32 Result = {};

  u32_linked_list_node *Bucket = GetBucketByValue(Table, Query);
  if (Bucket)
  {
    Result.Tag = Maybe_Yes;
    Result.Value = Bucket->Element;
  }

  return Result;
}


link_internal b32
Tombstone(u32 Key, u32_hashtable *Table, memory_arena *Memory)
{
  b32 Result = False;
  u32_linked_list_node *Bucket = GetBucketByValue(Table, Key);
  if (Bucket)
  {
    Assert(Bucket->Tombstoned == False);
    Bucket->Tombstoned = True;
    Result = True;
  }
  return Result;
}

link_internal b32
Drop( u32_hashtable *Table, u32 Key )
{
  return Tombstone(Key, Table, 0);
}

