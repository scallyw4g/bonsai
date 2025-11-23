// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:34:0

// def (hashtable_get_ptr)
// external/bonsai_stdlib/src/poof_functions.h:1080:0
maybe_window_layout_ptr
GetPtrByHashtableKey( window_layout_hashtable *Table, ui_id Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_window_layout_ptr Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Query)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (Bucket->Tombstoned == False && AreEqual(E->HashtableKey, Query))
    {
      Result.Tag = Maybe_Yes;
      Result.Value = E;
      break;
    }
    else
    {
      Bucket = Bucket->Next;
    }
  }

  return Result;


}

