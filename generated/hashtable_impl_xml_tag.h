    bonsai_function xml_tag_linked_list_node *
    Allocate_xml_tag_linked_list_node(memory_arena *Memory)
    {
      xml_tag_linked_list_node *Result = Allocate( xml_tag_linked_list_node, Memory, 1);
      return Result;
    }

    bonsai_function xml_tag_hashtable
    Allocate_xml_tag_hashtable(umm ElementCount, memory_arena *Memory)
    {
      xml_tag_hashtable Result = {};
      Result.Elements = Allocate( xml_tag_linked_list_node*, Memory, ElementCount);
      Result.Size = ElementCount;
      return Result;
    }

    bonsai_function xml_tag_linked_list_node *
    GetHashBucket(umm HashValue, xml_tag_hashtable *Table)
    {
      Assert(Table->Size);
      xml_tag_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
      return Result;
    }

    bonsai_function xml_tag *
    GetFirstAtBucket(umm HashValue, xml_tag_hashtable *Table)
    {
      xml_tag_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
      xml_tag *Result = &Bucket->Element;
      return Result;
    }

    bonsai_function xml_tag *
    Insert(xml_tag_linked_list_node *Node, xml_tag_hashtable *Table)
    {
      Assert(Table->Size);
      umm HashValue = Hash(&Node->Element) % Table->Size;
      xml_tag_linked_list_node **Bucket = Table->Elements + HashValue;
      while (*Bucket) Bucket = &(*Bucket)->Next;
      *Bucket = Node;
      return &Bucket[0]->Element;
    }

    bonsai_function xml_tag*
    Insert(xml_tag Element, xml_tag_hashtable *Table, memory_arena *Memory)
    {
      xml_tag_linked_list_node *Bucket = Allocate_xml_tag_linked_list_node(Memory);
      Bucket->Element = Element;
      Insert(Bucket, Table);
      return &Bucket->Element;
    }

