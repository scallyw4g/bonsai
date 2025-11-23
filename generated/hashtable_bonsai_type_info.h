// callsite
// src/engine/bonsai_type_info.h:37:0

// def (hashtable)
// external/bonsai_stdlib/src/poof_functions.h:790:0
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
  umm Size;
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
Allocate_bonsai_type_info_hashtable(umm ElementCount, memory_arena *Memory)
{
  bonsai_type_info_hashtable Result = {
    .Elements = Allocate( bonsai_type_info_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal bonsai_type_info_linked_list_node *
GetHashBucket(umm HashValue, bonsai_type_info_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  bonsai_type_info_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal bonsai_type_info *
GetFirstAtBucket(umm HashValue, bonsai_type_info_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  bonsai_type_info_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  bonsai_type_info *Result = &Bucket->Element;
  return Result;
}

link_internal bonsai_type_info_linked_list_node**
GetMatchingBucket(bonsai_type_info Element, bonsai_type_info_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  bonsai_type_info_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal bonsai_type_info *
Insert(bonsai_type_info_linked_list_node *Node, bonsai_type_info_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  bonsai_type_info_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    /* Assert(!AreEqual(&Bucket[0]->Element, &Node->Element)); */
    Bucket = &(*Bucket)->Next;
  }
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal bonsai_type_info*
Insert(bonsai_type_info Element, bonsai_type_info_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  bonsai_type_info_linked_list_node *Bucket = Allocate_bonsai_type_info_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

link_internal bonsai_type_info*
Upsert(bonsai_type_info Element, bonsai_type_info_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  bonsai_type_info_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }

  if (*Bucket && Bucket[0]->Tombstoned == False)
  {
    Bucket[0]->Element = Element;
  }
  else
  {
    Insert(Element, Table, Memory);
  }

  return &Bucket[0]->Element;
}


//
// Iterator impl.
//

struct bonsai_type_info_hashtable_iterator
{
  umm HashIndex;
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

 
