struct xml_tag
{
  counted_string Selector;

  umm OpeningStartOffset; // Relative to start of stream
  // umm OpeningEndOffset;   // Relative to start of stream

  /* umm ClosingStartOffset; // Relative to start of stream */
  /* umm ClosingEndOffset;   // Relative to start of stream */
};

enum xml_token_type
{
  XmlTokenType_Unknown,

  XmlTokenType_Boolean,
  XmlTokenType_Float,
  XmlTokenType_Int,
  XmlTokenType_Property,
  XmlTokenType_OpenTag,
  XmlTokenType_CloseTag,

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

  union
  {
    counted_string OpenTag;
    counted_string CloseTag;
    counted_string Boolean;
    xml_property Property;
  };
};

struct xml_token_stream
{
  xml_token* Start;
  xml_token* At;
  xml_token* End;
};

xml_token_stream
AllocateXmlTokenStream(umm TokenCount, memory_arena* Memory)
{
  xml_token_stream Result = {};
  Result.Start = Allocate(xml_token, Memory, TokenCount);
  Result.At = Result.Start;
  Result.End = Result.Start + TokenCount;

  return Result;
}

inline void
PushToken(xml_token_stream* Stream, xml_token Token)
{
  Assert(Stream->At < Stream->End);
  *Stream->At++ = Token;
  return;
}

xml_token
XmlCloseTag(counted_string Name)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_CloseTag;
  Result.CloseTag = Name;
  return Result;
}

xml_token
XmlOpenTag(counted_string Name)
{
  xml_token Result = {};
  Result.Type = XmlTokenType_OpenTag;
  Result.OpenTag = Name;
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
  Result.Boolean = Name;
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

