// src/engine/bonsai_type_info.h:46:0

bonsai_type_info_linked_list_node*
GetBucketByName( bonsai_type_info_hashtable *Table, cs Name )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  bonsai_type_info_linked_list_node* Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Name)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (Bucket->Tombstoned == False && AreEqual(E->Name, Name))
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
GetByName( bonsai_type_info_hashtable *Table, cs Name )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_bonsai_type_info Result = {};

  bonsai_type_info_linked_list_node *Bucket = GetBucketByName(Table, Name);
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

