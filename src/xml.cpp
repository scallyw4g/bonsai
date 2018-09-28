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
        Assert(!T1.Property.Value.Count);
        Assert(!T2.Property.Value.Count);
        Result = StringsMatch(&T1.Property.Name, &T2.Property.Name);
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

xml_tag*
GetFirstMatchingTag(xml_token_stream* Tokens, xml_token_stream* Selectors)
{
  xml_tag* Result = {};

  umm SelectorHash = XmlSelectorHash(Selectors, Tokens->Hashes.ElementCount);

  xml_tag* HashedTag = Tokens->Hashes.Elements[SelectorHash];

  while (HashedTag)
  {
    xml_tag *CurrentTag = HashedTag;

    b32 Valid = True;
    s32 MaxSelectorIndex = Count(Selectors) -1;
    for (s32 SelectorIndex = MaxSelectorIndex;
        SelectorIndex >= 0;
        --SelectorIndex)
    {
      xml_token* CurrentSelector = &Selectors->Start[SelectorIndex];

      // FIXME(Jesse): Do work to check #id-selector as well!
      if (CurrentTag && CurrentTag->Open &&
          *CurrentSelector == *CurrentTag->Open)
      {
        CurrentTag = CurrentTag->Parent;
      }
      else
      {
        b32 FoundMatchingTag = False;
        while (CurrentTag)
        {
          if ( CurrentTag->Open && *CurrentSelector == *CurrentTag->Open)
          {
            FoundMatchingTag = True;
            break;
          }
          else
          {
            CurrentTag = CurrentTag->Sibling;
          }
        }

        Valid = FoundMatchingTag;
        break;
      }
    }


    if (Valid)
    {
      Result = HashedTag;
      break;
    }
    else
    {
      HashedTag = HashedTag->Sibling;
    }
  }

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
    const char* NameDelimeters = "\n> <";

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
      umm HashValue = Parent ? Parent->HashValue : 0;
      HashValue = (HashValue + Hash(&StreamValue)) % Result.Hashes.ElementCount;

      xml_token* OpenToken = PushToken(&Result, XmlOpenToken(StreamValue));
      xml_tag* OpenTag = XmlTag(OpenToken, Parent, HashValue, Memory);

      xml_tag** Bucket = Result.Hashes.Elements + HashValue;
      while (*Bucket) Bucket = &(*Bucket)->NextInHash;

      *Bucket = OpenTag;
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
                PushToken(&Result, XmlStringProperty(PropertyName, PropValue));
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
    PushToken(&Result, XmlOpenToken(TagName));
  }

  return Result;
}

inline void
Rewind(xml_token_stream *Stream)
{
  Stream->At = Stream->Start;
  return;
}

