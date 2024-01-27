maybe_bonsai_type_info
GetByName( bonsai_type_info_hashtable *Table, cs Name )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_bonsai_type_info Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Name)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (AreEqual(E->Name, Name))
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

