// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:39:0

// def (hashtable_get_by_key)
// external/bonsai_stdlib/src/poof_functions.h:1021:0
//
// Get
//





link_internal ui_texture_ops *
GetByKey( ui_texture_ops_hashtable *Table, texture *KeyQuery )
{
  ui_texture_ops *Result = {};

  u32 HashValue = Hash(KeyQuery);
  ui_texture_ops_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  while (Bucket)
  {
     if (Bucket->Tombstoned == False && AreEqual( &Bucket->Element.Texture , KeyQuery)) 
    {
      Result = &Bucket->Element;
      break;
    }
    else
    {
      Bucket = Bucket->Next;
    }
  }

  return Result;
}

