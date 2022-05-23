    bonsai_function counted_string_linked_list_node *
    Allocate_counted_string_linked_list_node(memory_arena *Memory)
    {
      counted_string_linked_list_node *Result = Allocate( counted_string_linked_list_node, Memory, 1);
      return Result;
    }

    bonsai_function counted_string_hashtable
    Allocate_counted_string_hashtable(umm ElementCount, memory_arena *Memory)
    {
      counted_string_hashtable Result = {};
      Result.Elements = Allocate( counted_string_linked_list_node*, Memory, ElementCount);
      Result.Size = ElementCount;
      return Result;
    }

    bonsai_function counted_string_linked_list_node *
    GetHashBucket(umm HashValue, counted_string_hashtable *Table)
    {
      counted_string_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
      return Result;
    }

    bonsai_function counted_string *
    GetFirstAtBucket(umm HashValue, counted_string_hashtable *Table)
    {
      counted_string_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
      counted_string *Result = &Bucket->Element;
      return Result;
    }

    bonsai_function void
    Insert(counted_string_linked_list_node *E, counted_string_hashtable *Table)
    {
      umm HashValue = Hash(&E->Element) % Table->Size;
      counted_string_linked_list_node **Bucket = Table->Elements + HashValue;
      while (*Bucket) Bucket = &(*Bucket)->Next;
      *Bucket = E;
    }

    bonsai_function void
    Insert(counted_string E, counted_string_hashtable *Table, memory_arena *Memory)
    {
      counted_string_linked_list_node *Bucket = Allocate_counted_string_linked_list_node(Memory);
      Bucket->Element = E;
      Insert(Bucket, Table);
    }

