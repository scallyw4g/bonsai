maybe_ui_toggle_ptr
GetPtrById( ui_toggle_hashtable *Table, umm Id )
{
  maybe_ui_toggle_ptr Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Id)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (AreEqual(E->Id, Id))
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

