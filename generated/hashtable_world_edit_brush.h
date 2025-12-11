// callsite
// src/engine/editor.h:1743:0

// def (hashtable)
// external/bonsai_stdlib/src/poof_functions.h:790:0
struct world_edit_brush_linked_list_node
{
  b32 Tombstoned;
  world_edit_brush Element;
  world_edit_brush_linked_list_node *Next;
};

struct world_edit_brush_hashtable
poof(
  @collection
   @serdes 
   @do_editor_ui 
)
{
  umm Size;
  world_edit_brush_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};
link_internal b32 AreEqual(world_edit_brush_linked_list_node *Node1, world_edit_brush_linked_list_node *Node2 );
link_internal b32 AreEqual(world_edit_brush *Element1, world_edit_brush *Element2 );

link_internal world_edit_brush_linked_list_node *
Allocate_world_edit_brush_linked_list_node(memory_arena *Memory)
{
  world_edit_brush_linked_list_node *Result = Allocate( world_edit_brush_linked_list_node, Memory, 1);
  return Result;
}

link_internal world_edit_brush_hashtable
Allocate_world_edit_brush_hashtable(umm ElementCount, memory_arena *Memory)
{
  world_edit_brush_hashtable Result = {
    .Elements = Allocate( world_edit_brush_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal world_edit_brush_linked_list_node *
GetHashBucket(umm HashValue, world_edit_brush_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  world_edit_brush_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal world_edit_brush *
GetFirstAtBucket(umm HashValue, world_edit_brush_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  world_edit_brush_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  world_edit_brush *Result = &Bucket->Element;
  return Result;
}

link_internal world_edit_brush_linked_list_node**
GetMatchingBucket(world_edit_brush Element, world_edit_brush_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  world_edit_brush_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal world_edit_brush *
Insert(world_edit_brush_linked_list_node *Node, world_edit_brush_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  world_edit_brush_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    /* Assert(!AreEqual(&Bucket[0]->Element, &Node->Element)); */
    Bucket = &(*Bucket)->Next;
  }
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal world_edit_brush*
Insert(world_edit_brush Element, world_edit_brush_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  world_edit_brush_linked_list_node *Bucket = Allocate_world_edit_brush_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

link_internal world_edit_brush*
Upsert(world_edit_brush Element, world_edit_brush_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  world_edit_brush_linked_list_node **Bucket = Table->Elements + HashValue;
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

struct world_edit_brush_hashtable_iterator
{
  umm HashIndex;
  world_edit_brush_hashtable *Table;
  world_edit_brush_linked_list_node *Node;
};

link_internal cs
CS( world_edit_brush_hashtable_iterator I )
{
  return CSz("Element");
}


link_internal world_edit_brush_hashtable_iterator
operator++( world_edit_brush_hashtable_iterator &Iterator )
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
operator<( world_edit_brush_hashtable_iterator I0, world_edit_brush_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline world_edit_brush_hashtable_iterator
ZerothIndex(world_edit_brush_hashtable *Hashtable)
{
  world_edit_brush_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline world_edit_brush_hashtable_iterator
AtElements(world_edit_brush_hashtable *Hashtable)
{
  world_edit_brush_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline world_edit_brush *
GetPtr(world_edit_brush_hashtable *Hashtable, world_edit_brush_hashtable_iterator Iterator)
{
  world_edit_brush *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}

link_inline world_edit_brush *
TryGetPtr(world_edit_brush_hashtable *Hashtable, world_edit_brush_hashtable_iterator Iterator)
{
  return GetPtr(Hashtable, Iterator);
}

 
