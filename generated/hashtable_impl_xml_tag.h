
link_internal xml_tag_linked_list_node *
Allocate_xml_tag_linked_list_node(memory_arena *Memory)
{
  xml_tag_linked_list_node *Result = Allocate( xml_tag_linked_list_node, Memory, 1);
  return Result;
}

link_internal xml_tag_hashtable
Allocate_xml_tag_hashtable(umm ElementCount, memory_arena *Memory)
{
  xml_tag_hashtable Result = {
    .Elements = Allocate( xml_tag_linked_list_node*, Memory, ElementCount),
    .Size = ElementCount,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal xml_tag_linked_list_node *
GetHashBucket(umm HashValue, xml_tag_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  xml_tag_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal xml_tag *
GetFirstAtBucket(umm HashValue, xml_tag_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  xml_tag_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  xml_tag *Result = &Bucket->Element;
  return Result;
}

link_internal xml_tag *
Insert(xml_tag_linked_list_node *Node, xml_tag_hashtable *Table)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  xml_tag_linked_list_node **Bucket = Table->Elements + HashValue;
  while (*Bucket) Bucket = &(*Bucket)->Next;
  *Bucket = Node;
  return &Bucket[0]->Element;
}

link_internal xml_tag*
Insert(xml_tag Element, xml_tag_hashtable *Table, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  xml_tag_linked_list_node *Bucket = Allocate_xml_tag_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}

