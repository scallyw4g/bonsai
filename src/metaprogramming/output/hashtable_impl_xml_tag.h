    bonsai_function xml_tag_hashtable
    Allocate_xml_tag_hashtable(umm ElementCount, memory_arena *Memory)
    {
      xml_tag_hashtable Result = {};
      Result.Elements = Allocate(xml_tag*, Memory, ElementCount);
      Result.Size = ElementCount;
      return Result;
    }

    bonsai_function xml_tag *
    GetByHash(umm HashValue, xml_tag_hashtable *Table)
    {
      xml_tag *Result = Table->Elements[HashValue % Table->Size];
      return Result;
    }

    bonsai_function void
    Insert(xml_tag *E, u64 HashValue, xml_tag_hashtable *Table)
    {
      Assert(Hash(E) == HashValue);
      HashValue = HashValue % Table->Size;

      xml_tag** Bucket = Table->Elements + HashValue;
      while (*Bucket) Bucket = &(*Bucket)->NextInHash;
      *Bucket = E;
    }

    bonsai_function void
    Insert(xml_tag *E, xml_tag_hashtable *Table)
    {
      umm HashValue = Hash(E) % Table->Size;
      xml_tag** Bucket = Table->Elements + HashValue;
      while (*Bucket) Bucket = &(*Bucket)->NextInHash;
      *Bucket = E;
    }

