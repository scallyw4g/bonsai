enum xml_token_type
{
  XmlTokenType_Unknown,

  XmlTokenType_Boolean,
  XmlTokenType_Float,
  XmlTokenType_Int,
  XmlTokenType_Property,
  XmlTokenType_Open,
  XmlTokenType_Close,

  XmlTokenType_Count
};

struct xml_property
{
  counted_string Name;
  counted_string Value;

  /* TODO(Jesse, id: 80, tags: xml, back_burner): This could be factored out of
   * here if xml_tag::Properties thing was a xml_token_stream* - which would
   * require a two-pass parser most likely.  ie. Tokenizer and AST builder
   */
  xml_property* Next;
};

struct xml_token
{
  xml_token_type Type;
  xml_property Property;
};

struct xml_tag
{
  xml_token* Open;

  xml_tag* Parent;
  xml_tag* Sibling;
  xml_tag* NextInHash;

  umm HashValue;
  counted_string Value;

  xml_property* Properties;
  xml_property** NextPropertySlot;
};

struct xml_hashtable
{
  umm Size;
  xml_tag** Table;
};

struct xml_token_stream
{
  xml_token* Start;
  xml_token* At;
  xml_token* End;

  xml_hashtable Hashes;
};

struct xml_tag_stream
{
  xml_tag** Start;
  xml_tag** At;
  xml_tag** End;
};

inline xml_token*
PushToken(xml_token_stream* Stream, xml_token Token)
{
  Assert(Stream->At < Stream->End);
  *Stream->At = Token;
  xml_token* Result = Stream->At;
  Stream->At++;
  return Result;
}

struct xml_parsing_at_indicators
{
  xml_tag* LastClosedTag;
  xml_tag* CurrentlyOpenTag;
};

xml_token
XmlCloseToken(counted_string Name, xml_parsing_at_indicators* TagsAt)
{
  if (TagsAt)
  {
    TagsAt->LastClosedTag = TagsAt->CurrentlyOpenTag;
    TagsAt->CurrentlyOpenTag = TagsAt->CurrentlyOpenTag->Parent;
  }

  xml_token Result = {};
  Result.Type = XmlTokenType_Close;
  Result.Property.Name = Name;
  return Result;
}

xml_token
XmlOpenToken(counted_string Name)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_Open;
  Result.Property.Name = Name;
  return Result;
}

xml_property
XmlProperty(counted_string Name, counted_string Value)
{
  xml_property Prop = {};
  Prop.Name = Name;
  Prop.Value = Value;
  return Prop;
}

xml_token
XmlBooleanToken(counted_string Name)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_Boolean;
  Result.Property.Name = Name;
  return Result;
}

xml_token
XmlPropertyToken(counted_string Name, counted_string Value)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_Property;
  Result.Property = XmlProperty(Name, Value);
  return Result;
}

xml_token
XmlFloatToken(counted_string Name, counted_string Value)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_Float;
  Result.Property = XmlProperty(Name, Value);
  return Result;
}

xml_token
XmlIntToken(counted_string Name, counted_string Value)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_Int;
  Result.Property = XmlProperty(Name, Value);
  return Result;
}

// Shamelessly ripped from https://cp-algorithms.com/string/string-hashing.html

/* TODO(Jesse, id: 81, tags: string_hash): Theres' a null terminated
 * string-hash function that should be replaced by this one most likely
 */
inline umm
Hash(counted_string* String)
{
  umm Result = 0;

#if 1
  /* TODO(Jesse, id: 82, tags: robustness, rng, hashing): 257 seemed to produce
   * slightly worse results, but the source literature seemed to indicate using
   * a constant close to the total number of discrete characters you'll
   * encounter is advisable.  I believe this to be somewhat higher than 53, but
   * it would be worth testing on real-world data (source files).
   */
  u32 p = 53;
  u32 m = (u32)1e9 + 9;

  u64 p_pow = 1;

  for (umm CharIndex = 0;
      CharIndex < String->Count;
      ++CharIndex)
  {
    umm C = (umm)String->Start[CharIndex];
    Result = (Result + (C - (umm)('a' + 1)) * p_pow) % m;
    p_pow = (p_pow*p) % m;
  }
#else
  // Note(Jesse): This is just some random thing I came up with that I thought
  // sounded good ..it's worse than the implementation I got off the internet,
  // but I though I'd leave it here for posterity.

  u64 A = 6364136223846793005;
  u64 B = 1442695040888963407;
  u64 One = 1;
  u64 Mod = (One << 63);

  u32 Remainder = (u32)(String->Count % sizeof(u32));

  for (umm CharIndex = 0;
      CharIndex < String->Count;
      ++CharIndex)
  {
    Result = ((A * (umm)String->Start[CharIndex]) + B) % Mod;
  }

  u32 IterationCount = (u32)(String->Count / sizeof(u32));
  u32* Data = (u32*)String->Start+Remainder;
  for (umm IterIndex = 0;
      IterIndex < IterationCount;
      ++IterIndex)
  {
    Result = ((A * (umm)Data[IterIndex]) + B) % Mod;
  }

  Result = (u32)(Result >> 31);

#endif

  return Result;
}

inline umm
Hash(xml_token* Token)
{
  umm Result = Hash(&Token->Property.Name); // + Hash(&Token->Property.Value);
  return Result;
}
