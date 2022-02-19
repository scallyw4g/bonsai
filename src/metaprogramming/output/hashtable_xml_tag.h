    struct xml_tag_hashtable
    {
      umm Size;
      xml_tag **Table;
    };

    bonsai_function xml_tag_hashtable
    Allocate_hashtable_xml_tag(umm ElementCount)
    {
      xml_tag_hashtable Result = {};
      Result.Table = Allocate(xml_tag*, Memory, ElementCount);
      Result.Size = ElementCount;
      return Result;
    }

