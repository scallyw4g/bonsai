// callsite
// src/engine/bonsai_type_info.h:46:0

// def (hashtable_get)
// external/bonsai_stdlib/src/poof_functions.h:1005:0
bonsai_type_info_linked_list_node*
GetBucketByName( bonsai_type_info_hashtable *Table, cs Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  bonsai_type_info_linked_list_node* Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Query)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

        if (Bucket->Tombstoned == False && AreEqual(E->Name, Query))

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

maybe_bonsai_type_info
GetByName( bonsai_type_info_hashtable *Table, cs Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_bonsai_type_info Result = {};

  bonsai_type_info_linked_list_node *Bucket = GetBucketByName(Table, Query);
  if (Bucket)
  {
    Result.Tag = Maybe_Yes;
    Result.Value = Bucket->Element;
  }

  return Result;
}


link_internal b32
Tombstone(cs Key, bonsai_type_info_hashtable *Table, memory_arena *Memory)
{
  b32 Result = False;
  bonsai_type_info_linked_list_node *Bucket = GetBucketByName(Table, Key);
  if (Bucket)
  {
    Assert(Bucket->Tombstoned == False);
    Bucket->Tombstoned = True;
    Result = True;
  }
  return Result;
}

link_internal b32
Drop( bonsai_type_info_hashtable *Table, cs Key )
{
  return Tombstone(Key, Table, 0);
}

