// external/bonsai_stdlib/src/ui/ui.cpp:20:0

maybe_window_layout
GetByHashtableKey( window_layout_hashtable *Table, ui_id HashtableKey )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_window_layout Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&HashtableKey)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (AreEqual(E->HashtableKey, HashtableKey))
    {
      Result.Tag = Maybe_Yes;
      Result.Value = *E;
      break;
    }
    else
    {
      Bucket = Bucket->Next;
    }
  }

  return Result;
}

