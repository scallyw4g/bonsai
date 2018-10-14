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

  // TODO(Jesse): This could be factored out of here if the xml_tag.Properties
  // thing was a xml_token_stream* - which would require a two-pass parser most
  // likely.  ie. Tokenizer and AST builder
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

struct xml_token_stream
{
  xml_token* Start;
  xml_token* At;
  xml_token* End;

  hashtable<xml_tag*> Hashes;
};

struct xml_tag_stream
{
  xml_tag** Start;
  xml_tag** At;
  xml_tag** End;
};


xml_tag_stream
AllocateXmlTagStream(umm TagCount, memory_arena* Memory)
{
  xml_tag_stream Result = {};
  Result.Start = Allocate(xml_tag*, Memory, TagCount);
  Result.At = Result.Start;
  Result.End = Result.Start + TagCount;

  return Result;
}
xml_token_stream
AllocateXmlTokenStream(umm TokenCount, memory_arena* Memory)
{
  xml_token_stream Result = {};
  Result.Start = Allocate(xml_token, Memory, TokenCount);
  Result.At = Result.Start;
  Result.End = Result.Start + TokenCount;

  // TODO(Jesse): Profile this and see if it's reasonable
  Result.Hashes = AllocateHashtable<xml_tag*>(TokenCount/10, Memory);

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

xml_property*
XmlProperty(counted_string Name, counted_string Value, memory_arena* Memory)
{
  xml_property* Prop = Allocate(xml_property, Memory, 1);
  Prop->Name = Name;
  Prop->Value = Value;
  return Prop;
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

xml_tag*
XmlTag(xml_token* Open, xml_tag *Parent, umm HashValue, memory_arena* Memory)
{
  xml_tag* Result = Allocate(xml_tag, Memory, 1);
  Result->Open = Open;
  Result->Parent = Parent;
  Result->HashValue = HashValue;
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
  umm Result = Hash(&Token->Property.Name); // + Hash(&Token->Property.Value);
  return Result;
}
