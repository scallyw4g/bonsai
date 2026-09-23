// callsite
// external/bonsai_stdlib/src/work_queue.h:20:0

// def (hashtable)
// external/bonsai_stdlib/src/poof_functions.h:801:0
struct work_queue_job_stats_linked_list_node
{
  b32 Tombstoned;
  work_queue_job_stats Element;
  work_queue_job_stats_linked_list_node *Next;
};

struct work_queue_job_stats_hashtable
poof(
  @collection
  
  
)
{
  umm Size;
  work_queue_job_stats_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};
link_internal b32 AreEqual(work_queue_job_stats_linked_list_node *Node1, work_queue_job_stats_linked_list_node *Node2 );
link_internal b32 AreEqual(work_queue_job_stats *Element1, work_queue_job_stats *Element2 );

link_internal work_queue_job_stats_linked_list_node *
Allocate_work_queue_job_stats_linked_list_node(memory_arena *Memory)
{
  work_queue_job_stats_linked_list_node *Result = Allocate( work_queue_job_stats_linked_list_node, Memory, 1);
  return Result;
}

link_internal work_queue_job_stats_hashtable
Allocate_work_queue_job_stats_hashtable(umm ElementCount, memory_arena *Memory)
{
  work_queue_job_stats_hashtable Result = {
    .Elements = Allocate( work_queue_job_stats_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal work_queue_job_stats_linked_list_node *
GetHashBucket(umm HashValue, work_queue_job_stats_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  work_queue_job_stats_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal work_queue_job_stats *
GetFirstAtBucket(umm HashValue, work_queue_job_stats_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  work_queue_job_stats_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  work_queue_job_stats *Result = &Bucket->Element;
  return Result;
}

link_internal work_queue_job_stats_linked_list_node**
GetMatchingBucket(work_queue_job_stats Element, work_queue_job_stats_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  work_queue_job_stats_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    if (AreEqual(&Bucket[0]->Element, &Element)) { break; }
    Bucket = &(*Bucket)->Next;
  }
  return Bucket;
}

link_internal work_queue_job_stats *
Insert(work_queue_job_stats_linked_list_node *Node, work_queue_job_stats_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  work_queue_job_stats_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket)
  {
    /* Assert(!AreEqual(&Bucket[0]->Element, &Node->Element)); */
    Bucket = &(*Bucket)->Next;
  }
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal work_queue_job_stats*
Insert(work_queue_job_stats Element, work_queue_job_stats_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  work_queue_job_stats_linked_list_node *Bucket = Allocate_work_queue_job_stats_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

link_internal work_queue_job_stats*
Upsert(work_queue_job_stats Element, work_queue_job_stats_hashtable *Table, memory_arena *Memory)
{
  umm HashValue = Hash(&Element) % Table->Size;
  work_queue_job_stats_linked_list_node **Bucket = Table->Elements + HashValue;
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

struct work_queue_job_stats_hashtable_iterator
{
  umm HashIndex;
  work_queue_job_stats_hashtable *Table;
  work_queue_job_stats_linked_list_node *Node;
};



link_internal work_queue_job_stats_hashtable_iterator
operator++( work_queue_job_stats_hashtable_iterator &Iterator )
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
operator<( work_queue_job_stats_hashtable_iterator I0, work_queue_job_stats_hashtable_iterator I1)
{
  b32 Result = I0.HashIndex < I1.HashIndex;
  return Result;
}

link_inline work_queue_job_stats_hashtable_iterator
ZerothIndex(work_queue_job_stats_hashtable *Hashtable)
{
  work_queue_job_stats_hashtable_iterator Iterator = {};
  Iterator.Table = Hashtable;
  Iterator.Node = Hashtable->Elements[0];
  return Iterator;
}

link_inline work_queue_job_stats_hashtable_iterator
AtElements(work_queue_job_stats_hashtable *Hashtable)
{
  work_queue_job_stats_hashtable_iterator Result = { Hashtable->Size, 0, 0 };
  return Result;
}

link_inline work_queue_job_stats *
GetPtr(work_queue_job_stats_hashtable *Hashtable, work_queue_job_stats_hashtable_iterator Iterator)
{
  work_queue_job_stats *Result = {};
  if (Iterator.Node)
  {
    Result = &Iterator.Node->Element;
  }
  return Result;
}

link_inline work_queue_job_stats *
TryGetPtr(work_queue_job_stats_hashtable *Hashtable, work_queue_job_stats_hashtable_iterator Iterator)
{
  return GetPtr(Hashtable, Iterator);
}

 
