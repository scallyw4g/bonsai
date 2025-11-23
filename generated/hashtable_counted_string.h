// callsite
// external/bonsai_stdlib/src/counted_string.cpp:37:0

// def (hashtable)
// external/bonsai_stdlib/src/poof_functions.h:790:0
struct counted_string_linked_list_node
{
  b32 Tombstoned;
  counted_string Element;
  counted_string_linked_list_node *Next;
};

struct counted_string_hashtable
poof(
  @collection
  
  
)
{
  umm Size;
  counted_string_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};
link_internal b32 AreEqual(counted_string_linked_list_node *Node1, counted_string_linked_list_node *Node2 );
link_internal b32 AreEqual(counted_string *Element1, counted_string *Element2 );

link_internal counted_string_linked_list_node *
Allocate_counted_string_linked_list_node(memory_arena *Memory)
{
  counted_string_linked_list_node *Result = Allocate( counted_string_linked_list_node, Memory, 1);
  return Result;
}

link_internal counted_string_hashtable
Allocate_counted_string_hashtable(umm ElementCount, memory_arena *Memory)
{
  counted_string_hashtable Result = {
    .Elements = Allocate( counted_string_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal counted_string_linked_list_node *
GetHashBucket(umm HashValue, counted_string_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  counted_string_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal counted_string *
GetFirstAtBucket(umm HashValue, counted_string_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  counted_string_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  counted_string *Result = &Bucket->Element;
  return Result;
}

link_internal counted_string_linked_list_node**
GetMatchingBucket(counted_string Element, counted_string_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  counted_string_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal counted_string *
Insert(counted_string_linked_list_node *Node, counted_string_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  counted_string_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    /* Assert(!AreEqual(&Bucket[0]->Element, &Node->Element)); */
    Bucket = &(*Bucket)->Next;
  }
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal counted_string*
Insert(counted_string Element, counted_string_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  counted_string_linked_list_node *Bucket = Allocate_counted_string_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

link_internal counted_string*
Upsert(counted_string Element, counted_string_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  counted_string_linked_list_node **Bucket = Table->Elements + HashValue;
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

struct counted_string_hashtable_iterator
{
  umm HashIndex;
  counted_string_hashtable *Table;
  counted_string_linked_list_node *Node;
};



link_internal counted_string_hashtable_iterator
operator++( counted_string_hashtable_iterator &Iterator )
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
operator<( counted_string_hashtable_iterator I0, counted_string_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline counted_string_hashtable_iterator
ZerothIndex(counted_string_hashtable *Hashtable)
{
  counted_string_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline counted_string_hashtable_iterator
AtElements(counted_string_hashtable *Hashtable)
{
  counted_string_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline counted_string *
GetPtr(counted_string_hashtable *Hashtable, counted_string_hashtable_iterator Iterator)
{
  counted_string *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}

link_inline counted_string *
TryGetPtr(counted_string_hashtable *Hashtable, counted_string_hashtable_iterator Iterator)
{
  return GetPtr(Hashtable, Iterator);
}

 
