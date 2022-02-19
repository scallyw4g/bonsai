    bonsai_function xml_tag_hashtable
    Allocate_xml_tag_hashtable(umm ElementCount, memory_arena *Memory)
    {
      xml_tag_hashtable Result = {};
      Result.Elements = Allocate(xml_tag*, Memory, ElementCount);
      Result.Size = ElementCount;
      return Result;
    }

    bonsai_function xml_tag *
    GetFirst(umm HashValue, xml_tag_hashtable *Table)
    {
      xml_tag *Result = Table->Elements[HashValue % Table->Size];
      return Result;
    }

    bonsai_function xml_tag *
    GetFirst(xml_tag *E, xml_tag_hashtable *Table)
    {
      umm HashValue = Hash(E);
      xml_tag *Result = GetFirst(HashValue, Table);
      return Result;
    }

    bonsai_function void
    Insert(xml_tag *E, xml_tag_hashtable *Table)
    {
      umm HashValue = E->HashValue % Table->Size;
      xml_tag** Bucket = Table->Elements + HashValue;
      while (*Bucket) Bucket = &(*Bucket)->NextInHash;
      *Bucket = E;
    }

