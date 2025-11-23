// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:19:0

// def (hashtable_impl)
// external/bonsai_stdlib/src/poof_functions.h:817:0
link_internal b32 AreEqual(ui_toggle_linked_list_node *Node1, ui_toggle_linked_list_node *Node2 );
link_internal b32 AreEqual(ui_toggle *Element1, ui_toggle *Element2 );

link_internal ui_toggle_linked_list_node *
Allocate_ui_toggle_linked_list_node(memory_arena *Memory)
{
  ui_toggle_linked_list_node *Result = Allocate( ui_toggle_linked_list_node, Memory, 1);
  return Result;
}

link_internal ui_toggle_hashtable
Allocate_ui_toggle_hashtable(umm ElementCount, memory_arena *Memory)
{
  ui_toggle_hashtable Result = {
    .Elements = Allocate( ui_toggle_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal ui_toggle_linked_list_node *
GetHashBucket(umm HashValue, ui_toggle_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  ui_toggle_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal ui_toggle *
GetFirstAtBucket(umm HashValue, ui_toggle_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  ui_toggle_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  ui_toggle *Result = &Bucket->Element;
  return Result;
}

link_internal ui_toggle_linked_list_node**
GetMatchingBucket(ui_toggle Element, ui_toggle_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  ui_toggle_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal ui_toggle *
Insert(ui_toggle_linked_list_node *Node, ui_toggle_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  ui_toggle_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    /* Assert(!AreEqual(&Bucket[0]->Element, &Node->Element)); */
    Bucket = &(*Bucket)->Next;
  }
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal ui_toggle*
Insert(ui_toggle Element, ui_toggle_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  ui_toggle_linked_list_node *Bucket = Allocate_ui_toggle_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

link_internal ui_toggle*
Upsert(ui_toggle Element, ui_toggle_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  ui_toggle_linked_list_node **Bucket = Table->Elements + HashValue;
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

struct ui_toggle_hashtable_iterator
{
  umm HashIndex;
  ui_toggle_hashtable *Table;
  ui_toggle_linked_list_node *Node;
};

link_internal cs
CS( ui_toggle_hashtable_iterator I )
{
  return CSz("Element");
}


link_internal ui_toggle_hashtable_iterator
operator++( ui_toggle_hashtable_iterator &Iterator )
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
operator<( ui_toggle_hashtable_iterator I0, ui_toggle_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline ui_toggle_hashtable_iterator
ZerothIndex(ui_toggle_hashtable *Hashtable)
{
  ui_toggle_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline ui_toggle_hashtable_iterator
AtElements(ui_toggle_hashtable *Hashtable)
{
  ui_toggle_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline ui_toggle *
GetPtr(ui_toggle_hashtable *Hashtable, ui_toggle_hashtable_iterator Iterator)
{
  ui_toggle *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}

link_inline ui_toggle *
TryGetPtr(ui_toggle_hashtable *Hashtable, ui_toggle_hashtable_iterator Iterator)
{
  return GetPtr(Hashtable, Iterator);
}


