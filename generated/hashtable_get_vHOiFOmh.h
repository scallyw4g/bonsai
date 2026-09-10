// callsite
// src/engine/editor.h:2034:0

// def (hashtable_get_ptr)
// external/bonsai_stdlib/src/poof_functions.h:1080:0
maybe_prefab_ptr
GetPtrByName( prefab_hashtable *Table, cs Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_prefab_ptr Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Query)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (Bucket->Tombstoned == False && AreEqual(E->Name, Query))
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

