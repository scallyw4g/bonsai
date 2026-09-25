// callsite
// src/engine/bonsai_type_info.h:37:0

// def (hashtable)
// external/bonsai_stdlib/src/poof_functions.h:801:0
struct bonsai_type_info_linked_list_node
{
  b32 Tombstoned;
  bonsai_type_info Element;
  bonsai_type_info_linked_list_node *Next;
};

struct bonsai_type_info_hashtable
poof(
  @collection
  
  
)
{
  u32 Size;
  u32 Pad;
  bonsai_type_info_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};
link_internal b32 AreEqual(bonsai_type_info_linked_list_node *Node1, bonsai_type_info_linked_list_node *Node2 );
link_internal b32 AreEqual(bonsai_type_info *Element1, bonsai_type_info *Element2 );

link_internal bonsai_type_info_linked_list_node *
Allocate_bonsai_type_info_linked_list_node(memory_arena *Memory)
{
  bonsai_type_info_linked_list_node *Result = Allocate( bonsai_type_info_linked_list_node, Memory, 1);
  return Result;
}

link_internal bonsai_type_info_hashtable
Allocate_bonsai_type_info_hashtable(u32 ElementCount, memory_arena *Memory)
{
  bonsai_type_info_hashtable Result = {
    .Elements = Allocate( bonsai_type_info_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    .Pad = 0,
  };
  return Result;
}

link_internal bonsai_type_info_linked_list_node *
GetHashBucket(u32 HashValue, bonsai_type_info_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  bonsai_type_info_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal bonsai_type_info *
GetFirstAtBucket(u32 HashValue, bonsai_type_info_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  bonsai_type_info_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  bonsai_type_info *Result = &Bucket->Element;
  return Result;
}

link_internal bonsai_type_info_linked_list_node**
GetMatchingBucket(bonsai_type_info Element, bonsai_type_info_hashtable *Table, memory_arena *Memory)
{
  u32 HashValue = Hash(&Element) % Table->Size;
  bonsai_type_info_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal bonsai_type_info *
InsertBlank(u32 HashValue, bonsai_type_info_hashtable *Table, memory_arena *Memory)
{
  Assert(Table->Size);
  bonsai_type_info_linked_list_node **Bucket = Table->Elements + (HashValue % Table->Size);
  bonsai_type_info_linked_list_node  *Node   = Allocate_bonsai_type_info_linked_list_node(Memory);
  Node->Next = *Bucket;
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal bonsai_type_info*
Insert(bonsai_type_info Element, bonsai_type_info_hashtable *Table, memory_arena *Memory)
{
  u32 HashValue = Hash(&Element) % Table->Size;
  auto Result = InsertBlank(HashValue, Table, Memory);
  *Result = Element;
  return Result;
}

link_internal bonsai_type_info*
Upsert(bonsai_type_info Element, bonsai_type_info_hashtable *Table, memory_arena *Memory)
{
  u32 HashValue = Hash(&Element) % Table->Size;
  bonsai_type_info_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }

  bonsai_type_info *Result = {};
  if (*Bucket && Bucket[0]->Tombstoned == False)
  {
    Bucket[0]->Element = Element;
    Result = &Bucket[0]->Element;
  }
  else
  {
    Result = Insert(Element, Table, Memory);
  }

  return Result;
}

//
// Get
//

/* Type.member(@hashtable_key, (key_member) { @var key key_member }); */

/* link_internal (Type.name) * */
/* GetByKey( (Type.name)_hashtable *Table, key.type KeyQuery ) */
/* { */
/*   (Type.name) *Result = {}; */
/*   (Type.name)_linked_list_node *Bucket = GetBucketBy(key.name)(Table, KeyQuery); */
/*   while (Bucket) */
/*   { */
/*     Type.is_primitive? */
/*     { if (Bucket->Tombstoned == False && AreEqual(*E, KeyQuery)) } */
/*     { if (Bucket->Tombstoned == False && AreEqual(E->key.name, KeyQuery)) } */
/*     { */
/*       Result = &Bucket->Element; */
/*       break; */
/*     } */
/*     else */
/*     { */
/*       Bucket = Bucket->Next; */
/*     } */
/*   } */

/*   return Result; */
/* } */

//
// Iterator impl.
//

struct bonsai_type_info_hashtable_iterator
{
  u32 HashIndex;
  bonsai_type_info_hashtable *Table;
  bonsai_type_info_linked_list_node *Node;
};



link_internal bonsai_type_info_hashtable_iterator
operator++( bonsai_type_info_hashtable_iterator &Iterator )
{
  if (Iterator.Node)
  {
    Iterator.Node = Iterator.Node->Next;
  }
  else
  {
    Assert (Iterator.HashIndex < Iterator.Table->Size );
    Iterator.Node = Iterator.Table->Elements[++Iterator.HashIndex];
  }

  return Iterator;
}

link_internal b32
operator<( bonsai_type_info_hashtable_iterator I0, bonsai_type_info_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline bonsai_type_info_hashtable_iterator
ZerothIndex(bonsai_type_info_hashtable *Hashtable)
{
  bonsai_type_info_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline bonsai_type_info_hashtable_iterator
AtElements(bonsai_type_info_hashtable *Hashtable)
{
  bonsai_type_info_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline bonsai_type_info *
GetPtr(bonsai_type_info_hashtable *Hashtable, bonsai_type_info_hashtable_iterator Iterator)
{
  bonsai_type_info *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}

link_inline bonsai_type_info *
TryGetPtr(bonsai_type_info_hashtable *Hashtable, bonsai_type_info_hashtable_iterator Iterator)
{
  return GetPtr(Hashtable, Iterator);
}

 
