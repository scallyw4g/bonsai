// NOTE(Jesse): This has to be here instead of in the .h file because it does a string compare
// Sad times.
b32
operator==(xml_token T1, xml_token T2)
{
  b32 Result = False;

  if (T1.Type == T2.Type)
  {
    switch (T1.Type)
    {
      case XmlTokenType_Float:
      case XmlTokenType_Int:
      case XmlTokenType_Property:
      {
        Result = StringsMatch(&T1.Property.Name, &T2.Property.Name) &&
                 StringsMatch(&T1.Property.Value, &T2.Property.Value);
      } break;

      case XmlTokenType_Open:
      case XmlTokenType_Close:
      case XmlTokenType_Boolean:
      {
        Result = StringsMatch(&T1.Property.Name, &T2.Property.Name);
        if (T1.Property.Id.Count || T2.Property.Id.Count)
        {
          Result = Result && StringsMatch(&T1.Property.Id, &T2.Property.Id);
        }
      } break;

      InvalidDefaultCase;
    }
  }

  return Result;
}

b32
operator!=(xml_token T1, xml_token T2)
{
  b32 Result = !(T1 == T2);
  return Result;
}

#if 0
umm
XmlSelectorHash(xml_token_stream* Selectors, umm TargetHashSize)
{
  umm SelectorHashValue = {};

  s32 SelectorCount = Count(Selectors);

  for (umm SelectorIndex = 0;
      SelectorIndex < SelectorCount;
      ++SelectorIndex)
  {
    xml_token* Selector = Selectors->Start + SelectorIndex;
    SelectorHashValue = (SelectorHashValue + Hash(Selector)) % TargetHashSize;
  }

  SelectorHashValue = SelectorHashValue % TargetHashSize;

  return SelectorHashValue;
}
#endif

xml_tag*
GetNextMatchingRootTag(xml_tag* Current, xml_token* SearchTag)
{
  xml_tag* Result = 0;

  while (Current)
  {
    if (Current->Open && // TODO(Jesse): Should this not be an Assert(Current->Open) ?
        *SearchTag == *Current->Open)
    {
      Result = Current;
      break;
    }
    else
    {
      Current = Current->NextInHash;
    }
  }

  return Result;
}

xml_tag*
GetFirstMatchingTag(xml_token_stream* Tokens, xml_token_stream* Selectors)
{
  b32 Valid = True;
  s32 MaxSelectorIndex = Count(Selectors) -1;

  umm SelectorHash = Hash(Selectors->Start + MaxSelectorIndex) % Tokens->Hashes.Size;
  xml_token* FirstSelector = Selectors->Start + MaxSelectorIndex;

  xml_tag* RootTag = GetNextMatchingRootTag(Tokens->Hashes.Table[SelectorHash], FirstSelector);

  while (RootTag)
  {
    xml_tag *CurrentTag = RootTag->Parent;
    for (s32 SelectorIndex = MaxSelectorIndex-1; // We've already checked the first selector
        SelectorIndex >= 0;
        --SelectorIndex)
    {
      xml_token* CurrentSelector = &Selectors->Start[SelectorIndex];

      if (CurrentTag && CurrentTag->Open &&
          *CurrentSelector == *CurrentTag->Open)
      {
        CurrentTag = CurrentTag->Parent;
        Valid = True;
      }
      else
      {
        while (CurrentTag)
        {
          if (CurrentTag->Open &&
              *CurrentSelector == *CurrentTag->Open)
          {
            CurrentTag = CurrentTag->Parent;
            Valid = True;
            break;
          }
          else
          {
            CurrentTag = CurrentTag->Sibling;
            Valid = False;
          }
        }

        if (!Valid)
        {
          RootTag = GetNextMatchingRootTag(RootTag->NextInHash, FirstSelector);
          break;
        }
      }
    }

    if (Valid)
      break;
  }

  xml_tag* Result = Valid ? RootTag : 0;
  return Result;
}

xml_token_stream
TokenizeXmlStream(ansi_stream* Xml, memory_arena* Memory)
{
  xml_token_stream Result = {};

  EatWhitespace(Xml);
  if (*Xml->At != '<') { Error("Invalid XML Stream"); return Result; }

  // TODO(Jesse): Better way of allocating this?
  Result = AllocateXmlTokenStream(10000, Memory);

  xml_tag* Parent = 0;
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
      xml_tag* OpenTag = XmlTag(OpenToken, Parent, HashValue, Memory);

      if (IsSelfClosingTag)
      {
        PushToken(&Result, XmlCloseToken(StreamValue));
        ++Xml->At;
      }

      xml_tag** Bucket = Result.Hashes.Table + HashValue;
      while (*Bucket) Bucket = &(*Bucket)->NextInHash;

      *Bucket = OpenTag;
      if (Parent)
      {
        Parent->Sibling = OpenTag;
      }
      Parent = OpenTag;
    }
    else if (StreamValueIsCloseTag)
    {
      StreamValue = CS(ReadUntilTerminatorList(Xml, NameDelimeters, Memory));
      Assert(!StreamValueIsOpenTag);
      PushToken(&Result, XmlCloseToken(StreamValue));
      Parent = Parent->Parent;
    }
    else
    {
      StreamValue = CS(ReadUntilTerminatorList(Xml, "<", Memory));
      Trim(&StreamValue);
      Parent->Value = StreamValue;
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
              PushToken(&Result, XmlCloseToken(StreamValue));
              Parent = Parent->Parent;
            }
            else
            {
              PushToken(&Result, XmlBooleanProperty(PropertyName) );
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
                if (StringsMatch(&PropertyName, CS("id")))
                {
                  Parent->Open->Property.Id = PropValue;
                }
                else
                {
                  PushToken(&Result, XmlStringProperty(PropertyName, PropValue));
                }
              } break;

              default:
              {
                counted_string PropValue = PopWordCounted(Xml);
                if( Contains(PropValue, '.') )
                  PushToken(&Result, XmlFloatProperty(PropertyName, PropValue));
                else
                  PushToken(&Result, XmlIntProperty(PropertyName, PropValue));
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

xml_token_stream
TokenizeSelector(ansi_stream* Selector, memory_arena* Memory)
{
  // TODO(Jesse): Better or more accurate way of allocating this size?
  xml_token_stream Result = AllocateXmlTokenStream(100, Memory);

  while (Remaining(Selector))
  {
    counted_string TagName = PopWordCounted(Selector);
    counted_string TagId = Split(&TagName, '#');

    PushToken(&Result, XmlOpenToken(TagName, TagId));
  }

  return Result;
}

inline void
Rewind(xml_token_stream *Stream)
{
  Stream->At = Stream->Start;
  return;
}

