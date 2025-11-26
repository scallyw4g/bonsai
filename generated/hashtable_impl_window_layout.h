// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:29:0

// def (hashtable_impl)
// external/bonsai_stdlib/src/poof_functions.h:817:0
link_internal b32 AreEqual(window_layout_linked_list_node *Node1, window_layout_linked_list_node *Node2 );
link_internal b32 AreEqual(window_layout *Element1, window_layout *Element2 );

link_internal window_layout_linked_list_node *
Allocate_window_layout_linked_list_node(memory_arena *Memory)
{
  window_layout_linked_list_node *Result = Allocate( window_layout_linked_list_node, Memory, 1);
  return Result;
}

link_internal window_layout_hashtable
Allocate_window_layout_hashtable(umm ElementCount, memory_arena *Memory)
{
  window_layout_hashtable Result = {
    .Elements = Allocate( window_layout_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal window_layout_linked_list_node *
GetHashBucket(umm HashValue, window_layout_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  window_layout_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal window_layout *
GetFirstAtBucket(umm HashValue, window_layout_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  window_layout_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  window_layout *Result = &Bucket->Element;
  return Result;
}

link_internal window_layout_linked_list_node**
GetMatchingBucket(window_layout Element, window_layout_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  window_layout_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal window_layout *
Insert(window_layout_linked_list_node *Node, window_layout_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  window_layout_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    /* Assert(!AreEqual(&Bucket[0]->Element, &Node->Element)); */
    Bucket = &(*Bucket)->Next;
  }
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal window_layout*
Insert(window_layout Element, window_layout_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  window_layout_linked_list_node *Bucket = Allocate_window_layout_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

link_internal window_layout*
Upsert(window_layout Element, window_layout_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  window_layout_linked_list_node **Bucket = Table->Elements + HashValue;
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

struct window_layout_hashtable_iterator
{
  umm HashIndex;
  window_layout_hashtable *Table;
  window_layout_linked_list_node *Node;
};

link_internal cs
CS( window_layout_hashtable_iterator I )
{
  return CSz("Element");
}


link_internal window_layout_hashtable_iterator
operator++( window_layout_hashtable_iterator &Iterator )
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
operator<( window_layout_hashtable_iterator I0, window_layout_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline window_layout_hashtable_iterator
ZerothIndex(window_layout_hashtable *Hashtable)
{
  window_layout_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline window_layout_hashtable_iterator
AtElements(window_layout_hashtable *Hashtable)
{
  window_layout_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline window_layout *
GetPtr(window_layout_hashtable *Hashtable, window_layout_hashtable_iterator Iterator)
{
  window_layout *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}

link_inline window_layout *
TryGetPtr(window_layout_hashtable *Hashtable, window_layout_hashtable_iterator Iterator)
{
  return GetPtr(Hashtable, Iterator);
}


