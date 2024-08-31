// external/bonsai_stdlib/src/ui/ui.cpp:22:0

maybe_window_layout_ptr
GetPtrByHashtableKey( window_layout_hashtable *Table, ui_id HashtableKey )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_window_layout_ptr Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&HashtableKey)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (AreEqual(E->HashtableKey, HashtableKey))
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

