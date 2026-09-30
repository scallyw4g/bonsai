// callsite
// external/bonsai_stdlib/src/c_parser.cpp:1:0

// def (hashtable)
// external/bonsai_stdlib/src/poof_functions.h:801:0
struct parser_linked_list_node
{
  b32 Tombstoned;
  parser Element;
  parser_linked_list_node *Next;
};

struct parser_hashtable
poof(
  @collection
  
  
)
{
  u32 Size;
  u32 Pad;
  parser_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};
link_internal b32 AreEqual(parser_linked_list_node *Node1, parser_linked_list_node *Node2 );
link_internal b32 AreEqual(parser *Element1, parser *Element2 );

link_internal parser_linked_list_node *
Allocate_parser_linked_list_node(memory_arena *Memory)
{
  parser_linked_list_node *Result = Allocate( parser_linked_list_node, Memory, 1);
  return Result;
}

link_internal parser_hashtable
Allocate_parser_hashtable(u32 ElementCount, memory_arena *Memory)
{
  parser_hashtable Result = {
    .Elements = Allocate( parser_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    .Pad = 0,
  };
  return Result;
}

link_internal parser_linked_list_node *
GetHashBucket(u32 HashValue, parser_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  parser_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal parser *
GetFirstAtBucket(u32 HashValue, parser_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  parser_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  parser *Result = &Bucket->Element;
  return Result;
}

link_internal parser_linked_list_node**
GetMatchingBucket(parser Element, parser_hashtable *Table, memory_arena *Memory)
{
  u32 HashValue = Hash(&Element) % Table->Size;
  parser_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal parser *
InsertBlank(u32 HashValue, parser_hashtable *Table, memory_arena *Memory)
{
  Assert(Table->Size);
  parser_linked_list_node **Bucket = Table->Elements + (HashValue % Table->Size);
  parser_linked_list_node  *Node   = Allocate_parser_linked_list_node(Memory);
  Node->Next = *Bucket;
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal parser*
Insert(parser Element, parser_hashtable *Table, memory_arena *Memory)
{
  u32 HashValue = Hash(&Element) % Table->Size;
  auto Result = InsertBlank(HashValue, Table, Memory);
  *Result = Element;
  return Result;
}

link_internal parser*
Upsert(parser Element, parser_hashtable *Table, memory_arena *Memory)
{
  u32 HashValue = Hash(&Element) % Table->Size;
  parser_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }

  parser *Result = {};
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

struct parser_hashtable_iterator
{
  u32 HashIndex;
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

link_inline parser *
TryGetPtr(parser_hashtable *Hashtable, parser_hashtable_iterator Iterator)
{
  return GetPtr(Hashtable, Iterator);
}

 
