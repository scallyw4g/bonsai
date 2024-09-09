// external/bonsai_stdlib/src/ui/ui.cpp:10:0

ui_toggle_linked_list_node*
GetBucketById( ui_toggle_hashtable *Table, ui_id Id )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  ui_toggle_linked_list_node* Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Id)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (Bucket->Tombstoned == False && AreEqual(E->Id, Id))
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
GetById( ui_toggle_hashtable *Table, ui_id Id )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_ui_toggle Result = {};

  ui_toggle_linked_list_node *Bucket = GetBucketById(Table, Id);
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

