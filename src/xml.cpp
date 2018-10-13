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
      {
        Result = StringsMatch(&T1->Property.Name, &T2->Property.Name) &&
                 StringsMatch(&T1->Property.Value, &T2->Property.Value);
      } break;

      case XmlTokenType_Open:
      case XmlTokenType_Close:
      case XmlTokenType_Boolean:
      {
        Result = StringsMatch(&T1->Property.Name, &T2->Property.Name);
        if (T1->Property.Value.Count || T2->Property.Value.Count)
        {
          Result = Result && StringsMatch(&T1->Property.Value, &T2->Property.Value);
        }
      } break;

      InvalidDefaultCase;
    }
  }

  return Result;
}

b32
TagsAreEqual(xml_tag* T1, xml_tag* T2)
{
  b32 Result = False;

  if (T1 && T2 && TokensAreEqual(T1->Open, T2->Open))
  {
    Result = True;

    counted_string* Id1 = GetPropertyValue(T1, CS("id"));
    counted_string* Id2 = GetPropertyValue(T2, CS("id"));
    if ( (Id1 || Id2) && (!StringsMatch(Id1,Id2)) )
    {
      Result = False;
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

  if((*Stream)->At >= (*Stream)->Start)
  {
    --(*Stream)->At;
  }

  return Result;
}

xml_token_stream
TokenizeSelector(ansi_stream* Selector, memory_arena* Memory)
{
  // TODO(Jesse): Better or more accurate way of allocating this size?
  xml_token_stream Result = AllocateXmlTokenStream(100, Memory);

  while (Remaining(Selector))
  {
    counted_string TagName = PopWordCounted(Selector);
    counted_string TagId = Split(&TagName, '#');

    PushToken(&Result, XmlOpenToken(TagName));
    if (TagId.Count)
    {
      PushToken(&Result, XmlPropertyToken(CS("id"), TagId));
    }
  }

  Result.End = Result.At;
  return Result;
}

xml_tag*
GetFirstMatchingTag(xml_token_stream* Tokens, xml_token_stream* Selectors)
{
  b32 Valid = True;
  Selectors->At = Selectors->End-1;

  xml_token_stream FirstSelectorStream = *Selectors;
  xml_tag FirstSelector = XmlTagFromReverseStream(&Selectors);
  umm SelectorHash = Hash(FirstSelector.Open) % Tokens->Hashes.Size;

  xml_tag* RootTag = Tokens->Hashes.Table[SelectorHash];
  while (RootTag)
  {
    xml_tag *CurrentTag = RootTag;
    *Selectors = FirstSelectorStream;
    xml_tag CurrentSelector = XmlTagFromReverseStream(&Selectors);

    while (CurrentTag && Selectors->At >= Selectors->Start)
    {
      if (TagsAreEqual(&CurrentSelector, CurrentTag))
      {
        CurrentSelector = XmlTagFromReverseStream(&Selectors);
        Valid = True;
      }
      else
      {
        Valid = False;
      }

      CurrentTag = CurrentTag->Parent;
    }

    if (!Valid)
    {
      RootTag = RootTag->NextInHash;
    }
    else
    {
      break;
    }

  }

  xml_tag* Result = Valid ? RootTag : 0;
  return Result;
}

xml_tag*
GetFirstMatchingTag(xml_token_stream* Tokens, counted_string* Selectors, memory_arena* Memory)
{
  ansi_stream SelectorStream = AnsiStream(Selectors);
  xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
  xml_tag* ResultTag = GetFirstMatchingTag(Tokens, &Selector);

  return ResultTag;
}

xml_token_stream
TokenizeXmlStream(ansi_stream* Xml, memory_arena* Memory)
{
  xml_token_stream Result = {};

  EatWhitespace(Xml);
  if (*Xml->At != '<') { Error("Invalid XML Stream"); return Result; }

  // TODO(Jesse): Better way of allocating this?
  Result = AllocateXmlTokenStream(10000, Memory);

  xml_parsing_at_indicators TagsAt = {};
  while ( Remaining(Xml) )
  {
    const char* NameDelimeters = "\n> </";

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
      StreamValue = CS(ReadUntilTerminatorList(Xml, NameDelimeters, Memory));
      Assert(!StreamValueIsCloseTag);
      umm HashValue = Hash(&StreamValue) % Result.Hashes.Size;

      b32 IsSelfClosingTag = Xml->At[-1] == '/';

      xml_token* OpenToken = PushToken(&Result, XmlOpenToken(StreamValue));
      xml_tag* OpenTag = XmlTag(OpenToken, TagsAt.CurrentlyOpenTag, HashValue, Memory);
      xml_tag** Bucket = Result.Hashes.Table + HashValue;
      while (*Bucket) Bucket = &(*Bucket)->NextInHash;
      *Bucket = OpenTag;

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
      StreamValue = CS(ReadUntilTerminatorList(Xml, NameDelimeters, Memory));
      Assert(!StreamValueIsOpenTag);
      PushToken(&Result, XmlCloseToken(StreamValue, &TagsAt));
    }
    else
    {
      StreamValue = CS(ReadUntilTerminatorList(Xml, "<", Memory));
      Trim(&StreamValue);
      TagsAt.CurrentlyOpenTag->Value = StreamValue;
      EatWhitespace(Xml);
      continue;
    }

    const char* PropertyDelimeters = "\n> =";
    while ( *(Xml->At-1) != '>' )
    {
      EatWhitespace(Xml);
      counted_string PropertyName = CountedString(ReadUntilTerminatorList(Xml, PropertyDelimeters, Memory));

      if (PropertyName.Count)
      {
        char TerminatorFound = *(Xml->At-1);
        char NextTokenStartChar = *Xml->At;

        switch(TerminatorFound)
        {
          case '>':
          case ' ':
          case '\n':
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
                counted_string PropValue = PopQuotedString(Xml, Memory);
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

