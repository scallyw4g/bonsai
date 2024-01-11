struct parser_linked_list_node
{
  parser Element;
  parser_linked_list_node *Next;
};

struct parser_hashtable
{
  umm Size;
  parser_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};
link_internal parser_linked_list_node *
Allocate_parser_linked_list_node(memory_arena *Memory)
{
  parser_linked_list_node *Result = Allocate( parser_linked_list_node, Memory, 1);
  return Result;
}

link_internal parser_hashtable
Allocate_parser_hashtable(umm ElementCount, memory_arena *Memory)
{
  parser_hashtable Result = {
    .Elements = Allocate( parser_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal parser_linked_list_node *
GetHashBucket(umm HashValue, parser_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  parser_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal parser *
GetFirstAtBucket(umm HashValue, parser_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  parser_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  parser *Result = &Bucket->Element;
  return Result;
}

link_internal parser *
Insert(parser_linked_list_node *Node, parser_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  parser_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket) Bucket = &(*Bucket)->Next;
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal parser*
Insert(parser Element, parser_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  parser_linked_list_node *Bucket = Allocate_parser_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

//
// Iterator impl.
//

struct parser_hashtable_iterator
{
  umm HashIndex;
  parser_hashtable *Table;
  parser_linked_list_node *Node;
};

link_internal parser_hashtable_iterator
operator++( parser_hashtable_iterator &Iterator )
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
operator<( parser_hashtable_iterator I0, parser_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline parser_hashtable_iterator
ZerothIndex(parser_hashtable *Hashtable)
{
  parser_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline parser_hashtable_iterator
AtElements(parser_hashtable *Hashtable)
{
  parser_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline parser *
GetPtr(parser_hashtable *Hashtable, parser_hashtable_iterator Iterator)
{
  parser *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}
 
