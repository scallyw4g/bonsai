maybe_ui_toggle
GetById( ui_toggle_hashtable *Table, ui_id Id )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_ui_toggle Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Id)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (AreEqual(E->Id, Id))
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

