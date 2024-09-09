// src/engine/voxel_synthesis.h:158:0

struct voxel_synth_tile_linked_list_node
{
  b32 Tombstoned;
  voxel_synth_tile Element;
  voxel_synth_tile_linked_list_node *Next;
};

struct voxel_synth_tile_hashtable
{
  umm Size;
  voxel_synth_tile_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};
link_internal b32 AreEqual(voxel_synth_tile_linked_list_node *Node1, voxel_synth_tile_linked_list_node *Node2 );

link_internal voxel_synth_tile_linked_list_node *
Allocate_voxel_synth_tile_linked_list_node(memory_arena *Memory)
{
  voxel_synth_tile_linked_list_node *Result = Allocate( voxel_synth_tile_linked_list_node, Memory, 1);
  return Result;
}

link_internal voxel_synth_tile_hashtable
Allocate_voxel_synth_tile_hashtable(umm ElementCount, memory_arena *Memory)
{
  voxel_synth_tile_hashtable Result = {
    .Elements = Allocate( voxel_synth_tile_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal voxel_synth_tile_linked_list_node *
GetHashBucket(umm HashValue, voxel_synth_tile_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  voxel_synth_tile_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal voxel_synth_tile *
GetFirstAtBucket(umm HashValue, voxel_synth_tile_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  voxel_synth_tile_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  voxel_synth_tile *Result = &Bucket->Element;
  return Result;
}

link_internal voxel_synth_tile_linked_list_node**
GetMatchingBucket(voxel_synth_tile Element, voxel_synth_tile_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  voxel_synth_tile_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal voxel_synth_tile *
Insert(voxel_synth_tile_linked_list_node *Node, voxel_synth_tile_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  voxel_synth_tile_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    /* Assert(!AreEqual(*Bucket, Node)); */
    Bucket = &(*Bucket)->Next;
  }
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal voxel_synth_tile*
Insert(voxel_synth_tile Element, voxel_synth_tile_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  voxel_synth_tile_linked_list_node *Bucket = Allocate_voxel_synth_tile_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

link_internal voxel_synth_tile*
Upsert(voxel_synth_tile Element, voxel_synth_tile_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  voxel_synth_tile_linked_list_node **Bucket = Table->Elements + HashValue;
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

struct voxel_synth_tile_hashtable_iterator
{
  umm HashIndex;
  voxel_synth_tile_hashtable *Table;
  voxel_synth_tile_linked_list_node *Node;
};

link_internal voxel_synth_tile_hashtable_iterator
operator++( voxel_synth_tile_hashtable_iterator &Iterator )
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
operator<( voxel_synth_tile_hashtable_iterator I0, voxel_synth_tile_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline voxel_synth_tile_hashtable_iterator
ZerothIndex(voxel_synth_tile_hashtable *Hashtable)
{
  voxel_synth_tile_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline voxel_synth_tile_hashtable_iterator
AtElements(voxel_synth_tile_hashtable *Hashtable)
{
  voxel_synth_tile_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline voxel_synth_tile *
GetPtr(voxel_synth_tile_hashtable *Hashtable, voxel_synth_tile_hashtable_iterator Iterator)
{
  voxel_synth_tile *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}
 
