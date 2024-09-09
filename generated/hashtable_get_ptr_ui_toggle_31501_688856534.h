// external/bonsai_stdlib/src/ui/ui.cpp:13:0

maybe_ui_toggle_ptr
GetPtrById( ui_toggle_hashtable *Table, ui_id Id )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_ui_toggle_ptr Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Id)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (Bucket->Tombstoned == False && AreEqual(E->Id, Id))
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

