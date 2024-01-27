struct type_info_linked_list_node
{
  type_info Element;
  type_info_linked_list_node *Next;
};

struct type_info_hashtable
{
  umm Size;
  type_info_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};
link_internal type_info_linked_list_node *
Allocate_type_info_linked_list_node(memory_arena *Memory)
{
  type_info_linked_list_node *Result = Allocate( type_info_linked_list_node, Memory, 1);
  return Result;
}

link_internal type_info_hashtable
Allocate_type_info_hashtable(umm ElementCount, memory_arena *Memory)
{
  type_info_hashtable Result = {
    .Elements = Allocate( type_info_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal type_info_linked_list_node *
GetHashBucket(umm HashValue, type_info_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  type_info_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal type_info *
GetFirstAtBucket(umm HashValue, type_info_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  type_info_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  type_info *Result = &Bucket->Element;
  return Result;
}

link_internal type_info *
Insert(type_info_linked_list_node *Node, type_info_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  type_info_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket) Bucket = &(*Bucket)->Next;
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal type_info*
Insert(type_info Element, type_info_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  type_info_linked_list_node *Bucket = Allocate_type_info_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

//
// Iterator impl.
//

struct type_info_hashtable_iterator
{
  umm HashIndex;
  type_info_hashtable *Table;
  type_info_linked_list_node *Node;
};

link_internal type_info_hashtable_iterator
operator++( type_info_hashtable_iterator &Iterator )
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
operator<( type_info_hashtable_iterator I0, type_info_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline type_info_hashtable_iterator
ZerothIndex(type_info_hashtable *Hashtable)
{
  type_info_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline type_info_hashtable_iterator
AtElements(type_info_hashtable *Hashtable)
{
  type_info_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline type_info *
GetPtr(type_info_hashtable *Hashtable, type_info_hashtable_iterator Iterator)
{
  type_info *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}
 
