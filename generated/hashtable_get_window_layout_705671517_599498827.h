// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:32:0

// def (hashtable_get)
// external/bonsai_stdlib/src/poof_functions.h:1005:0
window_layout_linked_list_node*
GetBucketByHashtableKey( window_layout_hashtable *Table, ui_id Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  window_layout_linked_list_node* Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Query)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

        if (Bucket->Tombstoned == False && AreEqual(E->HashtableKey, Query))

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

maybe_window_layout
GetByHashtableKey( window_layout_hashtable *Table, ui_id Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_window_layout Result = {};

  window_layout_linked_list_node *Bucket = GetBucketByHashtableKey(Table, Query);
  if (Bucket)
  {
    Result.Tag = Maybe_Yes;
    Result.Value = Bucket->Element;
  }

  return Result;
}


link_internal b32
Tombstone(ui_id Key, window_layout_hashtable *Table, memory_arena *Memory)
{
  b32 Result = False;
  window_layout_linked_list_node *Bucket = GetBucketByHashtableKey(Table, Key);
  if (Bucket)
  {
    Assert(Bucket->Tombstoned == False);
    Bucket->Tombstoned = True;
    Result = True;
  }
  return Result;
}

link_internal b32
Drop( window_layout_hashtable *Table, ui_id Key )
{
  return Tombstone(Key, Table, 0);
}

