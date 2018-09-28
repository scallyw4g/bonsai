#include <bonsai_hashtable.h>

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
};

struct xml_token
{
  xml_token_type Type;
  xml_property Property;
};

struct xml_tag
{
  xml_token* Open;
  xml_token* Close;
  xml_tag* Parent;
  xml_tag* Sibling;

  umm HashValue;

  xml_tag* Next; // TODO(Jesse): Can this be factored out of here?  It's only
                 // purpose is to walk the chain in the hash table.  Maybe
                 // convert to do local probing instead of a linked-list?

  struct xml_token_stream* Properties;
};

struct xml_token_stream
{
  xml_token* Start;
  xml_token* At;
  xml_token* End;

  hashtable<xml_tag> Hashes;
};

xml_token_stream
AllocateXmlTokenStream(umm TokenCount, memory_arena* Memory)
{
  xml_token_stream Result = {};
  Result.Start = Allocate(xml_token, Memory, TokenCount);
  Result.At = Result.Start;
  Result.End = Result.Start + TokenCount;

  // TODO(Jesse): Profile this and see if it's reasonable
  Result.Hashes = AllocateHashtable<xml_tag>(TokenCount/10, Memory);

  return Result;
}

inline xml_token*
PushToken(xml_token_stream* Stream, xml_token Token)
{
  Assert(Stream->At < Stream->End);
  *Stream->At = Token;
  xml_token* Result = Stream->At;
  Stream->At++;
  return Result;
}

xml_token
XmlCloseToken(counted_string Name)
{
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
XmlBooleanProperty(counted_string Name)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_Boolean;
  Result.Property.Name = Name;
  return Result;
}

xml_token
XmlStringProperty(counted_string Name, counted_string Value)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_Property;
  Result.Property = XmlProperty(Name, Value);
  return Result;
}

xml_token
XmlFloatProperty(counted_string Name, counted_string Value)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_Float;
  Result.Property = XmlProperty(Name, Value);
  return Result;
}

xml_token
XmlIntProperty(counted_string Name, counted_string Value)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_Int;
  Result.Property = XmlProperty(Name, Value);
  return Result;
}

xml_tag
XmlOpenTag(xml_token* Open, xml_tag *Parent, umm HashValue)
{
  xml_tag Result = {};
  Result.Open = Open;
  Result.Parent = Parent;
  Result.HashValue = HashValue;
  return Result;
}

inline umm
Hash(counted_string* String)
{
  umm Result = 0;

  for (umm CharIndex = 0;
      CharIndex < String->Count;
      ++CharIndex)
  {
    // TODO(Jesse): Better Hash Function!
    Result = Result ^ 0x056A09B3220F6F2D + String->Start[CharIndex] + (String->Start[CharIndex] << 8);
  }

  return Result;
}

inline umm
Hash(xml_token* Token)
{
  umm Result = Hash(&Token->Property.Name) + Hash(&Token->Property.Value);
  return Result;
}
