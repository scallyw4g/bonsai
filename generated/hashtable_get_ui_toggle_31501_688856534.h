// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:22:0

// def (hashtable_get)
// external/bonsai_stdlib/src/poof_functions.h:1005:0
ui_toggle_linked_list_node*
GetBucketById( ui_toggle_hashtable *Table, ui_id Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  ui_toggle_linked_list_node* Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Query)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

        if (Bucket->Tombstoned == False && AreEqual(E->Id, Query))

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

maybe_ui_toggle
GetById( ui_toggle_hashtable *Table, ui_id Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_ui_toggle Result = {};

  ui_toggle_linked_list_node *Bucket = GetBucketById(Table, Query);
  if (Bucket)
  {
    Result.Tag = Maybe_Yes;
    Result.Value = Bucket->Element;
  }

  return Result;
}


link_internal b32
Tombstone(ui_id Key, ui_toggle_hashtable *Table, memory_arena *Memory)
{
  b32 Result = False;
  ui_toggle_linked_list_node *Bucket = GetBucketById(Table, Key);
  if (Bucket)
  {
    Assert(Bucket->Tombstoned == False);
    Bucket->Tombstoned = True;
    Result = True;
  }
  return Result;
}

link_internal b32
Drop( ui_toggle_hashtable *Table, ui_id Key )
{
  return Tombstone(Key, Table, 0);
}

