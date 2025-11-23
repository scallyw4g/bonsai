// callsite
// external/bonsai_stdlib/src/primitive_containers.cpp:5:0

// def (hashtable_impl)
// external/bonsai_stdlib/src/poof_functions.h:817:0
link_internal b32 AreEqual(u32_linked_list_node *Node1, u32_linked_list_node *Node2 );
link_internal b32 AreEqual(u32 *Element1, u32 *Element2 );

link_internal u32_linked_list_node *
Allocate_u32_linked_list_node(memory_arena *Memory)
{
  u32_linked_list_node *Result = Allocate( u32_linked_list_node, Memory, 1);
  return Result;
}

link_internal u32_hashtable
Allocate_u32_hashtable(umm ElementCount, memory_arena *Memory)
{
  u32_hashtable Result = {
    .Elements = Allocate( u32_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal u32_linked_list_node *
GetHashBucket(umm HashValue, u32_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  u32_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal u32 *
GetFirstAtBucket(umm HashValue, u32_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  u32_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  u32 *Result = &Bucket->Element;
  return Result;
}

link_internal u32_linked_list_node**
GetMatchingBucket(u32 Element, u32_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  u32_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal u32 *
Insert(u32_linked_list_node *Node, u32_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  u32_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    /* Assert(!AreEqual(&Bucket[0]->Element, &Node->Element)); */
    Bucket = &(*Bucket)->Next;
  }
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal u32*
Insert(u32 Element, u32_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  u32_linked_list_node *Bucket = Allocate_u32_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

link_internal u32*
Upsert(u32 Element, u32_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  u32_linked_list_node **Bucket = Table->Elements + HashValue;
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

struct u32_hashtable_iterator
{
  umm HashIndex;
  u32_hashtable *Table;
  u32_linked_list_node *Node;
};



link_internal u32_hashtable_iterator
operator++( u32_hashtable_iterator &Iterator )
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
operator<( u32_hashtable_iterator I0, u32_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline u32_hashtable_iterator
ZerothIndex(u32_hashtable *Hashtable)
{
  u32_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline u32_hashtable_iterator
AtElements(u32_hashtable *Hashtable)
{
  u32_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline u32 *
GetPtr(u32_hashtable *Hashtable, u32_hashtable_iterator Iterator)
{
  u32 *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}

link_inline u32 *
TryGetPtr(u32_hashtable *Hashtable, u32_hashtable_iterator Iterator)
{
  return GetPtr(Hashtable, Iterator);
}


