struct counted_string_linked_list_node
{
  counted_string Element;
  counted_string_linked_list_node *Next;
};

struct counted_string_hashtable
{
  umm Size;
  counted_string_linked_list_node **Elements;
};
link_internal counted_string_linked_list_node *
Allocate_counted_string_linked_list_node(memory_arena *Memory)
{
  counted_string_linked_list_node *Result = Allocate( counted_string_linked_list_node, Memory, 1);
  return Result;
}

link_internal counted_string_hashtable
Allocate_counted_string_hashtable(umm ElementCount, memory_arena *Memory)
{
  counted_string_hashtable Result = {};
  Result.Elements = Allocate( counted_string_linked_list_node*, Memory, ElementCount);
  Result.Size = ElementCount;
  return Result;
}

link_internal counted_string_linked_list_node *
GetHashBucket(umm HashValue, counted_string_hashtable *Table)
{
  Assert(Table->Size);
  counted_string_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal counted_string *
GetFirstAtBucket(umm HashValue, counted_string_hashtable *Table)
{
  counted_string_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  counted_string *Result = &Bucket->Element;
  return Result;
}

link_internal counted_string *
Insert(counted_string_linked_list_node *Node, counted_string_hashtable *Table)
{
  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  counted_string_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket) Bucket = &(*Bucket)->Next;
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal counted_string*
Insert(counted_string Element, counted_string_hashtable *Table, memory_arena *Memory)
{
  counted_string_linked_list_node *Bucket = Allocate_counted_string_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

