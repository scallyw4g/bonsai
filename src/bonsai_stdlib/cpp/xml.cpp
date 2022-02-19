meta(hashtable_impl(xml_tag));
#include <metaprogramming/output/hashtable_impl_xml_tag.h>

inline counted_string*
GetPropertyValue(xml_tag* Tag, counted_string PropertyName)
{
  xml_property* At = Tag->Properties;
  counted_string* Result = 0;

  while (At)
  {
    if (StringsMatch(At->Name, PropertyName))
    {
      Result = &At->Value;
      break;
    }

    At = At->Next;
  }

  return Result;
}

inline void
PushProperty(xml_tag* Target, xml_property *Prop)
{
  if (!Target->Properties)
  {
    Target->NextPropertySlot = &Target->Properties;
  }

  *Target->NextPropertySlot = Prop;
  Target->NextPropertySlot = &Prop->Next;

  return;
}

b32
PropertiesMatch(xml_property* P1, xml_property* P2)
{
  b32 Result = StringsMatch(&P1->Name, &P2->Name);
  if (P1->Value.Count || P2->Value.Count)
  {
    Result = Result && StringsMatch(&P1->Value, &P2->Value);
  }
  return Result;
}

b32
TokensAreEqual(xml_token* T1, xml_token* T2)
{
  b32 Result = False;

  if (T1->Type == T2->Type)
  {
    switch (T1->Type)
    {
      case XmlTokenType_Float:
      case XmlTokenType_Int:
      case XmlTokenType_Property:
      case XmlTokenType_Open:
      case XmlTokenType_Close:
      case XmlTokenType_Boolean:
      {
        Result = PropertiesMatch(&T1->Property, &T2->Property);
      } break;

      InvalidDefaultCase;
    }
  }

  return Result;
}

b32
TagsAreEqual(xml_tag* QueryTag, xml_tag* T2)
{
  b32 Result = False;

  if (QueryTag && T2 && TokensAreEqual(QueryTag->Open, T2->Open))
  {
    Result = True;

    xml_property *QueryProperty = QueryTag->Properties;
    while (QueryProperty)
    {
      Result = False;
      xml_property *CurrentProperty = T2->Properties;
      while (CurrentProperty)
      {
        if (PropertiesMatch(QueryProperty, CurrentProperty))
        {
          Result = True;
          break;
        }

        CurrentProperty = CurrentProperty->Next;
      }

      if (!Result)
      {
        break;
      }

      QueryProperty = QueryProperty->Next;
    }

  }

  return Result;
}

xml_tag
XmlTagFromReverseStream(xml_token_stream** Stream)
{
  xml_tag Result = {};

  while ((*Stream)->At->Type == XmlTokenType_Property)
  {
    PushProperty(&Result, &(*Stream)->At->Property);
    --(*Stream)->At;
    Assert((*Stream)->At >= (*Stream)->Start);
  }

  Assert((*Stream)->At->Type == XmlTokenType_Open);
  Result.Open = (*Stream)->At;

  if((*Stream)->At > (*Stream)->Start)
  {
    --(*Stream)->At;
  }


  Assert((*Stream)->At >= (*Stream)->Start);
  return Result;
}

xml_token_stream
AllocateXmlTokenStream(umm TokenCount, memory_arena* Memory)
{
  xml_token_stream Result = {};
  Result.Start = Allocate(xml_token, Memory, TokenCount);
  Result.At = Result.Start;
  Result.End = Result.Start + TokenCount;

  // TODO(Jesse, id: 147, tags: hashing): Profile this and see if it's reasonable
  Result.Hashes = Allocate_xml_tag_hashtable(TokenCount/10, Memory);

  return Result;
}

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
TokenizeSelector(ansi_stream* Selector, memory_arena* Memory)
{
  // TODO(Jesse, id: 148, tags: over_allocation): Better or more accurate way of allocating this size?
  xml_token_stream Result = AllocateXmlTokenStream(1000, Memory);

  while (Remaining(Selector))
  {
    counted_string TagName = PopWordCounted(Selector);
    counted_string TagProperties = Split(&TagName, ':');

    counted_string TagId = Split(&TagName, '#');
    PushToken(&Result, XmlOpenToken(TagName));
    if (TagId.Count)
    {
      PushToken(&Result, XmlPropertyToken(CS("id"), TagId));
    }

    while (TagProperties.Count)
    {
      counted_string PropName = TagProperties;
      TagProperties = Split(&PropName, ':');
      counted_string PropValue = Split(&PropName, '=');

      PushToken(&Result, XmlPropertyToken(PropName, PropValue));
    }
  }

  Result.End = Result.At;
  return Result;
}

inline u32
CountTagsInHashBucket(xml_tag *Tag)
{
  u32 Count = 0;
  while (Tag)
  {
    ++Count;
    Tag = Tag->NextInHash;
  }

  return Count;
}

xml_tag_stream
GetCountMatchingTags(xml_token_stream* Tokens, xml_token_stream* Selectors, u32 Count, memory_arena* Memory)
{
  b32 Valid = True;
  Selectors->At = Selectors->End-1;

  xml_token_stream FirstSelectorStream = *Selectors;
  xml_tag FirstSelector = XmlTagFromReverseStream(&Selectors);

  xml_tag *RootTag = GetFirst(&FirstSelector, &Tokens->Hashes);

  u32 MaxTagCount = CountTagsInHashBucket(RootTag);
  xml_tag_stream Result = AllocateXmlTagStream(MaxTagCount, Memory);

  while (Count && RootTag)
  {
    xml_tag *CurrentTag = RootTag;
    *Selectors = FirstSelectorStream;
    xml_tag CurrentSelector = XmlTagFromReverseStream(&Selectors);

    while (CurrentTag)
    {
      if (TagsAreEqual(&CurrentSelector, CurrentTag))
      {
        Valid = True;
      }
      else
      {
        Valid = False;
      }

      if (Valid)
      {
        if (Remaining(Selectors) < TotalElements(Selectors))
          CurrentSelector = XmlTagFromReverseStream(&Selectors);
        else
          break;
      }

      CurrentTag = CurrentTag->Parent;
    }

    if (Valid)
    {
      Push(RootTag, &Result);
      Assert(Result.At <= Result.End);

      --Count;
    }

    RootTag = RootTag->NextInHash;
  }


  Assert(Result.At <= Result.End);
  Result.End = Result.At;

  return Result;
}

xml_tag_stream
GetAllMatchingTags(xml_token_stream* Tokens, xml_token_stream* Selectors, memory_arena* Memory)
{
  xml_tag_stream Result = GetCountMatchingTags(Tokens, Selectors, u32_MAX, Memory);
  return Result;
}

xml_tag*
GetFirstMatchingTag(xml_token_stream* Tokens, xml_token_stream* Selectors)
{
  xml_tag_stream ResultStream = GetCountMatchingTags(Tokens, Selectors, 1, TranArena);
  xml_tag* Result = ResultStream.Start[0];
  return Result;
}

xml_tag*
GetFirstMatchingTag(xml_token_stream* Tokens, counted_string* SelectorString)
{
  ansi_stream SelectorStream = AnsiStream(SelectorString);
  xml_token_stream Selectors = TokenizeSelector(&SelectorStream, TranArena);
  xml_tag* Result = GetFirstMatchingTag(Tokens, &Selectors);
  return Result;
}

xml_tag_stream
GetAllMatchingTags(xml_token_stream* Tokens, counted_string* SelectorString, memory_arena* Memory)
{
  ansi_stream SelectorStream = AnsiStream(SelectorString);
  xml_token_stream Selectors = TokenizeSelector(&SelectorStream, Memory);
  xml_tag_stream Result = GetAllMatchingTags(Tokens, &Selectors, Memory);
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

xml_tag*
XmlTag(xml_token* Open, xml_tag *Parent, umm HashValue, memory_arena* Memory)
{
  xml_tag* Result = Allocate(xml_tag, Memory, 1);
  Result->Open = Open;
  Result->Parent = Parent;
  Result->HashValue = HashValue;
  return Result;
}

xml_token_stream
TokenizeXmlStream(ansi_stream* Xml, memory_arena* Memory)
{
  xml_token_stream Result = {};

  EatWhitespace(Xml);
  if (*Xml->At != '<') { Error("Invalid XML Stream"); return Result; }

  // TODO(Jesse, id: 149, tags: over_allocation): Better way of allocating this?
  Result = AllocateXmlTokenStream(10000, Memory);

  xml_parsing_at_indicators TagsAt = {};
  while ( Remaining(Xml) )
  {
    const char* NameDelimeters = "\r\n> </";

    b32 StreamValueIsTag = Xml->At[0] == '<' || Xml->At[0] == '/';

    b32 StreamValueIsCloseTag = StreamValueIsTag && (Xml->At[0] == '/' || Xml->At[1] == '/');
    b32 StreamValueIsOpenTag = StreamValueIsTag && (!StreamValueIsCloseTag);

    if (StreamValueIsTag)
    {
      ++Xml->At;
    }

    if (Xml->At[0] == '/')
    {
      ++Xml->At;
    }


    counted_string StreamValue = {};

    if (StreamValueIsOpenTag)
    {
      StreamValue = ReadUntilTerminatorList(Xml, NameDelimeters);
      Assert(!StreamValueIsCloseTag);
      umm HashValue = Hash(&StreamValue) % Result.Hashes.Size;

      b32 IsSelfClosingTag = Xml->At[-1] == '/';

      xml_token* OpenToken = PushToken(&Result, XmlOpenToken(StreamValue));
      xml_tag* OpenTag = XmlTag(OpenToken, TagsAt.CurrentlyOpenTag, HashValue, Memory);

      Insert(OpenTag, &Result.Hashes);

      TagsAt.CurrentlyOpenTag = OpenTag;
      if (TagsAt.LastClosedTag)
      {
        TagsAt.LastClosedTag->Sibling = OpenTag;
      }

      if (IsSelfClosingTag)
      {
        ++Xml->At;
        PushToken(&Result, XmlCloseToken(StreamValue, &TagsAt));
      }
    }
    else if (StreamValueIsCloseTag)
    {
      StreamValue = ReadUntilTerminatorList(Xml, NameDelimeters);
      Assert(!StreamValueIsOpenTag);
      PushToken(&Result, XmlCloseToken(StreamValue, &TagsAt));
    }
    else
    {
      StreamValue = Trim(ReadUntilTerminatorList(Xml, "<"));
      TagsAt.CurrentlyOpenTag->Value = StreamValue;
      EatWhitespace(Xml);
      continue;
    }

    while ( *(Xml->At-1) != '>' )
    {
      EatWhitespace(Xml);
      const char* PropertyDelimeters = "\r\n> =";
      counted_string PropertyName = ReadUntilTerminatorList(Xml, PropertyDelimeters);

      if (PropertyName.Count)
      {
        char TerminatorFound = *(Xml->At-1);
        char NextTokenStartChar = *Xml->At;

        switch(TerminatorFound)
        {
          case '>':
          case ' ':
          case '\n':
          case '\r':
          {
            if (PropertyName.Count == 1 && PropertyName.Start[0] == '/')
            {
              PushToken(&Result, XmlCloseToken(StreamValue, &TagsAt));
            }
            else
            {
              PushToken(&Result, XmlBooleanToken(PropertyName) );
            }
          } break;

          case '=':
          {
            switch(NextTokenStartChar)
            {
              case '"':
              case '\'':
              {
                counted_string PropValue = PopQuotedString(Xml);
                PushToken(&Result, XmlPropertyToken(PropertyName, PropValue));
                PushProperty(TagsAt.CurrentlyOpenTag, XmlProperty(PropertyName, PropValue, Memory));
              } break;

              default:
              {
                counted_string PropValue = PopWordCounted(Xml);
                if( Contains(PropValue, '.') )
                  PushToken(&Result, XmlFloatToken(PropertyName, PropValue));
                else
                  PushToken(&Result, XmlIntToken(PropertyName, PropValue));
              } break;

            }
          } break;

          InvalidDefaultCase;
        }
      }

    }

    EatWhitespace(Xml);
  }

  return Result;
}

inline void
Rewind(xml_token_stream *Stream)
{
  Stream->At = Stream->Start;
  return;
}

