// callsite
// src/engine/editor.h:1749:0

// def (hashtable_get)
// external/bonsai_stdlib/src/poof_functions.h:1005:0
world_edit_brush_linked_list_node*
GetBucketByNameBuf( world_edit_brush_hashtable *Table, cs Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  world_edit_brush_linked_list_node* Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Query)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

        if (Bucket->Tombstoned == False && AreEqual(E->NameBuf, Query))

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

maybe_world_edit_brush
GetByNameBuf( world_edit_brush_hashtable *Table, cs Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_world_edit_brush Result = {};

  world_edit_brush_linked_list_node *Bucket = GetBucketByNameBuf(Table, Query);
  if (Bucket)
  {
    Result.Tag = Maybe_Yes;
    Result.Value = Bucket->Element;
  }

  return Result;
}


link_internal b32
Tombstone(cs Key, world_edit_brush_hashtable *Table, memory_arena *Memory)
{
  b32 Result = False;
  world_edit_brush_linked_list_node *Bucket = GetBucketByNameBuf(Table, Key);
  if (Bucket)
  {
    Assert(Bucket->Tombstoned == False);
    Bucket->Tombstoned = True;
    Result = True;
  }
  return Result;
}

link_internal b32
Drop( world_edit_brush_hashtable *Table, cs Key )
{
  return Tombstone(Key, Table, 0);
}

