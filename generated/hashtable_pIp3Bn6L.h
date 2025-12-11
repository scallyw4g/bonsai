// callsite
// src/engine/editor.h:1880:0

// def (hashtable)
// external/bonsai_stdlib/src/poof_functions.h:790:0
struct prefab_linked_list_node
{
  b32 Tombstoned;
  prefab Element;
  prefab_linked_list_node *Next;
};

struct prefab_hashtable
poof(
  @collection
   @serdes 
   @do_editor_ui 
)
{
  umm Size;
  prefab_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};
link_internal b32 AreEqual(prefab_linked_list_node *Node1, prefab_linked_list_node *Node2 );
link_internal b32 AreEqual(prefab *Element1, prefab *Element2 );

link_internal prefab_linked_list_node *
Allocate_prefab_linked_list_node(memory_arena *Memory)
{
  prefab_linked_list_node *Result = Allocate( prefab_linked_list_node, Memory, 1);
  return Result;
}

link_internal prefab_hashtable
Allocate_prefab_hashtable(umm ElementCount, memory_arena *Memory)
{
  prefab_hashtable Result = {
    .Elements = Allocate( prefab_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal prefab_linked_list_node *
GetHashBucket(umm HashValue, prefab_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  prefab_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal prefab *
GetFirstAtBucket(umm HashValue, prefab_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  prefab_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  prefab *Result = &Bucket->Element;
  return Result;
}

link_internal prefab_linked_list_node**
GetMatchingBucket(prefab Element, prefab_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  prefab_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal prefab *
Insert(prefab_linked_list_node *Node, prefab_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  prefab_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    /* Assert(!AreEqual(&Bucket[0]->Element, &Node->Element)); */
    Bucket = &(*Bucket)->Next;
  }
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal prefab*
Insert(prefab Element, prefab_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  prefab_linked_list_node *Bucket = Allocate_prefab_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

link_internal prefab*
Upsert(prefab Element, prefab_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  prefab_linked_list_node **Bucket = Table->Elements + HashValue;
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

struct prefab_hashtable_iterator
{
  umm HashIndex;
  prefab_hashtable *Table;
  prefab_linked_list_node *Node;
};

link_internal cs
CS( prefab_hashtable_iterator I )
{
  return CSz("Element");
}


link_internal prefab_hashtable_iterator
operator++( prefab_hashtable_iterator &Iterator )
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
operator<( prefab_hashtable_iterator I0, prefab_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline prefab_hashtable_iterator
ZerothIndex(prefab_hashtable *Hashtable)
{
  prefab_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline prefab_hashtable_iterator
AtElements(prefab_hashtable *Hashtable)
{
  prefab_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline prefab *
GetPtr(prefab_hashtable *Hashtable, prefab_hashtable_iterator Iterator)
{
  prefab *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}

link_inline prefab *
TryGetPtr(prefab_hashtable *Hashtable, prefab_hashtable_iterator Iterator)
{
  return GetPtr(Hashtable, Iterator);
}

 
